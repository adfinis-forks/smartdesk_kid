/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"TIMER/Timer.hpp"
#include "Device/DynamixelController.hpp"
char Timer::GetEnable() const {
    return enable;
}

long int Timer::GetTimeOut() const {
    return timeOut;
}

void Timer::SetEnable(char enable) {
    this->enable = enable;

}

void Timer::setTimeout(__syscall_slong_t timeOut) {
    this->timeOut = timeOut;
    this->tim.tv_nsec = timeOut;
    this->clockTime = timeOut;
}

void Timer::SetTimerCallbackFcn(int(*timerCallbackFcn)(void*, void *),
        void * target_class_p ) {
    this->timerCallbackFcn = timerCallbackFcn;
    this->class_data = target_class_p;
}

//void Timer::Init(__syscall_slong_t ns_timeOut, uint64_t timeOut, 
//        void*(*Threadfcn)(void*), int(*CallbackFcn)(void *,void *), void* target_class_p) {
//    this->setTimeout(ns_timeOut);
//    this->SetPeriodTime(timeOut);
//    this->SetTimerCallbackFcn(CallbackFcn,target_class_p);
//    this->setupThreadFcn(Threadfcn, target_class_p);
//
//}

void Timer::Init(__syscall_slong_t ns_timeOut, uint64_t pll, void* target_class_p) {
    this->setTimeout(ns_timeOut);
    this->SetPeriodTime(pll);
    
    this->setupThreadFcn(doTimerThread, this);
    this->SetTimerCallbackFcn(TimerCallbackFcn, target_class_p);

}

void Timer::Init(__syscall_slong_t ns_timeOut) {
    this->setTimeout(ns_timeOut);
    this->SetPeriodTime(1);
    //this->SetTimerCallbackFcn(TimerCallbackFcn,&this);
    this->setupThreadFcn(doTimerThread, this);

}

void Timer::Init(__syscall_slong_t ns_timeOut, uint8_t pll, int(*CallbackFcn)(void*, void*), void* target_class_p) {
    this->setTimeout(ns_timeOut);
    this->SetPeriodTime(pll);
    this->setupThreadFcn(doTimerThread, this);
    this->SetTimerCallbackFcn(CallbackFcn,target_class_p);
    
}
Timer::Timer() {
    printf("Construct Timer\n");
    this->tim.tv_sec = 0;
    this->setTimeout(100000000L);// 100ms
    this->SetEnable(0);
    this->status = 0;
    this->timerVal = 0;
    this->timerCount = 0;
    this->enable = false;
    this->class_data = 0;
}

void Timer::Disable() {
    this->status = 0;
}

Timer::Timer(__syscall_slong_t ns_timeOut) {
    this->Init(ns_timeOut);
}

void Timer::Stop() {
    this->Disable();
}

void Timer::Start() {
    this->Reset();
    this->Enable();

}

void Timer::SetPeriodTime(uint64_t periodTime) {
    this->timerVal = periodTime;
}

void Timer::SetClockTime(__syscall_slong_t _clockTime) {
    this->timeOut = _clockTime;
    this->clockTime = _clockTime;
}

void Timer::Reset() {
    this->timerCount = 0;
}

uint8_t Timer::IsTimerTimout() {
    if (this->timerCount >= this->timerVal) {
        this->timerCount = 0;
        return 1;

    } else {
        return 0;
    }
}

uint8_t Timer::IsReady() {
    return this->status;
}







void Timer::Increase() {
    this->timerCount++;
}

void Timer::Enable() {
    this->status = 1;
}


void Timer::setupThreadFcn(void*(*fcn)(void*) , void* local_class_p) {
    pthread_create(&this->timerThread, 0, fcn, local_class_p);
}

void Timer::start() {
    
    this->SetEnable(1);
}

void Timer::stop() {
    this->SetEnable(0);
}

Timer::~Timer() {
    delete(this);
}

void Timer::init() {
    //this->SetTimerCallbackFcn(TimerCallbackFcn,);
    this->setupThreadFcn(doTimerThread,this);
}

__attribute__((weak)) void* doTimerThread(void* local_class_p) {
    Timer *x = (Timer*)local_class_p;
    while(1){
        nanosleep(&x->tim,&x->tim2);
        if(x->IsReady()){
            x->Increase();

            if (x->IsTimerTimout()){
            
                if(x->timerCallbackFcn != 0){
                    (*x->timerCallbackFcn)(x,x->class_data); 

                }
            }
        }
    };
    return 0;
}
__attribute__((weak)) int TimerCallbackFcn(void * local_class_p, void *target_class_p){
    
    Timer *_timer  = (Timer *)local_class_p;
    _timer->Stop();
    printf("Default Timer1 Fcn\n");
    return 0;
}
