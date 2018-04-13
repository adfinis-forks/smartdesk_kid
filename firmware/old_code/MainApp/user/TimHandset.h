#ifndef __TIM_HANDSET_H
#define __TIM_HANDSET_H
#include "stm32f10x.h"
#include "MicroController/MyUart.h"
#include "main.h"
/*
uint8_t const  upCmd[5]   = {0xD8, 0xD8, 0x66, 0x02, 0x02};
uint8_t const downCmd[5] = {0xD8, 0xD8, 0x66, 0x01, 0x01};
uint8_t const stopCmd[5] = {0xD8, 0xD8, 0x66, 0x00, 0x00};
uint8_t const pos1Cmd[5] = {0xD8, 0xD8, 0x66, 0x04, 0x04};
uint8_t const pos2Cmd[5] = {0xD8, 0xD8, 0x66, 0x08, 0x08};
uint8_t const pos3Cmd[5] = {0xD8, 0xD8, 0x66, 0x10, 0x10};
uint8_t const pos4Cmd[5] = {0xD8, 0xD8, 0x66, 0x20, 0x20};
uint8_t const saveCmd[5] = {0xD8, 0xD8, 0x66, 0x40, 0x40};
*/
typedef enum {
    CMD_STOP = 0x00,
    CMD_RESET = 0x03,
    CMD_UP   = 0x02,
    CMD_DOWN = 0x01,
    CMD_SAVE = 0x40,
    CMD_POS1 = 0x04,
    CMD_POS2 = 0x08,
    CMD_POS3 = 0x10,
    CMD_POS4 = 0x20,
    CMD_SAVE_POS1 = 0x44,
    CMD_SAVE_POS2 = 0x48,
    CMD_SAVE_POS3 = 0x50,
    CMD_SAVE_POS4 = 0x60,
    CMD_NULL = 0xff

}TIM_HANDSET_CMD_Typedef;

typedef enum {
    HIGHEST_PRIO = 0,
    MEDIUM_PRIO,
    LOW_PRIO
} TIM_HANDSET_DISPLAY_PRIO_Typedef;

void TIM_HANDSET_Init(void);

/*
    Xu Ly Cac Event Nhan Duoc Tu Handset Serial Port
    Can xu ly ngay tuc thoi nhanh.
    

*/
void TIM_HANDSET_Update(void);
void TIM_HANDSET_DisplayHeight(uint8_t height);
void TIM_HANDSET_Run(void);
void TIM_HS_SetDisplayPrio(TIM_HANDSET_DISPLAY_PRIO_Typedef _prio);
TIM_HANDSET_DISPLAY_PRIO_Typedef TIM_HS_GetDisplayPrio(void);
TIM_HANDSET_CMD_Typedef TIM_HANDSET_GetCommandFromDataPackage(void);
//Callback
void TIM_HANDSET_DmaCallback(void);
void TIM_HANDSET_rxCpy(void);
static struct {
    void (*Init)(void);
    void (*Update)(void);
    void (*DisplayHeight)(uint8_t);
    void (*Run)(void);
    void (*RxCopy)(void);
}TimHandset = {
  TIM_HANDSET_Init,
  TIM_HANDSET_Update,
  TIM_HANDSET_DisplayHeight,
  TIM_HANDSET_Run,
  TIM_HANDSET_rxCpy
};
#endif


