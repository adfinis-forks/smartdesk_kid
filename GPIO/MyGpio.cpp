/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdbool.h>

#include "GPIO/MyGpio.hpp"
#include "GPIO/gpio.h"
#define BUFFER_MAX 3

int GpioInput::setGpioCallbackFcn(int(*fcn)(void*, void*), void* target_class) {
    this->target_class = target_class;
    this->callbackFcn = fcn;
    return 0;
}

 int GpioInput::setInterrupType(unsigned int type){
     this->interrupt_type = type; 
 }
 int GpioInput::digitalRead(){   
    char ch;
    lseek(fd, 0, SEEK_SET);
    read(fd, &ch, 1);
    if(ch!='0')
        value = 1;
    else
        value = 0;
    return value;
  
 }
GpioInput::GpioInput() {
    this->status = false;
    this->callbackFcn = gpioIntFcn;
    this->setThreadFcn(doGpioThread,this);
    
}
GpioInput::~GpioInput() {
    this->clean();
    printf("deleted gpio\n");
}
int GpioInput::clean(){
//   return  this->gpioUnExport(this->gpioPin);
   printf("Clean GPIO\n");
}


int GpioInput::Stop() {
    this->status = false;
}

int GpioInput::Start() {
    this->status = true;
}

int GpioInput::setThreadFcn(void*(*fcn)(void*), void* data) {
    pthread_create(&this->gpio_thread, NULL, fcn, data);
}

void GpioInput::setPin(unsigned int pin) {
    this->gpioPin = pin;
    int gpio = pin;
    /*1. export the gpio*/  
    fd = open("/sys/class/gpio/export", O_WRONLY);

    if (fd < 0) {
            printf("cannot export [%s]\n",buf);
    }

    int len = snprintf(buf, sizeof(buf), "%d", gpio); 

    write(fd, buf, len); //Ghi gpio <number> vao file export de dang ky no voi user space

    close(fd);
    /*2.Set the direction in the GPIO folder just created*/ 
    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio); 
    fd = open(buf, O_WRONLY);
    if (fd < 0) {
            perror("gpio/direction");
    }
    write(fd, "in", 3); 
    close(fd);
    /*3.get the current value of GPIO:*/  
    
    
    len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_RDONLY); //Mo file de doc
    if(fd < 0)
    {
            perror("gpio/value");
    }
    
    char ch;
    read(fd, &ch, 1);
    if(ch!='0')
        value = 1;
    else
        value = 0;
}


bool GpioInput::IsReady() {
    return this->status;
}




int GpioInput::Init(GPIO_IntStruct* initStruct, void* target_lass) {    
    this->Stop();
    this->setPin(initStruct->gpioPin);
    this->setInterrupType(initStruct->interruptMode);
    this->setGpioCallbackFcn(initStruct->callbackFcn,target_lass);
    
    
}


/*
 * 
 */ void *doGpioThread(void *local_class){
    int pre_val, cur_val;
    int e;
    struct timespec tim,tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 10000000L;//10ms
    GpioInput *x = (GpioInput*)local_class;
    if(x->IsReady() == true){
        pre_val = x->digitalRead();
        printf("val:%d\n",x->digitalRead());
    }
    while(1){
//        printf("Ngat\n");
        nanosleep(&tim , &tim2);     
        if(x->IsReady() == true){
            cur_val = x->digitalRead();
            x->digitalRead();
            //printf("val:%d\n",x->digitalRead());
        }        
        e = cur_val - pre_val;
        if(e != 0){
            switch (x->interrupt_type){
                case GpioInput::EDGE_RISSING:
                if((e > 0)&&(x->callbackFcn != NULL)){
                    x->callbackFcn(local_class, x->target_class);
                } 
                break;
                case GpioInput::EDGE_FALLING:
                if((e < 0)&&(x->callbackFcn != NULL)){
                    x->callbackFcn(local_class, x->target_class);
                } 
                break;
                case GpioInput::EDGE_ONCHANGE:
                if(x->callbackFcn != NULL){
                    x->callbackFcn(local_class, x->target_class);
                }
                
                break;    
            }  
        }
        pre_val = cur_val;
        
    }
    
 }
__attribute__((weak)) int gpioIntFcn(void *local_class, void *target_class){
    GpioInput *x = (GpioInput*)local_class;
    switch(x->interrupt_type){
        case GpioInput::EDGE_RISSING:
            printf("Rissing edge detected\n");
            break;
        case GpioInput::EDGE_FALLING:
            printf("Falling detected\n");
            break;
        case GpioInput::EDGE_ONCHANGE:
            printf("interrupt onchange detected\n");
            break;
    }
}



unsigned int GpioInput::readPin(unsigned int pin) {
    lseek(fd, 0, SEEK_SET);
    assert(read(fd, &value, 1) >= 0);
    return value;
}
/*
 * MyGpio Class
 */
