#ifndef __DATA_FRAME_H
#define __DATA_FRAME_H
#include <string.h>
#ifdef __cplusplus
 extern "C" {
#endif

static const struct{ // ParamAddress
    const unsigned char MblUpLed;       //1
    const unsigned char MblDownLed;     //2
    const unsigned char MblStopLed;     //3
    const unsigned char MblMicLed;      //4
    const unsigned char MblSit;         //5
    const unsigned char MblStand;       //6
    const unsigned char MblSaveSit;     //7
    const unsigned char MblSaveStand;   //8
    
    const unsigned char HsUpLed;        //9
    const unsigned char HsDownLed;      //10
    const unsigned char HsStopLed;      //11
    const unsigned char SavePos1Led;    //12
    const unsigned char MovePos1Led;    //13
    const unsigned char SavePos2Led;    //14
    const unsigned char MovePos2Led;    //15
    const unsigned char MblReset;       //16
    const unsigned char HsReset;        //17
}ParamAddress = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
 





typedef struct { // FrameInit_Typedef
    unsigned char StartByte1 ;
    unsigned char StartByte2 ;
    unsigned char LengthOfParams ;
    unsigned char ProductId ;
    unsigned char ComponentId ;
    unsigned char Instruction ;
    unsigned char Address;
    unsigned char *Params;
    unsigned char CheckSum;
    unsigned char EndByte1;
    unsigned char Endbyte2;
}FrameInit_Typedef;


unsigned char DataFrame_GetLength(FrameInit_Typedef * dataFrame);
unsigned char DataFrame_StructToArray(FrameInit_Typedef * dataFrame, unsigned char *array);
unsigned char DataFrame_ArrayToStruct(unsigned char *array, FrameInit_Typedef * dataFrame, unsigned char arrayLength);
unsigned char DataFrame_UpdateArray(FrameInit_Typedef * dataFrame, unsigned char *array);
unsigned char DataFrame_UpdateChecksum(FrameInit_Typedef * dataFrame);
char          DataFrame_isValidpackage(unsigned char *data, unsigned char length);
unsigned short DataFrame_GetParams(unsigned char *array, unsigned char length); 


#ifdef __cplusplus
}
#endif
#endif

