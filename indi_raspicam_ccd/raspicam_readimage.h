#ifndef _RASPICAM_READIMAGE_H_
#define _RASPICAM_READIMAGE_H_
extern int read_dcraw(const char *filename, char **memptr, size_t *memsize, int *n_axis, int *w, int *h, int *bitsperpixel);
extern int read_jpeg(const char *filename, char **memptr, size_t *memsize, int *n_axis, int *w, int *h);
#endif
