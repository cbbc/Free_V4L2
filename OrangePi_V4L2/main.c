#include <stdio.h>
#include <stdlib.h>

#include "OrangePi_Internal.h"
#include "v4l2.h"

extern struct OrangePi_v4l2_device OrangePi;
FILE *Picture_fd;
#define PICTURE_PATH "./OP.JPEG"

/*
 * Initialize all
 */
struct OrangePi_v4l2_device *OrangePi_init(void)
{
    /* Initialize the output stream */
    Picture_fd = fopen(PICTURE_PATH,"w");
    if(Picture_fd == NULL) {
	printf("ERROR:Can't open the path\n");
	return NULL;
    }

    /* Prepare the v4l2 device */
    OrangePi.init(&OrangePi);
    OrangePi.buffers->Out_fd = Picture_fd;
    return &OrangePi;   
}

/*
 * Close All
 */
void OrangePi_close(struct OrangePi_v4l2_device *dev)
{
    fclose(dev->buffers->Out_fd);
}

int main(int argc,char *argv[])
{
    struct OrangePi_v4l2_device *dev;
   
    dev = OrangePi_init();
    dev->capture(dev);
    printf("Hello World\n");
    OrangePi_close(dev);
    return 0;
}
