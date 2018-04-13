#include "Device/JiecangCommFrame.h"
void JiecangRxFrame_array2struct(uint8_t *data, uint16_t length, JiecangRxFrame_Typedef *frameObj){
  
  
  
  frameObj->StartByte1 = data[0];
  frameObj->StartByte2 = data[1];
  frameObj->Function = data[2];
  frameObj->Length = data[3];
  frameObj->Data1 = ((uint16_t)data[4])*256 + (uint16_t)data[5];
  if (length == 9){
    frameObj->Data2 = (uint16_t)data[6];
    frameObj->Checksum = data[7];
    frameObj->StopByte = data[8];
  }
  else if (length == 10){
    frameObj->Data2 = ((uint16_t)data[6]*256) + (uint16_t)data[7];
    frameObj->Checksum = data[8];
    frameObj->StopByte = data[9];
  
  }

}
void JiecangRxFrame_struct2array(JiecangRxFrame_Typedef *frameObj,uint8_t *data){

}
uint8_t JiecangRxFrame_isValidArray(uint8_t *data, uint16_t *length){
  // Check Start Byte1
  if(data[0] != JiecangFrameHeader.RxStartByte1){
    
    return 0;
  }
  // Check Start Byte2
  if(data[1] != JiecangFrameHeader.RxStartByte2 ){
    return 0;
  }
  // Check Stop Byte
  if (data[data[3] + 5] != JiecangFrameHeader.StopByte)
  {
    return 0;
  }
  // Check Checksum
  uint16_t sum = (uint16_t)data[2] + (uint16_t)data[3];// = function + length
  for (int i = 4; i < 4+ data[3];i++){
     sum += (uint16_t)data[i];
  }
  if (sum >= 256){
    sum = sum%256;
  } 
  if (sum != data[(data[3] + 4)])
  {
    return 0;
  }
  *length = data[3] + 6;
  return 1;

}
uint16_t JiecangRxFrame_getDataFromStruct(JiecangRxFrame_Typedef *frameObj){

  return ((uint16_t)frameObj->Data1*256 + (uint16_t)frameObj->Data2);

}

void JiecangTxFrame_struct2array(JiecangTxFrame_Typedef *frameObj, uint8_t *data){

}

void JiecangTxFrame_updateChecksum(JiecangTxFrame_Typedef *frameObj){

}

JiecangFrameStatus_Typedef JiecangRxFrame_getStatusFromStruct(JiecangRxFrame_Typedef *frameObj){
    return (JiecangFrameStatus_Typedef)frameObj->Function;
}