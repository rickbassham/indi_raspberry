#ifndef _RASPICAM_DRIVER_H_
#define _RASPICAM_DRIVER_H_

struct _raspicam_driver;
typedef struct _raspicam_driver raspicam_driver;

extern int raspicam_start_exposure(raspicam_driver *raspicam, unsigned int exptime_msec);
extern int raspicam_read_exposure(raspicam_driver *raspicam);
extern int raspicam_read_exposure_fd(raspicam_driver *raspicam, int fd);
extern const char **raspicam_get_formats(raspicam_driver *raspicam, int *cnt);
extern const char **raspicam_get_iso(raspicam_driver *raspicam, int *cnt);
extern void raspicam_set_iso(raspicam_driver *raspicam, int iso);
extern void raspicam_set_format(raspicam_driver *raspicam, int format);
extern int raspicam_get_format_current(raspicam_driver *raspicam);
extern int raspicam_get_iso_current(raspicam_driver *raspicam);
extern raspicam_driver *raspicam_open();
extern int raspicam_close(raspicam_driver *raspicam);
extern void raspicam_get_buffer(raspicam_driver *raspicam, const char **buffer, size_t *size);
extern void raspicam_free_buffer(raspicam_driver *raspicam);
extern const char *raspicam_get_file_extension(raspicam_driver *raspicam);
extern void raspicam_show_options(raspicam_driver *raspicam);
extern int raspicam_get_dimensions(raspicam_driver *raspicam, int *width, int *height);

#endif
