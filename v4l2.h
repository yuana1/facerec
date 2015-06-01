#ifndef V4L2_H
#define V4L2_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
//#define ANDROID_ENV

#ifdef ANDROID_ENV
#define LOG LOGV
#else
#define LOG printf
#endif
#define CAMERA_DEVICE "/dev/video0"
#define CAPTURE_FILE "frame.jpeg"
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV
//#define VIDEO_FORMAT V4L2_PIX_FMT_JPEG
#define BUFFER_COUNT 1


class video
{
private:
    struct fimc_buffer {
        int length;
        void *start;
    } framebuf[BUFFER_COUNT];

    int fd;
    bool sign3;
public:
   // explicit video(void);
    int init_video();
    int get_data();
    void release_video();
    int yuvtorgbO(unsigned char *rgb, unsigned int width,unsigned int height);
    int yuvtorgb(int y, int u, int v);
    struct v4l2_buffer buf;
};


#endif // V4L2_H
