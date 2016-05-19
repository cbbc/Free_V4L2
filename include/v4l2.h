#ifndef _V4L2_H_
#define _V4L2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <asm/types.h>

#define WIDTH     160
#define HEIGHT    120
#define IMG_SIZE  (WIDTH * HEIGHT * 3)
#define BITCOUNT  24

#define DEST_WIDTH  320
#define DEST_HEIGHT 240
#define DEST_IMG_SIZE (DEST_WIDTH * DEST_HEIGHT * 3)
#define DEST_BUF_SIZE (((DEST_WIDTH * BITCOUNT + 31) / 32) * 4 * DEST_HEIGHT)

struct buffer 
{
	unsigned long int *start;
	size_t length;
};

struct v4l2 
{
	unsigned char *dest_buffers;
	unsigned char *rgb_buffers;
	FILE *bmp_fd;
	long bmp_size;

	void (*open)(void);
	int (*read)(void);
	int (*write)(char *buf,int size);
	void (*close)(void);
};

#define CLEAR(x) memset(&(x) , 0 ,sizeof(x))

#define mm_err() printf("%s:%d\n",__func__,__LINE__)

extern struct v4l2 v4l2_use;
#endif
