/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Timer.hpp
 * Author: thanh
 *
 * Created on April 4, 2016, 2:27 AM
 */

#ifndef __TIMER_H124
#define __TIMER_H124
#include<inttypes.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<assert.h>
#include<string>
#include<unistd.h>
#include<cstring>
#include <stddef.h>
void *doTimerThread(void *local_class_p);
int TimerCallbackFcn(void *local_class_p, void *target_class_p);
class Timer{
public:
	Timer();
	Timer(__syscall_slong_t ns_timeOut);
    //friend void* doTimerThread(void* data);
    bool timer_triggered = false;
    __syscall_slong_t timeOut;
    int(*timerCallbackFcn)(void *, void *);
    bool enable;
    struct timespec tim,tim2;
    pthread_t timerThread;
    
    void SetTimerCallbackFcn(int(*timerCallbackFcn)(void*, void *),void * target_class_p );
    uint8_t status;
    uint64_t  timerVal;
    uint64_t  clockTime;
    uint64_t  timerCount;
    void *class_data;   
    void Reset();
    void Increase();
    void SetClockTime(__syscall_slong_t _clockTime);
    void SetPeriodTime(uint64_t periodTime);
    void setTimeout(__syscall_slong_t timeOut);
    virtual ~Timer();
    
    uint8_t IsTimerTimout();
    char GetEnable() const;
    void SetEnable(char enable);
    long int GetTimeOut() const;
    void start();
    void stop();
    void init();
    void Start();
    void Stop();
    void Enable();
    void Disable();
    uint8_t IsReady();
    void Init(__syscall_slong_t ns_timeOut);
    /*
     * ns_timeOut: sleep time in Timer thread
     * timeOut: timeout value in ms
     * data : data to pass to thread callback
     */
    void Init(__syscall_slong_t ns_timeOut, uint64_t pll, void *target_class_p);
    
    /*
     * ns_timeOut: sleep time in Timer thread
     * timeOut: timeout value in ms
     * threadfcn : thread handler
     * CallbackFcn: thread callback
     * data: data pass to thread callback
     */
    
//    void Init(__syscall_slong_t ns_timeOut, uint64_t timeOut,
//    void*(*Threadfcn)(void *),int(*CallbackFcn)(void *, void *) , void *target_class_p);
    
    
    void Init(__syscall_slong_t ns_timeOut, uint8_t pll,
        int(*CallbackFcn)(void *, void *) , void *target_class_p );
private:
    void setupThreadFcn(void*(*fcn)(void *),void *local_class_p);

};


#endif /* TIMER_HPP */

