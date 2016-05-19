/*
 * This file is used to capture the picture or video via V4L2.
 * Create by Buddy Zhang
 */
#include "v4l2.h"

/* Global value */
static char *dev_name = "/dev/video0";
const char *picture_path = "./b0.JPEG";
static unsigned int n_buffers = 0;
struct buffer *buffers = NULL;
static int fd = -1;
FILE *file_fd = NULL;

/*
 * Read frame
 */
static int read_frame()
{
    struct v4l2_buffer buf;
    int count = 5;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

#if V4L2_CAPTURE_PICTURE
    if(ioctl(fd , VIDIOC_DQBUF , &buf) < 0)
#else
    if(ioctl(fd , VIDIOC_STREAMON , &buf) < 0)
#endif
	mm_err();

    fwrite(buffers[buf.index].start , buffers[buf.index].length , 1, file_fd);

#if V4L2_CAPTURE_PICTURE
    if(ioctl(fd, VIDIOC_QBUF , &buf) < 0)
#else
    if(ioctl(fd, VIDIOC_STREAMOFF , &buf) < 0)
#endif
	mm_err();

    return 1;
}

/*
 * Open video
 */
static int v4l2_open(void)
{
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;
    enum v4l2_buf_type type;
    unsigned i;
    int j,m;

    if((fd = open(dev_name , O_RDWR | O_NONBLOCK , 0)) < 0)
	mm_err();

    /* Get the camera params */
    if(ioctl(fd , VIDIOC_QUERYCAP , &cap) < 0)
	mm_err();

    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width  = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    /* Set picture format */
    if(ioctl(fd , VIDIOC_S_FMT , &fmt) < 0)
	mm_err();

    CLEAR(req);
    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd , VIDIOC_REQBUFS , &req) < 0)
    	mm_err();
    if(req.count < 2)
	mm_err();
    buffers = calloc(req.count , sizeof(*buffers));
    for(n_buffers = 0 ; n_buffers < req.count ; ++n_buffers) {
	struct v4l2_buffer buf;
		 
	CLEAR(buf);
	buf.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory    = V4L2_MEMORY_MMAP;
	buf.index     = n_buffers;
	if(ioctl(fd , VIDIOC_QUERYBUF , &buf) == -1)
		mm_err();
	buffers[n_buffers].length = buf.length;
	buffers[n_buffers].start = mmap(NULL , buf.length , PROT_READ | PROT_WRITE,
			    MAP_SHARED , fd , buf.m.offset);
	if(MAP_FAILED == buffers[n_buffers].start)
	    mm_err();
    }

    for(i = 0 ; i < n_buffers ; i++) {
	struct v4l2_buffer buf;

	CLEAR(buf);
	buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory  = V4L2_MEMORY_MMAP;
	buf.index   = i;
	if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)
	    mm_err();
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd , VIDIOC_STREAMON , &type)) {
	mm_err();
	close(fd);
	exit(-1);
    }

    return 1;
}

/*
 * Capture the picture or video.
 */
static int v4l2_capture(void)
{
    enum v4l2_buf_type type;
    fd_set fds;
    struct timeval tv;
    int r;

    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    /* Timeout */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    r = select(fd + 1 , &fds , NULL , NULL , &tv);

    if(r == -1) 
	mm_err();
    else if(r == 0)
	mm_err();

    printf("N_buffers=%d\n",n_buffers);

    read_frame();
}

/*
 * Open device.
 */
static void v4l2_init(void)
{
    file_fd = fopen(picture_path , "w");
    if(v4l2_open() < 0)
	mm_err();
}

/*
 * Close device.
 */
void v4l2_close(void)
{
    int i = 0;
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd , VIDIOC_STREAMOFF , &type))
	printf("VIDIOC_STREAMOFF\n");
unmap:
    for(i = 0 ; i < n_buffers ; ++i) {
	if(munmap(buffers[i].start , buffers[i].length))
	    mm_err();
	    printf("Free buffer\n");
    }
	
    fclose(file_fd);
    close(fd);
}

/*
 * Initialzie the date structure.
 */
struct v4l2 v4l2_use = {
    .open  = v4l2_init,
    .read  = v4l2_capture,
    .close = v4l2_close,
};
