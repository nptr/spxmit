#include "sp_gpio.h"

#include <linux/gpio.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <stdint.h>
#include <stdio.h>

int spxmit_io_request(const char* dev, int pin) 
{
    int fd = open(dev, O_RDWR | O_CLOEXEC);
    if(fd == -1)
    {
        return -1;
    }

    struct gpiohandle_request request;
    memset(&request, 0, sizeof(request));

    request.lineoffsets[0] = pin;
    request.lines = 1;
    request.flags = GPIOHANDLE_REQUEST_OUTPUT;

    int ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &request);
    if(ret < 0)
    {
        fprintf(stderr, "GPIO_GET_LINEHANDLE_IOCTL failed!: %d, %s\n", ret, strerror(errno));
    }
    
    close(fd);
    return ret >= 0? request.fd : -2;
}


int spxmit_io_setpin(int fd, int state)
{
    struct gpiohandle_data value;
    value.values[0] = state;

    int ret = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &value);
    if(ret < 0)
    {
        fprintf(stderr, "GPIOHANDLE_SET_LINE_VALUES_IOCTL failed!: %d, %s\n", ret, strerror(errno));
    }

    return ret;
}


void spxmit_io_close(int fd)
{
    close(fd);
}
