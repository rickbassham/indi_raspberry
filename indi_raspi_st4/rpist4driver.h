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

#ifndef RPIST4DRIVER_H
#define RPIST4DRIVER_H

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#include <sys/time.h>
#include <time.h>

enum {
    RPIST4_NORTH = 0x1,
    RPIST4_SOUTH = 0x2,
    RPIST4_EAST = 0x4,
    RPIST4_WEST = 0x8
};

class RPIST4Driver
{
public:
    RPIST4Driver();
    virtual ~RPIST4Driver();

    bool startPulse(int direction);
    bool stopPulse(int direction);

    void setDebug(bool enable) { debug = enable; }

private:
    bool debug;

};

#endif // RPIST4Driver_H
