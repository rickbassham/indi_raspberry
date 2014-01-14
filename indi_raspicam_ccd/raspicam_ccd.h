#ifndef RASPICAM_CCD_H
#define RASPICAM_CCD_H

#include <sys/types.h>
#include <unistd.h>
#include <indiccd.h>

struct popen2 {
    pid_t child_pid;
    int   from_child, to_child;
};

int popen2(const char *cmdline, struct popen2 *childinfo) {
    pid_t p;
    int pipe_stdin[2], pipe_stdout[2];

    if(pipe(pipe_stdin)) return -1;
    if(pipe(pipe_stdout)) return -1;

    printf("pipe_stdin[0] = %d, pipe_stdin[1] = %d\n", pipe_stdin[0], pipe_stdin[1]);
    printf("pipe_stdout[0] = %d, pipe_stdout[1] = %d\n", pipe_stdout[0], pipe_stdout[1]);

    p = fork();
    if(p < 0) return p; /* Fork failed */
    if(p == 0) { /* child */
        close(pipe_stdin[1]);
        dup2(pipe_stdin[0], 0);
        close(pipe_stdout[0]);
        dup2(pipe_stdout[1], 1);
        execl(cmdline, "", NULL);
        perror("execl"); exit(99);
    }
    childinfo->child_pid = p;
    childinfo->to_child = pipe_stdin[1];
    childinfo->from_child = pipe_stdout[0];
    return 0;
}

typedef struct
{
    int filesz; /* size of BMP file */
    short r1;   /* reserved - must be 0 */
    short r2;   /* reserved - must be 0 */
    int offset; /* offset to image data */
    int size;   /* size of remaining header (must be 40) */
    int width;  /* image width in pixels */
    int height; /* image height in pixels */
    short planes;   /* must be 1 */
    short bitcount; /* bits per pixel - 1,2,4,8,16,24,32 */
    int compression;/* must be 0 for uncompressed image */
    int sizeimage;  /* image size - may be 0 for uncompressed image */
    int xppm;   /* X pixels per metre (ignore) */
    int yppm;   /* Y pixels per metre (ignore) */
    int cmapentries;/* Number of colourmap entries */
    int clrimportant; /* number of important colours */
} bmp_header;

class RaspicamCCD : public INDI::CCD
{
public:
    RaspicamCCD();

    void ISGetProperties(const char *dev);

protected:
    // General device functions
    bool Connect();
    bool Disconnect();
    const char *getDefaultName();
    bool initProperties();
    bool updateProperties();

    // CCD specific functions
    bool StartExposure(float duration);
    void TimerHit();

private:
    // Utility functions
    float CalcTimeLeft();
    void setupParams();
    void grabImage();

    // Are we exposing?
    bool InExposure;
    // Struct to keep timing
    struct timeval ExpStart;

    float ExposureRequest;
    int timerID;

    struct popen2 child;
};

#endif // RASPICAM_CCD_H
