/*******************************************************************************
  Copyright(c) 2009 Geoffrey Hausheer. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.
*******************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <sys/time.h>
#include <time.h>

#include <pthread.h>

#include "raspicam_driver.h"

struct _raspicam_driver {
	int			exposure_cnt;
	double			*exposure;

	int			width;
	int			height;

	int			iso;
	int			format;
};

static int debug = 1;

static void gp_dprintf(const char *format, ...) {
    va_list args;

    if (debug) {
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

int raspicam_start_exposure(raspicam_driver *raspicam, unsigned int exptime_msec)
{
    return 0;
}

int raspicam_read_exposure_fd(raspicam_driver *raspicam, int fd)
{
    return 0;
}

int raspicam_read_exposure(raspicam_driver *raspicam)
{
    return raspicam_read_exposure_fd(raspicam, -1);
}

const char **raspicam_get_formats(raspicam_driver *raspicam, int *cnt)
{
	const char ** sub_str = malloc(2 * sizeof(char*));

	sub_str[0] = "jpg";
	sub_str[1] = "bmp";

	*cnt = 2;

	return sub_str;
}

const char **raspicam_get_iso(raspicam_driver *raspicam, int *cnt)
{
	const char ** sub_str = malloc(2 * sizeof(char*));

	sub_str[0] = "100";
	sub_str[1] = "200";

	*cnt = 2;

	return sub_str;
}

void raspicam_set_iso(raspicam_driver *raspicam, int iso)
{
    fprintf(stderr, "WARNING: Could not set iso\n");
}

void raspicam_set_format(raspicam_driver *raspicam, int format)
{
    fprintf(stderr, "WARNING: Could not set format\n");
}

int raspicam_get_format_current(raspicam_driver *raspicam)
{
    return 0;
}

int raspicam_get_iso_current(raspicam_driver *raspicam)
{
    return 0;
}

raspicam_driver *raspicam_open()
{
    raspicam_driver *raspicam;

    raspicam = calloc(sizeof(raspicam_driver), 1);

    raspicam->format = -1;
    raspicam->iso = -1;

    gp_dprintf("raspicam initialized\n");

    return raspicam;
}

int raspicam_close(raspicam_driver *raspicam)
{
    if(! raspicam)
        return 0;

    free(raspicam);

    return 0;
}

void raspicam_show_options(raspicam_driver *raspicam)
{
}

void raspicam_get_buffer(raspicam_driver *raspicam, const char **buffer, size_t *size)
{
}

void raspicam_free_buffer(raspicam_driver *raspicam)
{
}

const char *raspicam_get_file_extension(raspicam_driver *raspicam)
{
    return "jpg";
}

int raspicam_get_dimensions(raspicam_driver *raspicam, int *width, int *height)
{
    *width = raspicam->width;
    *height = raspicam->height;
    return 0;
}
