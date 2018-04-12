#include "Device/TimboxCommFrame.h"
void  _TIM_Rxarray2struct(uint8_t * array, uint16_t length,  TimotionRxStruct_Typedef *rxFrame){
  rxFrame->StartByte1 = array[0];
  rxFrame->StartByte2 = array[1];
  rxFrame->Param1 = array[2];
  rxFrame->Param2 = array[3];
  rxFrame->Data1 = array[4];
  rxFrame->Data2 = array[5];
    
}

void  _TIM_Txarray2struct(uint8_t * array, uint16_t length,  TimotionTxStruct_Typedef *txFrame){

}
uint8_t _TIM_isValidRxData(uint8_t *arrayData, uint16_t length){
  if (length < 6)
  {
    return 0;
  }
  else 
  {
    uint8_t res = 0;
    if ((arrayData[0] != TimboxFrameHeader.RxStartByte1)
        || (arrayData[1] != TimboxFrameHeader.RxStartByte2)
        ||(arrayData[4] != arrayData[5])
        || (arrayData[2] != arrayData[3]))
    {
      res = 0;
    }
    else
    {
      res = 1;
    }
    return res;  
  }
}

uint8_t _TIM_isValidTxData(uint8_t *arrayData, uint16_t length){
    return 0;
}

uint8_t _TIM_getDataFromRxStruct(TimotionRxStruct_Typedef *rxFrame){
  return rxFrame->Data1;
}
uint8_t _TIM_GetParamFromRxStruct(TimotionRxStruct_Typedef *rxFrame){
    return rxFrame->Param1;
}
#ifndef USE_IAR
const  struct {
  void (*RxArray2Struct)(uint8_t *, uint16_t, TimotionRxStruct_Typedef *);
  void (*TxArray2Struct)(uint8_t *, uint16_t, TimotionTxStruct_Typedef *);
  uint8_t (*isValidRxData)(uint8_t *, uint16_t);
  uint8_t (*isValidTxData)(uint8_t *, uint16_t);
  uint8_t (*GetRxData)(TimotionRxStruct_Typedef *);
}TimCommFrameCmd ={
 &_TIM_Rxarray2struct,
 &_TIM_Txarray2struct,
 &_TIM_isValidRxData,
 &_TIM_isValidTxData,
  &_TIM_getDataFromRxStruct
 
};
#endif