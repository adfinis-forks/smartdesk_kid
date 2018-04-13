/*
 * File:   AutonomousCommFrame.h
 * Author: thanhLe
 * Company: Autonomous.ai
 * Email: qhanh_le@hotmail.com
 * Created on Oct 27, 2016
 */

/*
 * Usage:
 * 1. Check the data packet if its valid
 * 2. copy the packet to the stuct for easy to analisic
 * 3. Get data from the struct such as (Instruction, Id,...)
 *
 *

 */

#ifndef __AUTONOMOUS_COMM_FRAME_H
#define __AUTONOMOUS_COMM_FRAME_H
#include <stdint.h>
#include <stdio.h>
#include "stdint.h"
#include <string.h>
#ifdef __cplusplus
extern "C"
{
#endif
typedef struct { // AddonBoardFrame
    unsigned char StartByte;
    unsigned char Instruction;
    unsigned char Param;
    unsigned char StopByte;
}AddonBoardFrame;
typedef struct{
    uint8_t StartByte1 ;
    uint8_t StartByte2 ;
    uint8_t LengthOfParams ;
    uint8_t ProductId ;
    uint8_t ComponentId ;
    uint8_t Instruction ;
    uint8_t Address;
    uint8_t Params[2];
    uint8_t CheckSum;
    uint8_t EndByte1;
    uint8_t EndByte2;
} AutonomousCommFrame_Typedef;
static const struct {
  uint8_t FrameLength;
  uint8_t StartByte1;
  uint8_t StartByte2;
  uint8_t StopByte1;
  uint8_t StopByte2;
}AutonomousFrame = {
  12,
  0xFF,
  0xFF,
  0xFA,
  0xFA
};
// Struct of frame for processing
static const struct {
  const uint8_t MayaRobot;
  const uint8_t Clock;
  const uint8_t SmartDesk;
  const uint8_t iOS;
  const uint8_t TeleRobot ;
  const uint8_t SmatCabinet;
  const uint8_t SmartBinV2;

} ProductId = {1,2,3,4,5,6,7}; // ProductId
static const struct {
   const uint8_t Read ;
   const uint8_t Write;
   const uint8_t Return ;
   const uint8_t Update;
} Instructions = {1,2,3,4};// Instructions
static const struct {
   const uint8_t PanMotor;
   const uint8_t TiltMotor;
   const uint8_t UpDownMotor;
   const uint8_t Base;
   const uint8_t AddonBoard;
   const uint8_t RpiBoard;
   const uint8_t Sensors;
   const uint8_t RcServo;
   const uint8_t NotCheck;

} ComponentId = {1,2,3,4,5,6,7,8,9}; // ComponentId
static const struct {
    const uint16_t Version     ;       // 0x01 - 1
    const uint16_t Height;             // 0x02 - 2
    const uint16_t Up;                 // 0x03 - 3
    const uint16_t Down;               // 0x04 - 4
    const uint16_t Stop;               // 0x05 - 5
    const uint16_t SaveToPos1;         // 0x06 - 6
    const uint16_t MoveToPos1;         // 0x07 - 7
    const uint16_t SaveToPos2;         // 0x08 - 8
    const uint16_t MoveToPos2;         // 0x09 - 9
    const uint16_t SaveToPos3;         // 0x0A - 10
    const uint16_t MoveToPos3;         // 0x0B - 11
    const uint16_t SaveToPos4;         // 0x0C - 12
    const uint16_t MoveToPos4;         // 0x0D - 13
    const uint16_t User9Height;        // 0x0E - 14
    const uint16_t User10Height;       // 0x0F - 15

    const uint16_t MinHeight;          // 0x10 - 16
    const uint16_t MaxHeight;          // 0x11 - 17
    const uint16_t SetPointHeight;     // 0x12 - 18
    const uint16_t OperateMode;        // 0x13 - 19
    const uint16_t UserID;             // 0x14 - 20
    const uint16_t BoxStatus;          // 0x15 - 21: 0: Free, 1: Busy
    const uint16_t ControllerState;    // 0x16 - 22
    const uint16_t MoveDirection;      // 0x17 - 23
    const uint16_t Pos1Height;         // 0x18 - 24
    const uint16_t Pos2Height;         // 0x19 - 25
    const uint16_t Pos3Height;         // 0x1A - 26
    const uint16_t Pos4Height;         // 0x1B - 27
    const uint16_t DualMotorReset;     // 0x1C - 28    // No Down Command
    const uint16_t Speaker;            // 0x1D - 29
    const uint16_t Mic;                // 0x1E - 30
    const uint16_t SingleMotorReset;           // 0x1F - 31 // With Down Command


    const uint16_t LengthOfAddr;       // 0x20 - 32
}ControlTableAddress =
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
static const struct {
    const uint16_t Version     ;        // 0x01 - 1
    const uint16_t TrashStatus;  
    const uint16_t RecycleStatus;
    const uint16_t Servo;               // 0: Home, 100 left, 200 righ
    const uint16_t Light;               //0-100: Do sang
    const uint16_t DoorStatus;       // 0: closed, >0 opened
    const uint16_t TrashGasSensor;          // 0: Normal, > 0 Detected
    const uint16_t RecycleGasSensor;          // 0 Normal, >0 Detected
    const uint16_t TrashTemSensor;
    const uint16_t RecycleTemSensor;
    const uint16_t TrashHumSensor;
    const uint16_t RecycleHumSensor;
    const uint16_t CoverMotor;
    const uint16_t DoorMotor;
    const uint16_t ResetButton;
    const uint16_t StatusLedColor; // 0: Off, 1:Red, 2: Green, 3: Blue, 4: Yellow, 5: While
    const uint16_t StatusLedBlink; // 0: off, > 1: 50ms, 2:100ms, 3:150ms, 4:200ms. 
    const uint16_t TrashButton; // 0: off, > 1: 50ms, 2:100ms, 3:150ms, 4:200ms. 
    const uint16_t RecycleButton;
    const uint16_t CoverButton;
    const uint16_t LengthOfAddr;
}SmartBinV2ControlTable = 
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
static const struct  {
    const uint16_t Version;         //1
    const uint16_t Backlight;       //2
    const uint16_t Brightness;      //3
    const uint16_t Touch2On;        //4
    const uint16_t Touch2Off;       //5
    const uint16_t Touch2Toggle;    //6
    const uint16_t LengthOfAddr;    //7
} RpiControlTable = {1,2,3,4,5,6,7};

uint8_t AutonomousCommFrame_GetStructLength(AutonomousCommFrame_Typedef *frameObj);
uint8_t AutonomousCommFrame_Struct2Array(AutonomousCommFrame_Typedef *frameObj,uint8_t *arrayData );
uint8_t AutonomousCommFrame_Array2Struct(uint8_t *arrayData , uint16_t arrayLength, AutonomousCommFrame_Typedef *frameObj);
uint8_t AutonomousCommFrame_UpdateStructChecksum(AutonomousCommFrame_Typedef *frameObj);
uint8_t AutonomousCommFrame_IsValidPacket(uint8_t *arrayData, uint8_t length);
uint16_t AutonomousCommFrame_GetParamsFromStruct(AutonomousCommFrame_Typedef *frameObj);
uint16_t AutonomousCommFrame_GetParamsFromArray(uint8_t *arrayData, uint8_t length);
uint16_t  AutonomousCommFrame_SetParamsToStruct(uint16_t paramData,AutonomousCommFrame_Typedef *frameObj);
void  AddonFrame_struct2Array(AddonBoardFrame * sData, unsigned char *array, unsigned char length);
void AddonFrame_Array2struct(unsigned char *array,AddonBoardFrame * sData, unsigned char length);
static struct {
    uint8_t  (*GetStuctLength)(AutonomousCommFrame_Typedef *);
    uint8_t  (*Struct2Array)(AutonomousCommFrame_Typedef *,uint8_t *);
    uint8_t  (*Array2Struct)(uint8_t *,uint16_t,AutonomousCommFrame_Typedef * );
    uint8_t  (*UpdateStructChecksum)(AutonomousCommFrame_Typedef *);
    uint8_t  (*IsValidPacket)(uint8_t *, uint8_t);
    uint16_t (*GetDataFromStruct)(AutonomousCommFrame_Typedef *);
    uint16_t (*GetDataFromArray)(uint8_t *, uint8_t);
    uint16_t (*SetDataToStruct)(uint16_t,AutonomousCommFrame_Typedef *);

} AutonomousCommFrameCmd = {
  AutonomousCommFrame_GetStructLength,
  AutonomousCommFrame_Struct2Array,
  AutonomousCommFrame_Array2Struct,
  AutonomousCommFrame_UpdateStructChecksum,
  AutonomousCommFrame_IsValidPacket,
  AutonomousCommFrame_GetParamsFromStruct,
  AutonomousCommFrame_GetParamsFromArray,
  AutonomousCommFrame_SetParamsToStruct
};


#ifdef __cplusplus
}
#endif
#endif
