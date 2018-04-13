#ifndef __JIECANG_COMM_FRAME_H
#define __JIECANG_COMM_FRAME_H
#include "stdint.h"
#include <string.h>
const static struct{
  const uint8_t Up;
  const uint8_t Down ;
  const uint8_t Status;
  const uint8_t Stop;
  const uint8_t GetMinMax; 
}JiecangFunction = {
    0x01,0x02,0x07,0x0A,0x0C};
static const struct {
  const uint8_t RxStartByte1;
  const uint8_t RxStartByte2;
  const uint8_t StopByte;
  const uint8_t TxStartByte1;
  const uint8_t TxStartByte2;
  const uint8_t maxRxFrameLength;
  const uint8_t txFrameLength;
  
}JiecangFrameHeader = {0xF2, 0xF2, 0x7E, 0xF1, 0xF1,10,6};
typedef enum {
    STATUS_CURRENT_HEIGHT = 0x01,
    STATUS_ERROR = 0x02,
    STATUS_RESET = 0x04,
    STATUS_MIN_MAX_HEIGHT = 0x07
} JiecangFrameStatus_Typedef;
typedef struct {
  uint8_t StartByte1;
  uint8_t StartByte2;
  uint8_t Function;
  uint8_t Length;
  uint16_t Data1;
  uint16_t Data2;
  uint8_t Checksum;
  uint8_t StopByte;
} JiecangRxFrame_Typedef;
typedef struct {
    uint8_t StartByte1;
    uint8_t StartByte2;
    uint8_t Function;
    uint8_t Data;
    uint8_t Checksum;
    uint8_t EndByte;    
}JiecangTxFrame_Typedef;


/*For Rx Frame*/
void JiecangRxFrame_array2struct(uint8_t *data, uint16_t length, JiecangRxFrame_Typedef *frameObj);
void JiecangRxFrame_struct2array(JiecangRxFrame_Typedef *frameObj,uint8_t *data);
uint8_t JiecangRxFrame_isValidArray(uint8_t *data, uint16_t* length);
uint16_t JiecangRxFrame_getDataFromStruct(JiecangRxFrame_Typedef *frameObj);
JiecangFrameStatus_Typedef JiecangRxFrame_getStatusFromStruct(JiecangRxFrame_Typedef *frameObj);
/*For Tx Frame*/
void JiecangTxFrame_struct2array(JiecangTxFrame_Typedef *frameObj, uint8_t *data);
void JiecangTxFrame_updateChecksum(JiecangTxFrame_Typedef *frameObj);



#endif