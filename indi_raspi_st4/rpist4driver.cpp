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

#include "rpist4driver.h"

#include <wiringPi.h>

// See this website for pins.
// http://wiringpi.com/pins/

#define PI_COBBLER_PIN_18 1
#define PI_COBBLER_PIN_23 4
#define PI_COBBLER_PIN_24 5
#define PI_COBBLER_PIN_25 6

#define RA_PLUS     PI_COBBLER_PIN_18
#define RA_MINUS    PI_COBBLER_PIN_25
#define DEC_PLUS    PI_COBBLER_PIN_23
#define DEC_MINUS   PI_COBBLER_PIN_24

RPIST4Driver::RPIST4Driver()
{
    debug=false;

    wiringPiSetup();

    pinMode(RA_PLUS,    OUTPUT);
    pinMode(RA_MINUS,   OUTPUT);
    pinMode(DEC_PLUS,   OUTPUT);
    pinMode(DEC_MINUS,  OUTPUT);
}

RPIST4Driver::RPIST4Driver()
{
    debug=false;

    wiringPiSetup();

    raPlus = RA_PLUS;
    raMinus = RA_MINUS;
    decPlus = DEC_PLUS;
    decMinus = DEC_MINUS;

    pinMode(raPlus,     OUTPUT);
    pinMode(raMinus,    OUTPUT);
    pinMode(decPlus,    OUTPUT);
    pinMode(decMinus,   OUTPUT);
}

RPIST4Driver::RPIST4Driver(int raPlusPin, int raMinusPin, int decPlusPin, int decMinusPin)
{
    debug=false;

    wiringPiSetup();

    raPlus = raPlusPin;
    raMinus = raMinusPin;
    decPlus = decPlusPin;
    decMinus = decMinusPin;

    pinMode(raPlus,     OUTPUT);
    pinMode(raMinus,    OUTPUT);
    pinMode(decPlus,    OUTPUT);
    pinMode(decMinus,   OUTPUT);
}

RPIST4Driver::~RPIST4Driver()
{
    stopPulse(RPIST4_NORTH);
    stopPulse(RPIST4_WEST);
    stopPulse(RPIST4_SOUTH);
    stopPulse(RPIST4_EAST);
}

bool RPIST4Driver::startPulse(int direction)
{
    int rc=0;

    switch (direction)
    {
        case RPIST4_NORTH:
            digitalWrite(decMinus, LOW);  // Off
            digitalWrite(decPlus, HIGH);  // On
            break;

        case RPIST4_WEST:
            digitalWrite(raMinus, LOW);  // Off
            digitalWrite(raPlus, HIGH);  // On
            break;

        case RPIST4_SOUTH:
            digitalWrite(decPlus, LOW);  // Off
            digitalWrite(decMinus, HIGH);  // On
            break;

        case RPIST4_EAST:
            digitalWrite(raPlus, LOW);  // Off
            digitalWrite(raMinus, HIGH);  // On
            break;
    }

    return true;
}

bool RPIST4Driver::stopPulse(int direction)
{
   int rc=0;

    switch (direction)
    {
        case RPIST4_NORTH:
            digitalWrite(decPlus, LOW);  // Off
            break;

        case RPIST4_WEST:
            digitalWrite(raPlus, LOW);  // Off
            break;

        case RPIST4_SOUTH:
            digitalWrite(decMinus, LOW);  // Off
            break;

        case RPIST4_EAST:
            digitalWrite(raMinus, LOW);  // Off
            break;
    }

    return true;
}
