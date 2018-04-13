#ifndef __TIMER_H
#define __TIMER_H
#ifdef USE_STM8
    #include "stm8s.h"

#else
    #include <stdint.h>
#endif
typedef enum{
    TIMER_OFF = 0,
    TIMER_ON
} TIMER_STATUS;
typedef struct mTimer{
    uint32_t  timeoutPeriod;
    uint32_t  timerVal;
    void (*timerOutCallback)(void);
    TIMER_STATUS status;
    
}TimerTypedef;


void TIMER_init(TimerTypedef * _timerObj,void (*_timeOutCallback)(void), uint32_t _timeout) ;
void TIMER_start(TimerTypedef * _timerObj);
void TIMER_stop(TimerTypedef * _timerObj);
void TIMER_cmdUp(TimerTypedef * _timerObj, uint32_t _val);
TIMER_STATUS TIMER_GetStatus(TimerTypedef * _timerObj);
uint32_t TIMER_getVal(TimerTypedef * _timerObj);
void TIMER_setVal(TimerTypedef * _timerObj, uint32_t _val);

#endif
