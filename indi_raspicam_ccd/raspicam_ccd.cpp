#include <sys/time.h>
#include <memory>
#include <signal.h>

#include "raspicam_ccd.h"

const int POLLMS = 500;   /* Polling interval 500 ms */

std::auto_ptr<RaspicamCCD> simpleCCD(0);

void ISInit()
{
    static int isInit =0;
    if (isInit == 1)
        return;

    isInit = 1;

    if(simpleCCD.get() == 0)
        simpleCCD.reset(new RaspicamCCD());
}

void ISGetProperties(const char *dev)
{
    ISInit();
    simpleCCD->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num)
{
    ISInit();
    simpleCCD->ISNewSwitch(dev, name, states, names, num);
}

void ISNewText(	const char *dev, const char *name, char *texts[], char *names[], int num)
{
    ISInit();
    simpleCCD->ISNewText(dev, name, texts, names, num);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num)
{
    ISInit();
    simpleCCD->ISNewNumber(dev, name, values, names, num);
}

void ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n)
{
    INDI_UNUSED(dev);
    INDI_UNUSED(name);
    INDI_UNUSED(sizes);
    INDI_UNUSED(blobsizes);
    INDI_UNUSED(blobs);
    INDI_UNUSED(formats);
    INDI_UNUSED(names);
    INDI_UNUSED(n);
}

void ISSnoopDevice (XMLEle *root)
{
    ISInit();
    simpleCCD->ISSnoopDevice(root);
}


RaspicamCCD::RaspicamCCD()
{
    InExposure = false;
}

/**************************************************************************************
** Client is asking us to establish connection to the device
***************************************************************************************/
bool RaspicamCCD::Connect()
{
//    int nRet = popen2("/home/rick/rpi/projects/indi_raspberry/indi_raspicam_ccd/test arg", &child);
    int nRet = popen2("raspistill -signal -encoding jpg -width 1296 -height 972 -ex night -output /tmp/indi_raspicam_ccd.jpg", &child);

    if (nRet == 0)
    {
        IDMessage(getDeviceName(), "Raspicam CCD connected successfully!");
        IDMessage(getDeviceName(), "Connect PID: %d", child.child_pid);

        SetTimer(POLLMS);

        return true;
    }
    else
    {
        return false;
    }
}

/**************************************************************************************
** Client is asking us to terminate connection to the device
***************************************************************************************/
bool RaspicamCCD::Disconnect()
{
    IDMessage(getDeviceName(), "Raspicam CCD disconnected successfully!");
    return true;
}

/**************************************************************************************
** INDI is asking us for our default device name
***************************************************************************************/
const char * RaspicamCCD::getDefaultName()
{
    return "Raspicam CCD";
}

/**************************************************************************************
** INDI is asking us to init our properties.
***************************************************************************************/
bool RaspicamCCD::initProperties()
{
    // Must init parent properties first!
    INDI::CCD::initProperties();

    // We set the CCD capabilities
    Capability cap;

    cap.canAbort = false;
    cap.canBin   = false;
    cap.canSubFrame = false;
    cap.hasCooler = false;
    cap.hasGuideHead = false;
    cap.hasShutter = true;
    cap.hasST4Port = false;

    SetCapability(&cap);

    // Add Debug, Simulator, and Configuration controls
    addAuxControls();

    return true;
}

/**************************************************************************************
** INDI is asking us to submit list of properties for the device
***************************************************************************************/
void RaspicamCCD::ISGetProperties(const char *dev)
{
    INDI::CCD::ISGetProperties(dev);
}

/********************************************************************************************
** INDI is asking us to update the properties because there is a change in CONNECTION status
** This fucntion is called whenever the device is connected or disconnected.
*********************************************************************************************/
bool RaspicamCCD::updateProperties()
{
    // Call parent update properties first
    INDI::CCD::updateProperties();

    if (isConnected())
    {
        // Let's get parameters now from CCD
        setupParams();

        // Start the timer
        SetTimer(POLLMS);
    }

    return true;
}

/**************************************************************************************
** Setting up CCD parameters
***************************************************************************************/
void RaspicamCCD::setupParams()
{
    // Our CCD is an 24 bit CCD, 2592x1944 resolution, with 1.4um square pixels.
    SetCCDParams(1296, 972, 8, 1.4, 1.4);

    // Let's calculate how much memory we need for the primary CCD buffer
    int nbuf;
    nbuf=PrimaryCCD.getXRes()*PrimaryCCD.getYRes() * PrimaryCCD.getBPP()/8;

    //nbuf += 2 + sizeof(bmp_header);

    PrimaryCCD.setFrameBufferSize(nbuf);
}

/**************************************************************************************
** Client is asking us to start an exposure
***************************************************************************************/
bool RaspicamCCD::StartExposure(float duration)
{
    ExposureRequest=duration;

    // Since we have only have one CCD with one chip, we set the exposure duration of the primary CCD
    PrimaryCCD.setExposureDuration(duration);

    gettimeofday(&ExpStart,NULL);

    kill(child.child_pid, SIGUSR1);

    InExposure=true;

    // We're done
    return true;
}

/**************************************************************************************
** How much longer until exposure is done?
***************************************************************************************/
float RaspicamCCD::CalcTimeLeft()
{
    double timesince;
    double timeleft;
    struct timeval now;
    gettimeofday(&now,NULL);

    timesince=(double)(now.tv_sec * 1000.0 + now.tv_usec/1000) - (double)(ExpStart.tv_sec * 1000.0 + ExpStart.tv_usec/1000);
    timesince=timesince/1000;

    timeleft=ExposureRequest-timesince;
    return timeleft;
}

/**************************************************************************************
** Main device loop. We check for exposure progress here
***************************************************************************************/
void RaspicamCCD::TimerHit()
{
    int timerID = -1;
    long timeleft = 1e6;

    if (isConnected() == false)
    {
        return;
    }

    if (InExposure)
    {
        timeleft = CalcTimeLeft();

        if (timeleft < 1.0)
        {
            if (timeleft > 0.25)
            {
                //  a quarter of a second or more
                //  just set a tighter timer
                timerID = SetTimer(250);
            }
            else
            {
                if (timeleft > 0.07)
                {
                    //  use an even tighter timer
                    timerID = SetTimer(50);
                }
                else
                {
                //  it's real close now, so spin on it
                    while (timeleft > 0)
                    {
                        int slv;
                        slv = 100000 * timeleft;
                        usleep(slv);
                    }

                    IDMessage(getDeviceName(), "Exposure done, downloading image...");

                    // Set exposure left to zero
                    PrimaryCCD.setExposureLeft(0);

                    // We're no longer exposing...
                    InExposure = false;

                    /* grab and save image */
                    grabImage();
                }
            }
        }
        else
        {
            DEBUGF(INDI::Logger::DBG_DEBUG, "Image not ready. Time left %ld", timeleft);
        }

        PrimaryCCD.setExposureLeft(timeleft);
    }

    if (timerID == -1)
        SetTimer(POLLMS);
}

void RaspicamCCD::grabImage()
{
    char *memptr = PrimaryCCD.getFrameBuffer();
    size_t memsize;
    int naxis=2, w, h, bpp=8;

    DEBUG(INDI::Logger::DBG_ERROR, "opening.");

    const char* filename = "/tmp/indi_raspicam_ccd.jpg";
//    const char* filename = "/home/rick/rpi/projects/indi_raspberry/indi_raspicam_ccd/test.jpg";

    if (read_jpeg(filename, &memptr, &memsize, &naxis, &w, &h))
    {
        DEBUG(INDI::Logger::DBG_ERROR, "Exposure failed to parse jpeg.");
        return;
    }

    PrimaryCCD.setImageExtension("fits");
    PrimaryCCD.setFrameBuffer(memptr);
    PrimaryCCD.setFrameBufferSize(memsize, false);
    PrimaryCCD.setResolution(w, h);
    PrimaryCCD.setFrame(0, 0, w, h);
    PrimaryCCD.setNAxis(naxis);
    PrimaryCCD.setBPP(bpp);

    IDMessage(getDeviceName(), "Download complete.");

    // Let INDI::CCD know we're done filling the image buffer
    ExposureComplete(&PrimaryCCD);
}

int RaspicamCCD::read_jpeg(const char *filename, char **memptr, size_t *memsize, int *naxis, int *w, int *h )
{
    int row;
    unsigned char *r_data = NULL, *g_data, *b_data;

    /* these are standard libjpeg structures for reading(decompression) */
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    /* libjpeg data structure for storing one row, that is, scanline of an image */
    JSAMPROW row_pointer[1] = {NULL};

    FILE *infile = fopen( filename, "rb" );
    int i = 0;

    /* here we set up the standard libjpeg error handler */
    cinfo.err = jpeg_std_error( &jerr );
    /* setup decompression process and source, then read JPEG header */
    jpeg_create_decompress( &cinfo );
    /* this makes the library read from infile */
    jpeg_stdio_src( &cinfo, infile );

    /* reading the image header which contains image information */
    int nret = jpeg_read_header( &cinfo, TRUE );

    /* Start decompression jpeg here */
    jpeg_start_decompress( &cinfo );

    *memsize = cinfo.output_width * cinfo.output_height * cinfo.num_components;
    *memptr = (char*)realloc(*memptr, *memsize);
    char *oldmem = *memptr; // if you do some ugly pointer math, remember to restore the original pointer or some random crashes will happen. This is why I do not like pointers!!
    *naxis = cinfo.num_components;
    *w = cinfo.output_width;
    *h = cinfo.output_height;

    /* now actually read the jpeg into the raw buffer */
    row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
    if (cinfo.num_components)
    {
        r_data = (unsigned char *) *memptr;
        g_data = r_data + cinfo.output_width * cinfo.output_height;
        b_data = r_data + 2 * cinfo.output_width * cinfo.output_height;
    }

    /* read one scan line at a time */
    for (row = 0; row < cinfo.image_height; row++)
    {
        unsigned char *ppm8 = row_pointer[0];
        jpeg_read_scanlines( &cinfo, row_pointer, 1 );

        if (cinfo.num_components == 3)
        {
            for (i = 0; i < cinfo.output_width; i++)
            {
                *r_data++ = *ppm8++;
                *g_data++ = *ppm8++;
                *b_data++ = *ppm8++;
            }
        }
        else
        {
            memcpy(*memptr, ppm8, cinfo.output_width);
            *memptr += cinfo.output_width;
        }
    }

    /* wrap up decompression, destroy objects, free pointers and close open files */
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );

    if (row_pointer[0] )
        free( row_pointer[0] );

    if(infile)
        fclose( infile );

    *memptr = oldmem;

    return 0;
}
