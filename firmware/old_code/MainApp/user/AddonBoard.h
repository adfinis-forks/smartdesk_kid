#ifndef __ADDON_BOARD_H
#define __ADDON_BOARD_H
#include "Speaker.h"
#include "TimHandset.h"
#include "Debug.h"  
#include "RPI.h" 
#include "BoxController.h" 
#include "MicroController/Timer.h"
#include "stm32f10x_iwdg.h"
#include "main.h"
/*
   Cau truc du lieu quan ly thoi gian cho cac state 
   timePeriod: Chu ky thoi gian update state
   timePeriodCount : bien dem cong don thoi gian sau moi ngat timer
   timeOut: Khoan thoi gian toi da ma 1 STATE co the ton tai, bien nay de chu dong thoat khoi state de tranh bi treo trong 1 STATE,
            hoac de thoat cac state ma ko co dieu kien thoat ro rang
   timeOutCount: Bien dem cong don thoi gian cho den khi vuot qua thoi gian timeout thi thoat state:    timeOutCount +=  timePeriod

*/
typedef struct{
   uint16_t timePeriod; 
   uint16_t timePeriodCount;
   uint16_t timeOut; 
   uint16_t timeOutCount;

}STATE_TIME_Handle_Typedef;

/*
    Handle chu tat ca cac data cua state.

*/
typedef struct {
    STATE_TIME_Handle_Typedef FREE_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef UP_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef DOWN_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef STOP_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef GET_HEIGHT_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef SET_HEIGHT_STATE_TimeStruct;
    STATE_TIME_Handle_Typedef SAVE_HEIGHT_STATE_TimeStruct;
   
}ADDON_BOARD_Handle_Typedef;

typedef enum {
    MIC_ON,
    MIC_OFF

}MIC_Status_Typedef;
typedef enum {
    SPEAKER_ON,
    SPEAKER_OFF

}SPEAKER_Status_Typedef;
/*
typedef enum {
    STATE_FREE = 0,
    STATE_UP,
    STATE_DOWN,
    STATE_STOP,
    STATE_GET_HEIGHT,
    STATE_SET_HEIGHT,
    STATE_SAVE_HEIGHT,
    STATE_SAVE2POS1,
    STATE_SAVE2POS2,
    STATE_SAVE2POS3,
    STATE_SAVE2POS4,
    STATE_MOVE2POS1,
    STATE_MOVE2POS2,
    STATE_MOVE2POS3,
    STATE_MOVE2POS4,
    STATE_SLEEP,
    STATE_FORGE_RESET,
    STATE_ERROR,
    STATE_SET_MIC,
    STATE_GET_MIC,
    STATE_SET_SPEAKER, // Nhan command turn on/off speaker tu board raspberry hay debug serial
    STATE_GET_SPEAKER  // Nhan request turn                                                                         
}BOARD_State_Typedef;
*/
void ADDON_BOARD_Init(void);
void ADDON_BOARD_Run(void);
void ADDON_BOARD_Update(void);
static struct {
  void (*Init)(void);
  void (*Run)(void);
  void (*Update)(void);
}AddonBoard
= {ADDON_BOARD_Init,ADDON_BOARD_Run,ADDON_BOARD_Update};
#endif


