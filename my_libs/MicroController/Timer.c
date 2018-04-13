#include "MicroController/Timer.h"
void TIMER_cmdUp(TimerTypedef * _timerObj, uint32_t _val){
    if(_timerObj->status == TIMER_ON){
       _timerObj->timerVal += _val;
        if(_timerObj->timerVal >= _timerObj->timeoutPeriod ){
            if(_timerObj->timerOutCallback != 0){
                _timerObj->timerOutCallback();
            }
        } 
    }
    
}
void TIMER_init(TimerTypedef * _timerObj,void (*_timeOutCallback)(void), uint32_t _timeout){
    _timerObj->timerVal = 0;
    _timerObj->timeoutPeriod = _timeout;
    _timerObj->timerOutCallback = _timeOutCallback;
    _timerObj->status = TIMER_OFF;
}
void TIMER_start(TimerTypedef * _timerObj){
     if (_timerObj->status == TIMER_OFF){
        _timerObj->status = TIMER_ON;
     }
     
}
void TIMER_stop(TimerTypedef * _timerObj){
     _timerObj->status = TIMER_OFF;
}
uint32_t TIMER_getVal(TimerTypedef * _timerObj){
    return _timerObj->timerVal;
}
void TIMER_setVal(TimerTypedef * _timerObj, uint32_t _val){
    _timerObj->timerVal = _val; 

}
TIMER_STATUS TIMER_GetStatus(TimerTypedef * _timerObj){
  return _timerObj->status;
}
