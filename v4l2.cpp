#include "v4l2.h"

int video::init_video()
{
    struct v4l2_fmtdesc fmtdesc;
    int i, ret;
    // Open Device

    fd = open(CAMERA_DEVICE, O_RDWR, 0);
    if (fd < 0) {
        LOG("Open %s failed/n", CAMERA_DEVICE);
        return -1;
    }
    // Query Capability
    struct v4l2_capability cap;
    ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
    if (ret < 0) {
        LOG("VIDIOC_QUERYCAP failed (%d)/n", ret);
        return ret;
    }
    // Print capability infomations
    LOG("Capability Informations:/n");
    LOG(" driver: %s/n", cap.driver);
    LOG(" card: %s/n", cap.card);
    LOG(" bus_info: %s/n", cap.bus_info);
    LOG(" version: %08X/n", cap.version);
    LOG(" capabilities: %08X/n", cap.capabilities);
    // Set Stream Format
    fmtdesc.index=0;
 fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
 printf("Support format:\n");
 while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
 {
     printf("SUPPORT\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
     fmtdesc.index++;
 }
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = VIDEO_WIDTH;
    fmt.fmt.pix.height = VIDEO_HEIGHT;
    fmt.fmt.pix.pixelformat = VIDEO_FORMAT;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
    if (ret < 0) {
        LOG("VIDIOC_S_FMT failed (%d)/n", ret);
        return ret;
    }
    // Get Stream Format
    ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
    if (ret < 0) {
        LOG("VIDIOC_G_FMT failed (%d)/n", ret);
        return ret;
    }

    struct v4l2_streamparm param;
    memset(&param,0,sizeof(param));
    param.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    param.parm.capture.timeperframe.numerator=1;
    param.parm.capture.timeperframe.denominator=30;
    param.parm.capture.capturemode = 1;
    ret = ioctl(fd, VIDIOC_S_PARM, &param) ;
    if(ret < 0)  {
        LOG("VIDIOC_S_PARAM failed (%d)/n", ret);
        return ret;
    }
    ret = ioctl(fd, VIDIOC_G_PARM, &param) ;
    if(ret < 0)  {
        LOG("VIDIOC_G_PARAM failed (%d)/n", ret);
        return ret;
    }


    // Print Stream Format
    LOG("Stream Format Informations:/n");
    LOG(" type: %d/n", fmt.type);
    LOG(" width: %d/n", fmt.fmt.pix.width);
    LOG(" height: %d/n", fmt.fmt.pix.height);
    char fmtstr[8];
    memset(fmtstr, 0, 8);
    memcpy(fmtstr, &fmt.fmt.pix.pixelformat, 4);
    LOG(" pixelformat: %s/n", fmtstr);
    LOG(" field: %d/n", fmt.fmt.pix.field);
    LOG(" bytesperline: %d/n", fmt.fmt.pix.bytesperline);
    LOG(" sizeimage: %d/n", fmt.fmt.pix.sizeimage);
    LOG(" colorspace: %d/n", fmt.fmt.pix.colorspace);
    LOG(" priv: %d/n", fmt.fmt.pix.priv);
    LOG(" raw_date: %s/n", fmt.fmt.raw_data);
    // Request buffers
    struct v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.count = BUFFER_COUNT;
    ret = ioctl(fd , VIDIOC_REQBUFS, &reqbuf);
    if(ret < 0) {
        LOG("VIDIOC_REQBUFS failed (%d)/n", ret);
        return ret;
    }
    // Queen buffers

    for(i=0; i<BUFFER_COUNT; i++) {
        // Query buffer
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(fd , VIDIOC_QUERYBUF, &buf);
        if(ret < 0) {
            LOG("VIDIOC_QUERYBUF (%d) failed (%d)/n", i, ret);
            return ret;
        }
        // mmap buffer
        framebuf[i].length = buf.length;
        framebuf[i].start = (char *) mmap(0, buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (framebuf[i].start == MAP_FAILED) {
            LOG("mmap (%d) failed: %s/n", i, strerror(errno));
            return -1;
        }
        // Queen buffer
        ret = ioctl(fd , VIDIOC_QBUF, &buf);
        if (ret < 0) {
            LOG("VIDIOC_QBUF (%d) failed (%d)/n", i, ret);
            return -1;
        }
        printf("my %d \n",i);
        LOG("Frame buffer %d: address=0x%x, length=%d/n", i, (unsigned long)(framebuf[i].start), framebuf[i].length);
    }
    // Stream On
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret < 0) {
        LOG("VIDIOC_STREAMON failed (%d)/n", ret);
        return ret;
    }
    return 1;

}
int video::get_data()
{
    int ret;
    // Get frame
    ret = ioctl(fd, VIDIOC_DQBUF, &buf);
    if (ret < 0) {
        LOG("VIDIOC_DQBUF failed (%d)/n", ret);
        return ret;
    }
    // Re-queen buffer
    ret = ioctl(fd, VIDIOC_QBUF, &buf);
    if (ret < 0) {
        LOG("VIDIOC_QBUF failed (%d)/n", ret);
        return ret;
    }

    sign3 = false;
}

void video::release_video()
{
    // Release the resource
    for (int i=0; i<BUFFER_COUNT; i++) {
        munmap(framebuf[i].start, framebuf[i].length);
    }
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
    LOG("Camera test Done./n");
}

int video::yuvtorgbO( unsigned char *rgb, unsigned int width,unsigned int height)
{
     unsigned char *yuv = (unsigned char *)(framebuf[buf.index].start);
     unsigned int in, out;
     int y0, u, y1, v;
     unsigned int pixel24;
     unsigned char *pixel = (unsigned char *)&pixel24;
     unsigned int size = width*height*2;
     for(in = 0, out = 0; in < size; in += 4, out += 6)
     {
          y0 = yuv[in+0];
          u  = yuv[in+1];
          y1 = yuv[in+2];
          v  = yuv[in+3];

          sign3 = true;
          pixel24 = yuvtorgb(y0, u, v);
          rgb[out+0] = pixel[0];    //for QT
          rgb[out+1] = pixel[1];
          rgb[out+2] = pixel[2];
          //rgb[out+0] = pixel[2];  //for iplimage
          //rgb[out+1] = pixel[1];
          //rgb[out+2] = pixel[0];

          //sign3 = true;
          pixel24 = yuvtorgb(y1, u, v);
          rgb[out+3] = pixel[0];
          rgb[out+4] = pixel[1];
          rgb[out+5] = pixel[2];
          //rgb[out+3] = pixel[2];
          //rgb[out+4] = pixel[1];
          //rgb[out+5] = pixel[0];
     }
     return 0;
}

int video::yuvtorgb(int y, int u, int v)
{
     unsigned int pixel24 = 0;
     unsigned char *pixel = (unsigned char *)&pixel24;
     int r, g, b;
     static long int ruv, guv, buv;

     if(sign3)
     {
         sign3 = false;
         ruv = 1159*(v-128);
         guv = 380*(u-128) + 813*(v-128);
         buv = 2018*(u-128);
     }

     r = (1164*(y-16) + ruv) / 1000;
     g = (1164*(y-16) - guv) / 1000;
     b = (1164*(y-16) + buv) / 1000;

     if(r > 255) r = 255;
     if(g > 255) g = 255;
     if(b > 255) b = 255;
     if(r < 0) r = 0;
     if(g < 0) g = 0;
     if(b < 0) b = 0;

     pixel[0] = r;
     pixel[1] = g;
     pixel[2] = b;

     return pixel24;
}

