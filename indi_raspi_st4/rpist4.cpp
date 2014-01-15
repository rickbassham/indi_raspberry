/*******************************************************************************
Copyright(c) 2014 Rick Bassham. All rights reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU Library General Public License
along with this library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.

The full GNU General Public License is included in this distribution in the
file called LICENSE.
*******************************************************************************/

#include "rpist4.h"
#include "rpist4driver.h"
#include <memory>

#define POLLMS 250

// We declare an auto pointer to rpiGuide.
std::auto_ptr<RPIST4> rpiGuide(0);

void ISInit()
{
   static int isInit =0;

   if (isInit == 1)
       return;

    isInit = 1;

    if(rpiGuide.get() == 0)
        rpiGuide.reset(new RPIST4());
    //IEAddTimer(POLLMS, ISPoll, NULL);
}

void ISGetProperties(const char *dev)
{
    ISInit();
    rpiGuide->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num)
{
    ISInit();
    rpiGuide->ISNewSwitch(dev, name, states, names, num);
}

void ISNewText(	const char *dev, const char *name, char *texts[], char *names[], int num)
{
    ISInit();
    rpiGuide->ISNewText(dev, name, texts, names, num);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num)
{
    ISInit();
    rpiGuide->ISNewNumber(dev, name, values, names, num);
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
    INDI_UNUSED(root);
}

RPIST4::RPIST4()
{
    driver = new RPIST4Driver();
    WEDir = NSDir = 0;
}

RPIST4::~RPIST4()
{
    delete (driver);
}

const char * RPIST4::getDefaultName()
{
    return (char *)"RPIST4";
}

bool RPIST4::Connect()
{
    driver->setDebug(isDebug());

    IDMessage(getDeviceName(), "RPIST4 is online.");

    return true;
}

bool RPIST4::Disconnect()
{
    IDMessage(getDeviceName(), "RPIST4 is offline.");

    return true;
}

bool RPIST4::initProperties()
{
    initGuiderProperties(getDeviceName(), MAIN_CONTROL_TAB);

    IUFillNumber(&GPIOPinsN[0],"RA_PLUS","RA+ ","%d",0,8,1,1);
    IUFillNumber(&GPIOPinsN[1],"RA_MINUS","RA-","%d",0,8,1,6);
    IUFillNumber(&GPIOPinsN[2],"DEC_PLUS","DEC+","%d",0,8,1,4);
    IUFillNumber(&GPIOPinsN[3],"DEC_MINUS","DEC-","%d",0,8,1,5);
    IUFillNumberVector(GPIOPinsNP,GPIOPinsN,4,getDeviceName(),"GPIO_PINS","GPIO Pins","GPIO Setup",IP_RW,60,IPS_IDLE);

    addDebugControl();

    return INDI::DefaultDevice::initProperties();
}

bool RPIST4::updateProperties()
{
    INDI::DefaultDevice::updateProperties();

    defineNumber(GPIOPinsNP);

    if (isConnected())
    {
        defineNumber(&GuideNSNP);
        defineNumber(&GuideWENP);
    }
    else
    {
        deleteProperty(GuideNSNP.name);
        deleteProperty(GuideWENP.name);
    }

    return true;
}

void RPIST4::ISGetProperties (const char *dev)
{
    INDI::DefaultDevice::ISGetProperties(dev);
}

bool RPIST4::ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n)
{
    if (strcmp(dev,getDeviceName())==0)
    {
        if (!strcmp(name,GuideNSNP.name) || !strcmp(name,GuideWENP.name))
        {
            processGuiderProperties(name, values, names, n);
            return true;
        }

        if (strcmp(name,"GPIO_PINS")==0)
        {
            GPIOPinsNP->s=IPS_OK;
            IUUpdateNumber(GPIOPinsNP,values,names,n);

            delete driver;
            driver = new RPIST4Driver(GPIOPinsN[0].value, GPIOPinsN[1].value, GPIOPinsN[2].value, GPIOPinsN[3].value);

            IDSetNumber(GPIOPinsNP, NULL);
            return true;
        }
    }

    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

bool RPIST4::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
    return INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n);
}

bool RPIST4::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    return INDI::DefaultDevice::ISNewText(dev, name, texts, names, n);
}

bool RPIST4::ISSnoopDevice (XMLEle *root)
{
    return INDI::DefaultDevice::ISSnoopDevice(root);
}

void RPIST4::debugTriggered(bool enable)
{
    driver->setDebug(enable);
}

float RPIST4::CalcWEPulseTimeLeft()
{
    double timesince;
    double timeleft;
    struct timeval now;
    gettimeofday(&now,NULL);

    timesince = (double)(now.tv_sec * 1000.0 + now.tv_usec/1000) - (double)(WEPulseStart.tv_sec * 1000.0 + WEPulseStart.tv_usec/1000);
    timesince = timesince / 1000;

    timeleft = WEPulseRequest - timesince;
    return timeleft;
}

float RPIST4::CalcNSPulseTimeLeft()
{
    double timesince;
    double timeleft;
    struct timeval now;
    gettimeofday(&now,NULL);

    timesince = (double)(now.tv_sec * 1000.0 + now.tv_usec/1000) - (double)(NSPulseStart.tv_sec * 1000.0 + NSPulseStart.tv_usec/1000);
    timesince = timesince / 1000;


    timeleft = NSPulseRequest - timesince;
    return timeleft;
}

void RPIST4::TimerHit()
{
    float timeleft;
    int rc;

    if(InWEPulse)
    {
        timeleft=CalcWEPulseTimeLeft();

        if(timeleft < 1.0)
        {
            if(timeleft > 0.25)
            {
                //  a quarter of a second or more
                //  just set a tighter timer
                WEtimerID = SetTimer(250);
            }
            else
            {
                if(timeleft >0.07)
                {
                    //  use an even tighter timer
                    WEtimerID = SetTimer(50);
                }
                else
                {
                    //  it's real close now, so spin on it
                    while(timeleft > 0)
                    {
                        int slv;
                        slv=100000*timeleft;
                        //IDLog("usleep %d\n",slv);
                        usleep(slv);
                        timeleft=CalcWEPulseTimeLeft();
                    }

                    IDMessage(getDeviceName(), "Stop Pulse %d.", WEDir);

                    driver->stopPulse(WEDir);
                    InWEPulse = false;

                    // If we have another pulse, keep going
                    if (!InNSPulse)
                        SetTimer(250);
                }
            }
        }
        else if (!InNSPulse)
        {
            WEtimerID = SetTimer(250);
        }
    }

    if(InNSPulse)
    {
        timeleft=CalcNSPulseTimeLeft();

        if(timeleft < 1.0)
        {
            if(timeleft > 0.25)
            {
                //  a quarter of a second or more
                //  just set a tighter timer
                NStimerID =  SetTimer(250);
            }
            else
            {
                if(timeleft >0.07)
                {
                    //  use an even tighter timer
                    NStimerID = SetTimer(50);
                }
                else
                {
                    //  it's real close now, so spin on it
                    while(timeleft > 0)
                    {
                        int slv;
                        slv=100000*timeleft;
                        //IDLog("usleep %d\n",slv);
                        usleep(slv);
                        timeleft=CalcNSPulseTimeLeft();
                    }

                    IDMessage(getDeviceName(), "Stop Pulse %d.", NSDir);

                    driver->stopPulse(NSDir);
                    InNSPulse = false;
                }
            }
        } else
        {
            NStimerID = SetTimer(250);
        }
    }

}

bool RPIST4::GuideNorth(float ms)
{
    RemoveTimer(NStimerID);

    driver->startPulse(RPIST4_NORTH);

    NSDir = RPIST4_NORTH;

    DEBUG(INDI::Logger::DBG_DEBUG, "Starting NORTH guide");
    IDMessage(getDeviceName(), "Starting NORTH guide.");

    if (ms <= POLLMS)
    {
        usleep(ms*1000);

        driver->stopPulse(RPIST4_NORTH);
        return true;
    }

    NSPulseRequest=ms/1000.0;
    gettimeofday(&NSPulseStart,NULL);
    InNSPulse=true;

    NStimerID = SetTimer(ms-50);

    return true;
}

bool RPIST4::GuideSouth(float ms)
{
    RemoveTimer(NStimerID);

    driver->startPulse(RPIST4_SOUTH);

    DEBUG(INDI::Logger::DBG_DEBUG, "Starting SOUTH guide");
    IDMessage(getDeviceName(), "Starting SOUTH guide.");

    NSDir = RPIST4_SOUTH;

    if (ms <= POLLMS)
    {
        usleep(ms*1000);

        driver->stopPulse(RPIST4_SOUTH);
        return true;
    }

    NSPulseRequest=ms/1000.0;
    gettimeofday(&NSPulseStart,NULL);
    InNSPulse=true;

    NStimerID = SetTimer(ms-50);

    return true;
}

bool RPIST4::GuideEast(float ms)
{
    RemoveTimer(WEtimerID);

    driver->startPulse(RPIST4_EAST);

    DEBUG(INDI::Logger::DBG_DEBUG, "Starting EAST guide");
    IDMessage(getDeviceName(), "Starting EAST guide.");

    WEDir = RPIST4_EAST;

    if (ms <= POLLMS)
    {
        usleep(ms*1000);

        driver->stopPulse(RPIST4_EAST);
        return true;
    }

    WEPulseRequest=ms/1000.0;
    gettimeofday(&WEPulseStart,NULL);
    InWEPulse=true;

    WEtimerID = SetTimer(ms-50);

    return true;
}

bool RPIST4::GuideWest(float ms)
{
    RemoveTimer(WEtimerID);

    driver->startPulse(RPIST4_WEST);

    DEBUG(INDI::Logger::DBG_DEBUG, "Starting WEST guide");
    IDMessage(getDeviceName(), "Starting WEST guide.");

    WEDir = RPIST4_WEST;

    if (ms <= POLLMS)
    {
        usleep(ms*1000);

        driver->stopPulse(RPIST4_WEST);
        return true;
    }

    WEPulseRequest=ms/1000.0;
    gettimeofday(&WEPulseStart,NULL);
    InWEPulse=true;

    WEtimerID = SetTimer(ms-50);

    return true;
}
