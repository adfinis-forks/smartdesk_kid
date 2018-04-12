#include "SmartDesk/DataFrame.h"
#ifdef USE_JIECANG_BOX
void DataFrame_JIE_array2struct(unsigned char *data,JiecangStruct *jiecangFrame, unsigned char length){
    memcpy(jiecangFrame,data,length);
}
char DataFrame_JIE_isValid(JiecangStruct *mJiecangRxStruct){
    char res = -1;
    unsigned char checksum = 0;
    checksum =   mJiecangRxStruct->Function;
    checksum += mJiecangRxStruct->Length;
    checksum += mJiecangRxStruct->Data1;
    checksum += mJiecangRxStruct->Data2;
    checksum += mJiecangRxStruct->Data3;
    if (checksum == mJiecangRxStruct->Checksum){
        res = 1;
    }
    return res;    
}
#endif
unsigned char DataFrame_GetStructLength(FrameInit_Typedef * dataFrame){
    return (sizeof(FrameInit_Typedef) - 6 + dataFrame->LengthOfParams);
}
unsigned char DataFrame_StructToArray(FrameInit_Typedef * dataFrame, unsigned char *array){
    unsigned char len = DataFrame_GetStructLength(dataFrame)-1;
    array[0] = dataFrame->StartByte1;
    array[1] = dataFrame->StartByte2;
    array[2] = dataFrame->LengthOfParams;
    array[3] = dataFrame->ProductId;
    array[4] = dataFrame->ComponentId;
    array[5] = dataFrame->Instruction;
    array[6] = dataFrame->Address;
    for (int i = 7; i < 7 + dataFrame->LengthOfParams;i++){
        array[i] = dataFrame->Params[i-7];
    }
    array[len] = dataFrame->Endbyte2;
    array[len -1] = dataFrame->EndByte1;
    array[len -2] = DataFrame_UpdateChecksum(dataFrame);   
return len;    
}

unsigned char DataFrame_ArrayToStruct(unsigned char *array, FrameInit_Typedef * dataFrame, unsigned char arrayLength){
    dataFrame->StartByte1 = array[0];
    dataFrame->StartByte2 = array[1];
    dataFrame->LengthOfParams = array[2];
    dataFrame->ProductId = array[3];
    dataFrame->ComponentId = array[4];
    dataFrame->Instruction = array[5];
    dataFrame->Address = array[6];
    dataFrame->Params = &array[7];
    
    dataFrame->Endbyte2 = array[arrayLength - 1];
    dataFrame->EndByte1 = array[arrayLength - 2];
    dataFrame->CheckSum = array[arrayLength - 3];
    return dataFrame->LengthOfParams;
}
unsigned char DataFrame_UpdateArray(FrameInit_Typedef * dataFrame, unsigned char *array){
    unsigned char len = DataFrame_GetStructLength(dataFrame)-1;
    // Update checksum
    array[len -2] = DataFrame_UpdateChecksum(dataFrame);
    // Update Lengh of params
    array[2] = dataFrame->LengthOfParams;
    // Update Instructuon
    array[5] = dataFrame->Instruction;
    // Update Address
    array[6] = dataFrame->Address;
    // Update params
    for (int i = 7; i < 7 + dataFrame->LengthOfParams;i++){
        array[i] = dataFrame->Params[i-7];
    }
    return len;
}
unsigned char DataFrame_UpdateChecksum(FrameInit_Typedef * dataFrame){
    unsigned int sum =  (unsigned int)dataFrame->LengthOfParams + 
                        (unsigned int)dataFrame->ProductId + 
                        (unsigned int)dataFrame->ComponentId + 
                        (unsigned int)dataFrame->Instruction +
                        (unsigned int)dataFrame->Address;
    for (int i = 0; i < dataFrame->LengthOfParams; i++){
        sum += (unsigned int)dataFrame->Params[i];
    }
    if (sum > 255) sum %= 256;
    dataFrame->CheckSum = (unsigned char)sum;
    
    return dataFrame->CheckSum;
}
char DataFrame_isValidpackage(unsigned char *data, unsigned char length){
    char res = 0;
    unsigned int checksum  = 0;
    checksum += (unsigned int)data[2];
    checksum += (unsigned int)data[3];
    checksum += (unsigned int)data[4];
    checksum += (unsigned int)data[5];
    checksum += (unsigned int)data[6];
    for (int i = 7; i < 7 + data[2]; i++){
      checksum += (unsigned int)data[i];  
    }
    if (checksum > 255) checksum %= 256;
    if((unsigned char)checksum == data[length - 3])
        res = 1;
    return res;
}
unsigned short DataFrame_GetParams(unsigned char *array, unsigned char length){
    return ((unsigned short)(array[7]*256) + (unsigned short)array[8]);
}
void  AddonFrame_struct2Array(AddonBoardFrame * sData, unsigned char *array, unsigned char length){
  memcpy(array,sData,length);

}
void AddonFrame_Array2struct(unsigned char *array,AddonBoardFrame * sData, unsigned char length){
  memcpy(sData,array,length);
}
#ifdef USE_TIMOTION_BOX
  void  DataFrame_TIM_array2struct(unsigned char * array,TimotionRxStruct * _structData){
    memcpy(_structData,array,6);
  }

#endif
