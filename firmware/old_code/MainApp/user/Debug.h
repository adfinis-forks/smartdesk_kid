#ifndef __DEBUG_H
#define __DEBUG_H
#include "stm32f10x.h"
#include "MicroController/MyUart.h"
#include "TimBox.h"
#include "Speaker.h"
#include "TimHandset.h"
#include "AddonBoard.h"
void DEBUG_Init(void);
void DEBUG_Update(void);
void DEBUG_GetString(void);
void DEBUG_UartFullFcn(void);
void DEBUG_LfFcn(void);
void DEBUG_PRINT(const char * s);
#endif
