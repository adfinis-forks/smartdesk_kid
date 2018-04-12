/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyGpio.hpp
 * Author: thanh
 *
 * Created on March 12, 2016, 10:58 AM
 */

#ifndef MYGPIO_HPP
#define MYGPIO_HPP
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdarg>
#include <inttypes.h>
#include<assert.h>
#include <string.h>
//#include "gpio.h"

typedef struct {
    unsigned int  gpioPin;
    unsigned int  interruptMode;
    int (*callbackFcn)(void *, void *);
}GPIO_IntStruct;

void *doGpioThread(void *local_class);
int gpioIntFcn(void *local_class, void * target_class);

class GpioInput{
    
    unsigned int currentVal;
    unsigned int preValue;
    
public:  
    static const unsigned int MODE_INPUT = 0;
    static const unsigned int MODE_OUTPUT = 1;
    static const unsigned int EDGE_FALLING = 0;
    static const unsigned int EDGE_RISSING = 1;
    static const unsigned int EDGE_ONCHANGE = 2;
    static const unsigned int STATUS_OFF = 0;
    static const unsigned int STATUS_ON = 1;
    static const unsigned int LOW = 0;
    static const unsigned int HIGH = 1;
    GpioInput();
    ~GpioInput();
    int (*callbackFcn)(void *, void *);
    int digitalRead();
    int clean();
    
    void setPin(unsigned int pin);
    int setInterrupType(unsigned int type);
    int setGpioCallbackFcn(int (*fcn)(void *, void *), void * target_class);
    
    
    /*
     * fcn: function to be called in GPIO thread
     * data: data that pass to the "fcn" when it's called
     */
    int Init(GPIO_IntStruct *initStruct, void * target_lass);
    int interrupt_type;
    bool IsReady();
    int Start();
    int Stop();
    void *target_class;
private:
    bool status;
    int gpioPin;
    int fd;
    int in_out_mode;
    pthread_t gpio_thread;  
    char buf[128];
    char value;
    int setThreadFcn(void*(*fcn)(void *),void *data);    
    unsigned int readPin(unsigned int pin);
    
    
};
class GpioOutput{
public:
    GpioOutput(uint16_t pin){
        this->init(pin);
        
    };
    GpioOutput(){
        
    };
    virtual ~GpioOutput() {
        close(this->fd);
    };
    void setHigh(){
        assert(write(fd, "1", 1)> -1);
    }
    
    void setLow(){
        assert(write(fd, "0", 1) > -1); 
    }
void init(uint16_t pin){
        // 1.export  the GPIO:
        this->pin = pin;
         
        int gpio = pin;

        fd = open("/sys/class/gpio/export", O_WRONLY);
        assert(fd >= 0);

        sprintf(buf, "%d", gpio); 

        write(fd, buf, strlen(buf));

        close(fd);
        
        //2. Set the direction in the GPIO folder just created:
        sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
        fd = open(buf, O_WRONLY);
        assert(fd >= 0);
        
        // Set out direction
        assert (write(fd, "out", 3) > -1); 
        close(fd);
        
        
        
        
        
        // 3.setup the pin as out put
        sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
        this->fd = open(buf, O_WRONLY);
        assert(fd > -1);
        // Set GPIO high status
        assert(write(fd, "1", 1)> -1); 
        
    }
private:
    uint16_t pin;
    int fd;
    char buf[128];
    
    
};

#endif /* MYGPIO_HPP */

