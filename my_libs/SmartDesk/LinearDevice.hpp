/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LinearDevice.hpp
 * Author: thanh
 *
 * Created on March 31, 2016, 12:13 AM
 */

#ifndef LINEARDEVICE_HPP
#define LINEARDEVICE_HPP
#include <inttypes.h>
#include<pthread.h>
#include<vector>
#include<iostream>

class LinearDevice{
    pthread_t controlThread;
    char status;
public:
    
    long int timeOut;
    int (*controlThreadFcn)(void *, void *);
    char Enable(){
        this->status = 1;
        return this->status;
    }
    char Disable(){
        this->status = 0;
        return this->status;
    }
    struct timespec tim,tim2;
    LinearDevice(){
        this->status = 0;    
        this->tim.tv_sec = 0;
        this->tim.tv_nsec = 100000000L; //100ms
        
    }
    virtual ~LinearDevice(){
    
    }
    void SetControlThreadFcn(int(*fcn)(void*, void*) ){
        this->controlThreadFcn = fcn;
        
    }
    char isReady(){
        return this->status;
    }
    virtual void Start() = 0;
    virtual void Stop() = 0;


    void SetEnable(char enable);
    virtual int setupThread(void*(*fcn)(void*),void *data){
        return pthread_create(&this->controlThread,NULL,fcn,data);
    }
    long int GetTimeOut() const {
        return timeOut;
    }
    void SetTimeOut(long int timeOut) {
        this->timeOut = timeOut;
        this->tim.tv_nsec = timeOut;
    }      
};
#endif /* LINEARDEVICE_HPP */

