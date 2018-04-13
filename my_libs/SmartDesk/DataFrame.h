#ifndef __DATA_FRAME_H
#define __DATA_FRAME_H
#include <string.h>
#ifdef __cplusplus
 extern "C" {
#endif
// this Frame is used for communication between Add-on Board and Mic-button-led Board
typedef struct { // AddonBoardFrame
    unsigned char StartByte;
    unsigned char Instruction;
    unsigned char Param;
    unsigned char StopByte;
}AddonBoardFrame;


// These frames are used for communication between add-on board and RPI Board
static const struct{ 
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
}ParamAddress = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};// ParamAddress
static const struct { 
  const unsigned char MayaRobot;
  const unsigned char Clock;
  const unsigned char SmartDesk;
  const unsigned char iOS;
  const unsigned char TeleRobot ;
    
} ProductId = {1,2,3,4,5}; // ProductId
static const struct { 
   const unsigned char Read ;
   const unsigned char Write;
   const unsigned char Return ;
} Instructions = {1,2,3};// Instructions
static const struct { 
   const unsigned char PanMotor;
   const unsigned char TiltMotor;
   const unsigned char UpDownMotor;
   const unsigned char Base;
   const unsigned char AddonBoard;

} ComponentId = {1,2,3,4,5}; // ComponentId
static const struct { 
    const unsigned char Version     ;       // 1
    const unsigned char Height;             // 2
    const unsigned char Up;                 // 3
    const unsigned char Down;               // 4
    const unsigned char Stop;               // 5
    const unsigned char SaveToPos1;         // 6
    const unsigned char MoveToPos1;         // 7
    const unsigned char SaveToPos2;         // 8
    const unsigned char MoveToPos2;         // 9
    const unsigned char User1Height;        // 10
    const unsigned char User2Height;        // 11
    const unsigned char User3Height;        // 12
    const unsigned char User4Height;        // 13
    const unsigned char User5Height;        // 14
    const unsigned char User6Height;        // 15
    
    const unsigned char MinHeight;          // 16
    const unsigned char MaxHeight;          // 17
    const unsigned char SetPointHeight;     // 18
    const unsigned char OperateMode;        // 19
    const unsigned char UserID;             // 20
    const unsigned char BoxStatus;          // 21
    const unsigned char Speaker;            // 22
    const unsigned char Mic;                // 23
    
    
    const unsigned char LengthOfAddr;       // 24
}ControlTableAddress = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}; // ControlTableAddress
typedef struct { 
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
}FrameInit_Typedef; // FrameInit_Typedef
typedef struct { 
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
}ZmqFrameStruct_Typedef; // FrameInit_Typedef
unsigned char DataFrame_GetStructLength(FrameInit_Typedef * dataFrame);
unsigned char DataFrame_StructToArray(FrameInit_Typedef * dataFrame, unsigned char *array);
unsigned char DataFrame_ArrayToStruct(unsigned char *array, FrameInit_Typedef * dataFrame, unsigned char arrayLength);
unsigned char DataFrame_UpdateArray(FrameInit_Typedef * dataFrame, unsigned char *array);
unsigned char DataFrame_UpdateChecksum(FrameInit_Typedef * dataFrame);
char          DataFrame_isValidpackage(unsigned char *data, unsigned char length);
unsigned short DataFrame_GetParams(unsigned char *array, unsigned char length); 
void  AddonFrame_struct2Array(AddonBoardFrame * sData, unsigned char *array, unsigned char length);
void AddonFrame_Array2struct(unsigned char *array,AddonBoardFrame * sData, unsigned char length);

#ifdef USE_JIECANG_BOX
    #define JIECANG_TX_START_BYTE1 0xF1
    #define JIECANG_TX_START_BYTE2 0xF1
    #define JIECANG_STOP_BYTE = 0x7E
    
    #define JIECANG_RX_START_BYTE1 = 0xF2
    #define JIECANG_RX_START_BYTE2 = 0xF2
    const static struct{
        const unsigned char Up;
        const unsigned char Down ;
        const unsigned char Status;
        const unsigned char Stop;
        const unsigned char MinMax;
        
    }JiecangCmd = {
    0x01,0x02,0x07,0x0A,0x0C};
    typedef struct {
        unsigned char StartByte1;
        unsigned char StartByte2;
        unsigned char Function;
        unsigned char Length;
        unsigned char Data1;
        unsigned char Data2;
        unsigned char Data3;
        unsigned char Checksum;
        unsigned char StopByte;
    } JiecangStruct;
    static const struct {
        const unsigned char StartByte1;
        const unsigned char StartByte2;
        const unsigned char StopByte;
    }JiecangHeader = {0xF2, 0xF2, 0x7E};
    void DataFrame_JIE_array2struct(unsigned char *data,JiecangStruct *jiecangFrame, unsigned char length);
    char DataFrame_JIE_isValid(JiecangStruct *mJiecangRxStruct);


#endif
#ifdef USE_TIMOTION_BOX
    #define TIM_BOX_RX_HEADER1 0x98
    #define TIM_BOX_RX_HEADER2 0x98  
    #define TIM_BOX_TX_HEADER1 0xD8
    #define TIM_BOX_TX_HEADER2 0xD8
    
    typedef struct {
        unsigned char StartByte1;
        unsigned char StartByte2;
        unsigned char Param1;
        unsigned char Param2;
        unsigned char Data1;
        unsigned char Data2;
    }TimotionRxStruct;
    typedef struct {
        unsigned char StartByte1;
        unsigned char StartByte2;
        unsigned char Param;
        unsigned char Command1;
        unsigned char Command2;
    }TimotionTxStruct;
    static const struct {
        const unsigned char Up;
        const unsigned char Down;
        const unsigned char Stop;
        const unsigned char Pos1;
        const unsigned char Pos2;
        const unsigned char Pos3;
        const unsigned char Pos4;
        const unsigned char Save;
        const unsigned char Reset;
    
    }TimBoxCmdByte = {0x02, 0x01,0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x03};
    void  DataFrame_TIM_array2struct(unsigned char * array,TimotionRxStruct * _structData);
    
#endif
#ifdef USE_ADDON_BOARD
        
#endif
#ifdef USE_RPI
    
#endif

#ifdef __cplusplus
}
#endif
#endif

