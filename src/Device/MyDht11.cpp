/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Device/MyDht11.hpp"
__attribute__((weak)) void *doThread(void *data){
    struct timespec tim,tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 900000000L;//900ms
    
    
    
    MyDht11 *x;
    x = (MyDht11*)data;
    while(1){
        printf("Read Sensor Dht11\n");
        nanosleep(&tim , &tim2);     
        if(x->isReady()){
            x->callbackFcn(data);
        }                
    }
    
 }
__attribute__((weak)) int CallbackFcn(void *data){
    MyDht11 *x;
    x = (MyDht11*)data;
    x->readDht();
    
} 