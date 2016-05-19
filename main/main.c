#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "v4l2.h"


int main()
{
    int fd;

    v4l2_use.open();

    v4l2_use.read();

    v4l2_use.close();
    close(fd);
    printf("Hello World\n");
	
    return 0;
}
