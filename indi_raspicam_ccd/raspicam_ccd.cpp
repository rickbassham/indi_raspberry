/*
    Driver type: raspicam Camera INDI Driver

    Copyright (C) 2013 Rick Bassham (brodrick DOT bassham AT gmail DOT com)

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA

*/

#include <memory>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#include <indidevapi.h>
#include <eventloop.h>
#include <indilogger.h>

extern "C" {
    #include "raspicam_driver.h"
    #include "raspicam_readimage.h"
}

#include "raspicam_ccd.h"

#define MAX_DEVICES         5      /* Max device cameraCount */
#define POLLMS              1000

static int cameraCount;
static RaspiCamCCD *cameras[MAX_DEVICES];

/**********************************************************
 *
 *  IMPORRANT: List supported camera models in initializer of deviceTypes structure
 *
 **********************************************************/

static void cleanup() {
  for (int i = 0; i < cameraCount; i++) {
    delete cameras[i];
  }
}

void ISInit()
{
  static bool isInit = false;
  if (!isInit)
  {
     // Let's just create one camera for now
     cameraCount = 1;
     cameras[0] = new RaspiCamCCD();

    atexit(cleanup);
    isInit = true;
  }
}

void ISGetProperties(const char *dev) {
  ISInit();
  for (int i = 0; i < cameraCount; i++) {
    RaspiCamCCD *camera = cameras[i];
    if (dev == NULL || !strcmp(dev, camera->name)) {
      camera->ISGetProperties(dev);
      if (dev != NULL)
        break;
    }
  }
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num) {
  ISInit();
  for (int i = 0; i < cameraCount; i++) {
    RaspiCamCCD *camera = cameras[i];
    if (dev == NULL || !strcmp(dev, camera->name)) {
      camera->ISNewSwitch(dev, name, states, names, num);
      if (dev != NULL)
        break;
    }
  }
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int num) {
  ISInit();
  for (int i = 0; i < cameraCount; i++) {
    RaspiCamCCD *camera = cameras[i];
    if (dev == NULL || !strcmp(dev, camera->name)) {
      camera->ISNewText(dev, name, texts, names, num);
      if (dev != NULL)
        break;
    }
  }
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num) {
  ISInit();
  for (int i = 0; i < cameraCount; i++) {
    RaspiCamCCD *camera = cameras[i];
    if (dev == NULL || !strcmp(dev, camera->name)) {
      camera->ISNewNumber(dev, name, values, names, num);
      if (dev != NULL)
        break;
    }
  }
}

void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n) {
  INDI_UNUSED(dev);
  INDI_UNUSED(name);
  INDI_UNUSED(sizes);
  INDI_UNUSED(blobsizes);
  INDI_UNUSED(blobs);
  INDI_UNUSED(formats);
  INDI_UNUSED(names);
  INDI_UNUSED(n);
}
void ISSnoopDevice(XMLEle *root) {
  INDI_UNUSED(root);
}


//==========================================================================
RaspiCamCCD::RaspiCamCCD()
{
    // For now let's set name to default name. In the future, we need to to support multiple devices per one driver
    if (*getDeviceName() == '\0')
        strncpy(name, getDefaultName(), MAXINDINAME);
    else
        strncpy(name, getDeviceName(), MAXINDINAME);

    raspicamdrv = NULL;
    on_off[0] = strdup("On");
    on_off[1] = strdup("Off");

    setVersion(1, 0);
}
//==========================================================================
RaspiCamCCD::~RaspiCamCCD()
{
    free(on_off[0]);
    free(on_off[1]);
    expTID = 0;
}

const char * RaspiCamCCD::getDefaultName()
{
    return (const char *)"RaspiCam CCD";
}

bool RaspiCamCCD::initProperties()
{
  // Init parent properties first
  INDI::CCD::initProperties();

  //We don't know how many items will be in the switch yet
  IUFillSwitchVector(&mIsoSP, NULL, 0, getDeviceName(), "ISO", "ISO", IMAGE_SETTINGS_TAB, IP_RW, ISR_1OFMANY, 60, IPS_IDLE);
  IUFillSwitchVector(&mFormatSP, NULL, 0, getDeviceName(), "CAPTURE_FORMAT", "Capture Format", IMAGE_SETTINGS_TAB, IP_RW, ISR_1OFMANY, 60, IPS_IDLE);

  IUFillSwitch(&transferFormatS[0], "FITS", "", ISS_ON);
  IUFillSwitch(&transferFormatS[1], "Native", "", ISS_OFF);
  IUFillSwitchVector(&transferFormatSP, transferFormatS, 2, getDeviceName(), "Transfer Format", "", IMAGE_SETTINGS_TAB, IP_RW, ISR_1OFMANY, 0, IPS_IDLE);

  Capability cap;

  cap.canAbort = false;
  cap.canBin = false;
  cap.canSubFrame = false;
  cap.hasCooler = false;
  cap.hasGuideHead = false;
  cap.hasShutter = false;
  cap.hasST4Port = true;

  SetCapability(&cap);

  return true;
}

void RaspiCamCCD::ISGetProperties(const char *dev)
{
  INDI::CCD::ISGetProperties(dev);

  if (isConnected())
  {
      if (mIsoSP.nsp > 0)
            defineSwitch(&mIsoSP);
      if (mFormatSP.nsp > 0)
        defineSwitch(&mFormatSP);

      defineSwitch(&transferFormatSP);
  }

  // Add Debug, Simulator, and Configuration controls
  addAuxControls();
}

bool RaspiCamCCD::updateProperties()
{
  INDI::CCD::updateProperties();

  if (isConnected())
  {
      if (mIsoSP.nsp > 0)
        defineSwitch(&mIsoSP);
      if (mFormatSP.nsp > 0)
        defineSwitch(&mFormatSP);

      defineSwitch(&transferFormatSP);

    // Dummy value
    PrimaryCCD.setPixelSize(5, 5);
    PrimaryCCD.setBPP(8);

    timerID = SetTimer(POLLMS);
  } else
  {
    if (mIsoSP.nsp > 0)
       deleteProperty(mIsoSP.name);
    if (mFormatSP.nsp > 0)
       deleteProperty(mFormatSP.name);

    deleteProperty(transferFormatSP.name);

    rmTimer(timerID);
  }

  return true;
}

bool RaspiCamCCD::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if(strcmp(dev,getDeviceName())==0)
    {
      // TODO: Handle new Text
      return true;
    }

    return INDI::CCD::ISNewText(dev, name, texts, names, n);
}

bool RaspiCamCCD::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{

  if (strcmp(dev, getDeviceName()) == 0)
  {
       DEBUGF(INDI::Logger::DBG_SESSION, "%s new switch.", name);


      if (!strcmp(name, mIsoSP.name))
      {
          if (IUUpdateSwitch(&mIsoSP, states, names, n) < 0)
              return false;

          for (int i = 0; i < mIsoSP.nsp; i++)
          {
              if (mIsoS[i].s == ISS_ON) {
                  raspicam_set_iso(raspicamdrv, i);
                  mIsoSP.s = IPS_OK;
                  IDSetSwitch(&mIsoSP, NULL);
                  break;
              }
          }
      }

      if (!strcmp(name, mFormatSP.name))
      {
          if (IUUpdateSwitch(&mFormatSP, states, names, n) < 0)
              return false;

          for (int i = 0; i < mFormatSP.nsp; i++)
          {
              if (mFormatS[i].s == ISS_ON) {
                  raspicam_set_format(raspicamdrv, i);
                  mFormatSP.s = IPS_OK;
                  IDSetSwitch(&mFormatSP, NULL);
                  break;
              }
          }
      }

      if (!strcmp(name, transferFormatSP.name))
      {
          IUUpdateSwitch(&transferFormatSP, states, names, n);
          transferFormatSP.s = IPS_OK;
          IDSetSwitch(&transferFormatSP, NULL);
          return true;
      }
  }

  return INDI::CCD::ISNewSwitch(dev, name, states, names, n);

}

bool RaspiCamCCD::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{

  if (strcmp(dev, getDeviceName()) == 0)
  {
      // TODO: Handle new Number
      return true;
  }

  return INDI::CCD::ISNewNumber(dev, name, values, names, n);
}

bool RaspiCamCCD::Connect()
{
  DEBUGF(INDI::Logger::DBG_SESSION, "%s connecting.", getDeviceName());

  int setidx;
  const char **options;
  int max_opts;

  if (! (raspicamdrv = raspicam_open()))
  {
      DEBUG(INDI::Logger::DBG_ERROR, "Can not open camera: Power OK?");
      return false;
  }

  DEBUGF(INDI::Logger::DBG_SESSION, "%s getting formats.", getDeviceName());

  if (mFormatS)
      free(mFormatS);
  setidx = raspicam_get_format_current(raspicamdrv);
  options = raspicam_get_formats(raspicamdrv, &max_opts);
  mFormatS = create_switch("FORMAT", options, max_opts, setidx);
  mFormatSP.sp = mFormatS;
  mFormatSP.nsp = max_opts;

  DEBUGF(INDI::Logger::DBG_SESSION, "%s getting isos.", getDeviceName());

  if (mIsoS)
      free(mIsoS);
  setidx = raspicam_get_iso_current(raspicamdrv);
  options = raspicam_get_iso(raspicamdrv, &max_opts);
  mIsoS = create_switch("ISO", options, max_opts, setidx);
  mIsoSP.sp = mIsoS;
  mIsoSP.nsp = max_opts;

  DEBUGF(INDI::Logger::DBG_SESSION, "%s is online.", getDeviceName());

  return true;

}

bool RaspiCamCCD::Disconnect()
{
   raspicam_close(raspicamdrv);
   raspicamdrv = NULL;
   DEBUGF(INDI::Logger::DBG_SESSION, "%s is offline.", getDeviceName());
  return true;

}

bool RaspiCamCCD::StartExposure(float duration)
{
    if (PrimaryCCD.isExposing())
    {
        DEBUG(INDI::Logger::DBG_ERROR, "RaspiCam driver is already exposing. Can not abort.");
        return false;
    }

    /* start new exposure with last ExpValues settings.
     * ExpGo goes busy. set timer to read when done
     */
    int expms = (int)ceil(duration*1000);

    PrimaryCCD.setExposureDuration(duration);

    if (raspicam_start_exposure(raspicamdrv, expms) < 0)
    {
        DEBUG(INDI::Logger::DBG_ERROR, "Error starting exposure");
        return false;
    }

    ExposureRequest = duration;
    gettimeofday(&ExpStart, NULL);
    InExposure = true;

    DEBUGF(INDI::Logger::DBG_SESSION, "Starting %g sec exposure", duration);

    return true;
}

float RaspiCamCCD::CalcTimeLeft()
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

void RaspiCamCCD::TimerHit()
{
  int timerID = -1;
  long timeleft=1e6;

  if (isConnected() == false)
    return;

  if (InExposure)
  {
    timeleft = CalcTimeLeft();

    if (timeleft < 1.0)
    {
      if (timeleft > 0.25) {
        //  a quarter of a second or more
        //  just set a tighter timer
        timerID = SetTimer(250);
      } else {
        if (timeleft > 0.07) {
          //  use an even tighter timer
          timerID = SetTimer(50);
        } else {
          //  it's real close now, so spin on it
          while (timeleft > 0)
          {

            int slv;
            slv = 100000 * timeleft;
            usleep(slv);
          }

          PrimaryCCD.setExposureLeft(0);
          InExposure = false;
          /* grab and save image */
          grabImage();
        }
      }
    }
    else
        DEBUGF(INDI::Logger::DBG_DEBUG, "Image not ready. Time left %ld", timeleft);


      PrimaryCCD.setExposureLeft(timeleft);

    }

  if (timerID == -1)
    SetTimer(POLLMS);
}

bool RaspiCamCCD::grabImage()
{
    //char ext[16];
    char *memptr = PrimaryCCD.getFrameBuffer();
	size_t memsize;
    int fd, naxis=2, w, h, bpp=8;


    if (transferFormatS[0].s == ISS_ON)
    {

        char tmpfile[] = "/tmp/indi_XXXXXX";

        //dcraw can't read from stdin, so we need to write to disk then read it back
        fd = mkstemp(tmpfile);

        raspicam_read_exposure_fd(raspicamdrv, fd);

        if (fd == -1)
        {
            DEBUG(INDI::Logger::DBG_ERROR, "Exposure failed to save image...");
            unlink(tmpfile);
            return false;
        }

        /* We're done exposing */
        DEBUG(INDI::Logger::DBG_SESSION, "Exposure done, downloading image...");


        if(strcasecmp(raspicam_get_file_extension(raspicamdrv), "jpg") == 0 ||
           strcasecmp(raspicam_get_file_extension(raspicamdrv), "jpeg") == 0)
        {
                if (read_jpeg(tmpfile, &memptr, &memsize, &naxis, &w, &h))
                {
                    DEBUG(INDI::Logger::DBG_ERROR, "Exposure failed to parse jpeg.");
                    unlink(tmpfile);
                    return false;
                }
        }
        else
        {
                if (read_dcraw(tmpfile, &memptr, &memsize, &naxis, &w, &h, &bpp))
                {
                    DEBUG(INDI::Logger::DBG_ERROR, "Exposure failed to parse raw image.");
                    unlink(tmpfile);
                    return false;
                }

                unlink(tmpfile);
        }

        PrimaryCCD.setImageExtension("fits");
    }
    else
    {
        raspicam_get_dimensions(raspicamdrv, &w, &h);
        int rc = raspicam_read_exposure(raspicamdrv);

        if (rc != 0)
        {
            DEBUG(INDI::Logger::DBG_ERROR, "Failed to expose.");
            return rc;
        }

        /* We're done exposing */
         DEBUG(INDI::Logger::DBG_DEBUG, "Exposure done, downloading image...");
         char *newMemptr = NULL;
         raspicam_get_buffer(raspicamdrv, (const char **)&newMemptr, &memsize);
         memptr = (char *)realloc(memptr, memsize); // We copy the obtained memory pointer to avoid freeing some raspicam memory
         memcpy(memptr, newMemptr, memsize); //

        PrimaryCCD.setImageExtension(raspicam_get_file_extension(raspicamdrv));
    }

    PrimaryCCD.setFrameBuffer(memptr);
    PrimaryCCD.setFrameBufferSize(memsize, false);
    PrimaryCCD.setResolution(w, h);
    PrimaryCCD.setFrame(0, 0, w, h);
    PrimaryCCD.setNAxis(naxis);
    PrimaryCCD.setBPP(bpp);

    ExposureComplete(&PrimaryCCD);
    return true;
}

ISwitch *RaspiCamCCD::create_switch(const char *basestr, const char **options, int max_opts, int setidx)
{
	int i;
	ISwitch *sw = (ISwitch *)calloc(sizeof(ISwitch), max_opts);
    ISwitch *one_sw = sw;

    char sw_name[MAXINDINAME];
    char sw_label[MAXINDILABEL];
    ISState sw_state;

    for(i = 0; i < max_opts; i++)
    {
        snprintf(sw_name, MAXINDINAME, "%s%d", basestr, i);
        strncpy(sw_label, options[i], MAXINDILABEL);
        sw_state = (i == setidx) ? ISS_ON : ISS_OFF;

        IUFillSwitch(one_sw++, sw_name, sw_label, sw_state);
	}
	return sw;
}


bool RaspiCamCCD::GuideNorth(float duration) {
  /**********************************************************
   *
   *
   *
   *  IMPORRANT: Put here your CCD Guide call
   *  Some CCD API support pulse guiding directly (i.e. without timers)
   *  Others implement GUIDE_ON and GUIDE_OFF for each direction, and you
   *  will have to start a timer and then stop it after the 'duration' seconds
   *  For an example on timer usage, please refer to indi-sx and indi-gpusb drivers
   *  available in INDI 3rd party repository
   *  If there is an error, report it back to client
   *  e.g.
   *  IDMessage(getDeviceName(), "Error, unable to guide due ...");
   *  return false;
   *
   *
   **********************************************************/

  return true;
}

bool RaspiCamCCD::GuideSouth(float duration) {
  /**********************************************************
   *
   *
   *
   *  IMPORRANT: Put here your CCD Guide call
   *  Some CCD API support pulse guiding directly (i.e. without timers)
   *  Others implement GUIDE_ON and GUIDE_OFF for each direction, and you
   *  will have to start a timer and then stop it after the 'duration' seconds
   *  For an example on timer usage, please refer to indi-sx and indi-gpusb drivers
   *  available in INDI 3rd party repository
   *  If there is an error, report it back to client
   *  e.g.
   *  IDMessage(getDeviceName(), "Error, unable to guide due ...");
   *  return false;
   *
   *
   **********************************************************/

  return true;

}

bool RaspiCamCCD::GuideEast(float duration) {
  /**********************************************************
   *
   *
   *
   *  IMPORRANT: Put here your CCD Guide call
   *  Some CCD API support pulse guiding directly (i.e. without timers)
   *  Others implement GUIDE_ON and GUIDE_OFF for each direction, and you
   *  will have to start a timer and then stop it after the 'duration' seconds
   *  For an example on timer usage, please refer to indi-sx and indi-gpusb drivers
   *  available in INDI 3rd party repository
   *  If there is an error, report it back to client
   *  e.g.
   *  IDMessage(getDeviceName(), "Error, unable to guide due ...");
   *  return false;
   *
   *
   **********************************************************/

  return true;

}

bool RaspiCamCCD::GuideWest(float duration) {
  /**********************************************************
   *
   *
   *
   *  IMPORRANT: Put here your CCD Guide call
   *  Some CCD API support pulse guiding directly (i.e. without timers)
   *  Others implement GUIDE_ON and GUIDE_OFF for each direction, and you
   *  will have to start a timer and then stop it after the 'duration' seconds
   *  For an example on timer usage, please refer to indi-sx and indi-gpusb drivers
   *  available in INDI 3rd party repository
   *  If there is an error, report it back to client
   *  e.g.
   *  IDMessage(getDeviceName(), "Error, unable to guide due ...");
   *  return false;
   *
   *
   **********************************************************/

  return true;
}
