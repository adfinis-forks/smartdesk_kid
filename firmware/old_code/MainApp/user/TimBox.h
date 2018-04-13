#ifndef __TIMBOX_H
#define __TIMBOX_H
#include "stm32f10x.h"
#include "MicroController/MyUart.h"
#include "main.h"
#include "MicroController/MyFlash.h"
#include "RPI.h"
#include "Devices/TimboxCommFrame.h"
typedef struct {
  TimotionRxStruct_Typedef *rxFrameObj;
  TimotionTxStruct_Typedef *txFrameObj;
  uint8_t       *dmaBuff_ptr;
  uint16_t      dmaBuffLength;
  uint8_t       *rxBuf_ptr;
  uint16_t      rxBufLength;
  uint16_t      bufferIndex;
}TimotionHandle_Typedef;
typedef enum {
    TIMOTION_STATE_FREE = 0,
    TIMOTION_STATE_UP,
    TIMOTION_STATE_DOWN,
// STATES FOR TIMOTION HANDSET
    TIMOTION_STATE_HANDSET_UP,
    TIMOTION_STATE_HANDSET_DOWN,
    TIMOTION_STATE_HANDSET_STOP,
    TIMOTION_STATE_HANDSET_RESET,
    TIMOTION_STATE_HANDSET_SAVE,
    TIMOTION_STATE_HANDSET_SAVE2POS1,
    TIMOTION_STATE_HANDSET_SAVE2POS2,
    TIMOTION_STATE_HANDSET_SAVE2POS3,
    TIMOTION_STATE_HANDSET_SAVE2POS4,
    TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO,
    TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO,
    TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO,
    TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO,
    TIMOTION_STATE_HANDSET_MOVE2POS1,
    TIMOTION_STATE_HANDSET_MOVE2POS2,
    TIMOTION_STATE_HANDSET_MOVE2POS3,
    TIMOTION_STATE_HANDSET_MOVE2POS4,
    TIMOTION_STATE_HANDSET_MIC,
    TIMOTION_STATE_HANDSET_SPEAKER_ON,
    TIMOTION_STATE_HANDSET_SPEAKER_OFF,
    TIMOTION_STATE_HANDSET_TIME_OUT,
    
// STATES FOR MIC-BUTTON-LED Modules
    
    TIMOTION_STATE_MBL_UP,
    TIMOTION_STATE_MBL_DOWN,
    TIMOTION_STATE_MBL_STOP,
    TIMOTION_STATE_MBL_RESET,
    TIMOTION_STATE_MBL_SAVE_SIT,
    TIMOTION_STATE_MBL_SAVE_STAND,    
    TIMOTION_STATE_MBL_SIT,
    TIMOTION_STATE_MBL_STAND,    
    TIMOTION_STATE_MBL_MIC,
    TIMOTION_STATE_MBL_SPEAKER_ON,
    TIMOTION_STATE_MBL_SPEAKER_OFF,
    TIMOTION_STATE_MBL_TIME_OUT,
// States for RPI
    TIMOTION_STATE_RPI_UP,
    TIMOTION_STATE_RPI_DOWN,
    TIMOTION_STATE_RPI_STOP,
    TIMOTION_STATE_RPI_RESET,    
    TIMOTION_STATE_RPI_MOVE_POS1,
    TIMOTION_STATE_RPI_MOVE_POS2,
    TIMOTION_STATE_RPI_MOVE_POS3,
    TIMOTION_STATE_RPI_MOVE_POS4,
    TIMOTION_STATE_RPI_SAVE_POS1,
    TIMOTION_STATE_RPI_SAVE_POS2,
    TIMOTION_STATE_RPI_SAVE_POS3,
    TIMOTION_STATE_RPI_SAVE_POS4,
    TIMOTION_STATE_RPI_MIC,
    TIMOTION_STATE_RPI_SPEAKER_ON,
    TIMOTION_STATE_RPI_SPEAKER_OFF,
    TIMOTION_STATE_RPI_TIME_OUT
// Common state

}TIMOTION_STATE_Typedef;

#ifdef __cplusplus
extern "C" {
#endif
/*
  
*/
uint8_t TIM_BOX_Init(void);
void TIM_BOX_Update(void);
void TIM_BOX_Run(void);

// Callbacks
void TIMOTION_DmaCallback(DMA_RX_FLAG_Typedef flag); // copy data tu dma buffer vao long buffer


// Raw APIs
void  TIM_BOX_UpApi(void);
void  TIM_BOX_DownApi(void);
void  TIM_BOX_StopApi(void);
void  TIM_BOX_ResetApi(void);	
void  TIM_BOX_MovePos1Api(void);
void  TIM_BOX_MovePos2Api(void);
void  TIM_BOX_MovePos3Api(void);
void  TIM_BOX_MovePos4Api(void);
void  TIM_BOX_SaveApi(void);
void  TIM_BOX_SaveSitPos(uint16_t height_mm);
void  TIM_BOX_SaveStandPos(uint16_t height_mm);
void  TIM_BOX_SleepOn(void);
void  TIM_BOX_SleepOff(void);

//APIs 
WorkStatus_Typedef TIM_BOX_GetControllerState(void);
void TIM_BOX_PutBytes(uint8_t * _data, uint8_t length);
void TIM_BOX_SetHeight(uint16_t height_mm);
uint16_t TIM_BOX_GetHeight(void);
void TIM_BOX_SetState(TIMOTION_STATE_Typedef _state);
TIMOTION_STATE_Typedef TIM_BOX_GetCurState(void);
TIMOTION_STATE_Typedef TIM_BOX_GetPreState(void);
void TIM_BOX_SetMode(OPERATE_MODE_Typedef _mode);
uint8_t TIM_BOX_GetDetectStatus(void);
uint16_t TIM_BOX_ReadSitPosHeight(void);
uint16_t TIM_BOX_ReadStandPosHeight(void);
uint8_t TIMBOX_GetPacketStatus(void);
/*

*/
const static struct {
    void (*Up)(void);                               //1
    void (*Down)(void);                             //2
    void (*Stop)(void);                             //3
    void (*Reset)(void);                            //4
    void (*MovePos1)(void);                         //5
    void (*MovePos2)(void);                         //6
    void (*MovePos3)(void);                         //7
    void (*MovePos4)(void);                         //8
    void (*Save)(void);                             //9
    void (*EnableSleep)(void);                      //10
    void (*DisableSleep)(void);                     //11
    WorkStatus_Typedef (*GetControllerStatus)(void);//12
    void (*SetSetpointHeight)(uint16_t);            //13
    uint16_t (*GetCurrentHeight)(void);             //14
    void (*SetState)(TIMOTION_STATE_Typedef);       //15
    TIMOTION_STATE_Typedef (*GetCurrentState)(void);//16
    TIMOTION_STATE_Typedef (*GetPreviousState)(void);//17
    void (*SetOperationMode)(OPERATE_MODE_Typedef);  //18
    uint8_t (*GetConnectStatus)(void);                  //19  
    uint16_t (*GetSitPos)(void);                    //20
    uint16_t (*GetStandPos)(void);                  //21
    uint8_t (*GetPacketStatus)(void);
    
    
}TimBoxCmd = 
{
TIM_BOX_UpApi,                  //1
TIM_BOX_DownApi,                //2
TIM_BOX_StopApi,                //3
TIM_BOX_ResetApi,               //4
TIM_BOX_MovePos1Api,            //5
TIM_BOX_MovePos2Api,            //6
TIM_BOX_MovePos3Api,            //7
TIM_BOX_MovePos4Api,            //8
TIM_BOX_SaveApi,                //9
TIM_BOX_SleepOn,                //10
TIM_BOX_SleepOff,               //11
TIM_BOX_GetControllerState,     //12
TIM_BOX_SetHeight,              //13
TIM_BOX_GetHeight,              //14
TIM_BOX_SetState,               //15
TIM_BOX_GetCurState,            //16
TIM_BOX_GetPreState,            //17
TIM_BOX_SetMode,                 //18
TIM_BOX_GetDetectStatus,         //19
TIM_BOX_ReadSitPosHeight,        //20
TIM_BOX_ReadStandPosHeight,       //21
TIMBOX_GetPacketStatus          //22

} ;


// state timeout callback
void TIM_BOX_checkHeightCallback(void);

#ifdef __cplusplus
}
#endif

#endif
