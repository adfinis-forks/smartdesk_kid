#ifndef __TIM_BOX_COMM_FRAME_H
#define __TIM_BOX_COMM_FRAME_H
#ifdef CC2541
#include <hal_types.h>
#else
#include <stdint.h>
#endif


#ifdef __cplusplus
extern "C"
{
#endif

// your functions here for the header
typedef struct {
  uint8_t StartByte1;
  uint8_t StartByte2;
  uint8_t Param1;
  uint8_t Param2;
  uint8_t Data1;
  uint8_t Data2;
}TimotionRxStruct_Typedef;
typedef struct {
  uint8_t StartByte1;
  uint8_t StartByte2;
  uint8_t Param;
  uint8_t Command1;
  uint8_t Command2;
}TimotionTxStruct_Typedef;
static const struct {
  const uint8_t Up;
  const uint8_t Down;
  const uint8_t Stop;
  const uint8_t Pos1;
  const uint8_t Pos2;
  const uint8_t Pos3;
  const uint8_t Pos4;
  const uint8_t Save;
  const uint8_t Reset;
}TimBoxCmds = {0x02, 0x01,0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x03};
static const struct {
  const uint8_t RxStartByte1;
  const uint8_t RxStartByte2;
  const uint8_t TxStartByte1;
  const uint8_t TxStartByte2;
  const uint8_t rxFrameLength;
  const uint8_t txFrameLength;
}TimboxFrameHeader = {
 0x98,
 0x98,
 0xD8,
 0xD8,
 6,
 5
};
static const struct{
    const uint8_t STOPPED;
    const uint8_t SAVE_TRIGGED;
    const uint8_t MOVING;
    
}TimboxStatus={
    0x00,
    0x01,
    0x03
};
typedef enum {
    STOPPED = 0,
    SAVE = 1,
    MOVING=3        
}TimboxFrameStatus;
static const struct {
  const uint8_t DISCONNECTED;
  const uint8_t CONNECTED;
}TimboxConnectStatus = {
  0,1
};


void  _TIM_Rxarray2struct(uint8_t * array, uint16_t length, TimotionRxStruct_Typedef *rxFrame);
void  _TIM_Txarray2struct(uint8_t * array, uint16_t length, TimotionTxStruct_Typedef *txFrame);
uint8_t _TIM_isValidRxData(uint8_t *arrayData, uint16_t length);
uint8_t _TIM_isValidTxData(uint8_t *arrayData, uint16_t length);
uint8_t _TIM_getDataFromRxStruct(TimotionRxStruct_Typedef *rxFrame);
uint8_t _TIM_GetParamFromRxStruct(TimotionRxStruct_Typedef *rxFrame);
#ifdef USE_IAR
static struct {
  void (*RxArray2Struct)(uint8_t *, uint16_t, TimotionRxStruct_Typedef *);
  void (*TxArray2Struct)(uint8_t *, uint16_t, TimotionTxStruct_Typedef *);
  uint8_t (*isValidRxData)(uint8_t *, uint16_t);
  uint8_t (*isValidTxData)(uint8_t *, uint16_t);
  uint8_t (*GetRxData)(TimotionRxStruct_Typedef *);
  uint8_t (*GetRxParam)(TimotionRxStruct_Typedef *);
}TimCommFrameCmd ={
 &_TIM_Rxarray2struct,
 &_TIM_Txarray2struct,
 &_TIM_isValidRxData,
 &_TIM_isValidTxData,
 &_TIM_getDataFromRxStruct,
 &_TIM_GetParamFromRxStruct
 
};
#endif
#ifdef __cplusplus
}
#endif
#endif