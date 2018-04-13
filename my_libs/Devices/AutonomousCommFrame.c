#include "Devices/AutonomousCommFrame.h"

uint8_t  AutonomousCommFrame_GetStructLength(AutonomousCommFrame_Typedef *frameObj){

    return (sizeof(AutonomousCommFrame_Typedef));
}
uint8_t  AutonomousCommFrame_UpdateStructChecksum(AutonomousCommFrame_Typedef *frameObj){
    uint16_t sum =  (uint16_t)frameObj->LengthOfParams +
                        (uint16_t)frameObj->ProductId +
                        (uint16_t)frameObj->ComponentId +
                        (uint16_t)frameObj->Instruction +
                        (uint16_t)frameObj->Address;
    for (int i = 0; i < frameObj->LengthOfParams; i++){
        sum += (uint16_t)frameObj->Params[i];
    }
    if (sum > 255) sum %= 255;
    frameObj->CheckSum = (uint8_t)sum;

    return frameObj->CheckSum;
}

uint8_t  AutonomousCommFrame_Struct2Array(AutonomousCommFrame_Typedef *frameObj,uint8_t *arrayData ){
    uint8_t len = AutonomousCommFrame_GetStructLength(frameObj);
  if (len < 12){
    printf("Length error %d\n",(int)len);
  }
  else{
    arrayData[0] = frameObj->StartByte1;
    arrayData[1] = frameObj->StartByte2;
    arrayData[2] = frameObj->LengthOfParams;
    arrayData[3] = frameObj->ProductId;
    arrayData[4] = frameObj->ComponentId;
    arrayData[5] = frameObj->Instruction;
    arrayData[6] = frameObj->Address;
    arrayData[7] = frameObj->Params[0];
    arrayData[8] = frameObj->Params[1];
    arrayData[9] = AutonomousCommFrame_UpdateStructChecksum(frameObj);
    arrayData[10] = frameObj->EndByte1;
    arrayData[11] = frameObj->EndByte2;
  }

return len;

}

uint8_t AutonomousCommFrame_Array2Struct(uint8_t *arrayData ,uint16_t arrayLength, AutonomousCommFrame_Typedef *frameObj){
    if (arrayLength < 12){
        return 0;
    }
    else {
        frameObj->StartByte1 = arrayData[0];
        frameObj->StartByte2 = arrayData[1];
        frameObj->LengthOfParams = arrayData[2];
        frameObj->ProductId = arrayData[3];
        frameObj->ComponentId = arrayData[4];
        frameObj->Instruction = arrayData[5];
        frameObj->Address = arrayData[6];
        frameObj->Params[0] = arrayData[7];
        frameObj->Params[1] = arrayData[8];
        frameObj->CheckSum = arrayData[9];
        frameObj->EndByte1 = arrayData[10];
        frameObj->EndByte2 = arrayData[11];


    }

    return frameObj->LengthOfParams;
}
uint8_t AutonomousCommFrame_IsValidPacket(uint8_t *arrayData, uint8_t length){
  //start bytes, stop bytes checking
  if (  (arrayData[0] != AutonomousFrame.StartByte1)
      ||(arrayData[1] != AutonomousFrame.StartByte2)
      ||(arrayData[length-1] != AutonomousFrame.StopByte2)
      ||(arrayData[length-2] != AutonomousFrame.StopByte1)){
    return 0;
  }
  // Checksum checking
  else{
    char res = 0;
    uint16_t checksum  = 0;
    checksum += (uint16_t)arrayData[2];
    checksum += (uint16_t)arrayData[3];
    checksum += (uint16_t)arrayData[4];
    checksum += (uint16_t)arrayData[5];
    checksum += (uint16_t)arrayData[6];
    for (int i = 7; i < 7 + arrayData[2]; i++){
      checksum += (uint16_t)arrayData[i];
    }
    if (checksum > 255) checksum %= 255;
    if((uint8_t)checksum == arrayData[length - 3]){
      res = 1;
    }
    return res;
  }



}

uint16_t AutonomousCommFrame_GetParamsFromStruct(AutonomousCommFrame_Typedef *frameObj){
    return ((uint16_t)(frameObj->Params[0]*256) + (uint16_t)frameObj->Params[1]);

}
uint16_t AutonomousCommFrame_GetParamsFromArray(uint8_t *arrayData, uint8_t length){
    return ((uint16_t)(arrayData[7]*256) + (uint16_t)arrayData[8]);
}


uint16_t  AutonomousCommFrame_SetParamsToStruct(uint16_t paramData,AutonomousCommFrame_Typedef *frameObj){
    frameObj->Params[1] = (uint8_t)paramData;
    frameObj->Params[0] = (uint8_t)(paramData >> 8);
    return AutonomousCommFrame_GetParamsFromStruct(frameObj);
}
void  AddonFrame_struct2Array(AddonBoardFrame * sData, unsigned char *array, unsigned char length){
    memcpy(array,sData,length);
}
void AddonFrame_Array2struct(unsigned char *array,AddonBoardFrame * sData, unsigned char length){
    memcpy(sData,array,length);
}
