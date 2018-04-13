/*
 * GPIO user space helpers
 *
 * Copyright 2009 Analog Devices Inc.
 * Michael Hennerich (hennerich@blackfin.uclinux.org)
 *
 * Licensed under the GPL-2 or later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

/****************************************************************
 * Constants
 ****************************************************************/
 
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

#define GPIO_DIR_IN     0
#define GPIO_DIR_OUT    1

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio){
    int fd, len;
    char buf[64];

    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (fd < 0) {
            printf("cannot export [%s]\n",buf);
            return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio); 

    write(fd, buf, len); //Ghi gpio <number> vao file export de dang ky no voi user space

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio){
    int fd, len;
    char buf[64]; 

    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (fd < 0) {
            perror("gpio/unexport");
            return fd;
    } 

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len); //Ghi gpio <number> vao file unexport khi muon giai phong

    close(fd);

    return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int dir){
    int fd, len;
    char buf[64]; 

    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio); 
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
            perror("gpio/direction");
            return fd;
    }

    //config input/output by write "in", "out" into file /sys/class/gpio/gpio[ID]/diriection

    if (dir == GPIO_DIR_OUT)
            write(fd, "out", 4);
    else
            write(fd, "in", 3); 

    close(fd);

    return 0;
} 

int gpio_dir_out(unsigned int gpio){
    return gpio_set_dir(gpio, GPIO_DIR_OUT); //Set gpio direction output
} 

int gpio_dir_in(unsigned int gpio){
    return gpio_set_dir(gpio, GPIO_DIR_IN); //Set gpio direction output
} 

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned gpio, unsigned value){
    int fd, len;
    char buf[64]; 
    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio); 

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
            perror("gpio/value");
            return fd;
    }
    //Output value 1 or 0 by write "1" or "0" into file value
    if (value)
            write(fd, "1", 2);
    else
            write(fd, "0", 2); 

    close(fd);
    return 0;
} 
/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value){
    int fd, len;
    char buf[128];
    char ch;
    
    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY); //Mo file de doc
    if(fd<0)
    {
            perror("gpio/value");
            return fd;
    }
    read(fd, &ch, 1);
    if(ch!='0')
            *value = 1;
    else
            *value = 0;
    return 0;  
}
