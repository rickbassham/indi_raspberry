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
    int nRet = popen2("/home/rick/rpi/projects/indi_raspberry/indi_raspicam_ccd/test", &child);
//    int nRet = popen2("raspistill -signal -encoding bmp -width 1296 -height 972 -ex night -output -", &child);

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
    SetCCDParams(1296, 972, 24, 1.4, 1.4);

    // Let's calculate how much memory we need for the primary CCD buffer
    int nbuf;
    nbuf=PrimaryCCD.getXRes()*PrimaryCCD.getYRes() * PrimaryCCD.getBPP()/8;

    nbuf += 2 + sizeof(bmp_header);

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
    // Get width and height
    int width = PrimaryCCD.getXRes() * PrimaryCCD.getBPP() / 8;
    int height = PrimaryCCD.getYRes();
/*
    char magicBuf[2];

    read(child.from_child, magicBuf, sizeof(magicBuf));

    if (strncmp(magicBuf, "BM", 2))
    {
        DEBUG(INDI::Logger::DBG_ERROR, "ERROR: BMP Magic Bytes is not BM");
    }

    bmp_header header;

    read(child.from_child, (char*)&header, sizeof(header));

    if (header.size != 40)
    {
        DEBUGF(INDI::Logger::DBG_ERROR, "ERROR: BMP Header size is not 40: %d", header.size);
    }

    if (header.bitcount != 24)
    {
        DEBUGF(INDI::Logger::DBG_ERROR, "ERROR: BMP Bit Count is not 24: %d", header.bitcount);
    }

    if (header.width != PrimaryCCD.getXRes())
    {
        DEBUGF(INDI::Logger::DBG_ERROR, "ERROR: BMP Width is not %d: %d", PrimaryCCD.getXRes(), header.width);
    }

    if (header.height != PrimaryCCD.getYRes())
    {
        DEBUGF(INDI::Logger::DBG_ERROR, "ERROR: BMP Height is not %d: %d", PrimaryCCD.getYRes(), header.height);
    }
*/
    // Let's get a pointer to the frame buffer
    char * image = PrimaryCCD.getFrameBuffer();

    read(child.from_child, image, 2 + sizeof(bmp_header) + width * height);

    PrimaryCCD.setImageExtension("bmp");

    IDMessage(getDeviceName(), "Download complete.");

    // Let INDI::CCD know we're done filling the image buffer
    ExposureComplete(&PrimaryCCD);
}
