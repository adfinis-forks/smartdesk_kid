#ifndef __BOX_CONTROLLER_H
#define __BOX_CONTROLLER_H
#include "stm32f10x.h"
#include "TimBox.h"
#include "TimHandset.h"
typedef enum {
    BOX_STATE_FREE,
    BOX_STATE_UP,
    BOX_STATE_DOWN,
    BOX_STATE_STOP,
    BOX_STATE_MIC,
    BOX_STATE_SIT,
    BOX_STATE_STAND,
    BOX_STATE_SAVE_SIT,
    BOX_STATE_SAVE_STAND,
    BOX_STATE_RESET
    
} BOX_STATE_Typedef;




/*
    

*/
void BOX_CONTROLLER_Init(void);
void BOX_CONTROLLER_Update(void);
void BOX_CONTROLLER_Run(void);
void BOX_CONTROLLER_SetState( BOX_STATE_Typedef state, unsigned short data);
void BOX_CONTROLLER_SetHeight(uint16_t height_mm);
uint16_t BOX_CONTROLLER_GetHeight(void);
void BOX_CONTROLLER_SetMode(OPERATE_MODE_Typedef _mode);


#endif
