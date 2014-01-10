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

#define PI_COBBLER_PIN_18 1
#define PI_COBBLER_PIN_23 4
#define PI_COBBLER_PIN_24 5
#define PI_COBBLER_PIN_25 6

#define RA_PLUS     PI_COBBLER_PIN_18
#define DEC_PLUS    PI_COBBLER_PIN_23
#define DEC_MINUS   PI_COBBLER_PIN_24
#define RA_MINUS    PI_COBBLER_PIN_25

RPIST4Driver::RPIST4Driver()
{
    debug=false;

    wiringPiSetup();

    pinMode(RA_PLUS,    OUTPUT);
    pinMode(DEC_PLUS,   OUTPUT);
    pinMode(DEC_MINUS,  OUTPUT);
    pinMode(RA_MINUS,   OUTPUT);
}

RPIST4Driver::~RPIST4Driver()
{
}

bool RPIST4Driver::startPulse(int direction)
{
    int rc=0;

    switch (direction)
    {
        case RPIST4_NORTH:
            digitalWrite(RA_MINUS, LOW);  // Off
            digitalWrite(RA_PLUS, HIGH);  // On
            break;

        case RPIST4_WEST:
            digitalWrite(DEC_MINUS, LOW);  // Off
            digitalWrite(DEC_PLUS, HIGH);  // On
            break;

        case RPIST4_SOUTH:
            digitalWrite(RA_PLUS, LOW);  // Off
            digitalWrite(RA_MINUS, HIGH);  // On
            break;

        case RPIST4_EAST:
            digitalWrite(DEC_PLUS, LOW);  // Off
            digitalWrite(DEC_MINUS, HIGH);  // On
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
            digitalWrite(RA_PLUS, LOW);  // Off
            break;

        case RPIST4_WEST:
            digitalWrite(DEC_PLUS, LOW);  // Off
            break;

        case RPIST4_SOUTH:
            digitalWrite(RA_MINUS, LOW);  // Off
            break;

        case RPIST4_EAST:
            digitalWrite(DEC_MINUS, LOW);  // Off
            break;
    }

    return true;
}
