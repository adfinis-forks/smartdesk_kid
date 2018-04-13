#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "MicroController/MyFlash.h"
#define TIM_BOX_RX_PRIO  0
#define TIM_HANDSET_RX_PRIO 1
#define MBL_RX_PRIO 2
#define RPI_RX_PRIO 3

void Delay(__IO uint32_t nCount);
//#define SYSCLK_HSE
#define SYSCLK_FREQ_24MHz
#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL && !defined STM32F10X_HD_VL
  //#define SYSCLK_FREQ_36MHz
  //#define SYSCLK_FREQ_48MHz
  //#define SYSCLK_FREQ_56MHz
  #define SYSCLK_FREQ_72MHz
#endif
typedef enum{
  FLAG_NULL = 0,
  HALF_COMPLETED,
  FULL_COMPLETED
  
}DMA_RX_FLAG_Typedef;
typedef enum{
    STATE_UNCHANGE = 0,
    STATE_CHANGED
}STATE_CHANGE_STATTUS_Typedef;
typedef struct{
    uint16_t periodCount;
    uint16_t timeOutCount;
    uint16_t timeOutMax;
}STATE_TIME_Typedef;
typedef enum {
    MODE_MANUAL = 1,
    MODE_AUTO
}OPERATE_MODE_Typedef;
typedef enum {
    SET_HEIGHT_INCOMPLETE = 0,
    SET_HEIGHT_COMPLETE
}SETHEIGHT_STATTUS_Typedef;
static void Delay(__IO uint32_t nCount){
	while(nCount--){
	}
}
typedef enum {
  NOT_CONNECT = 0,
  CONNECTED
}ConnectStatus_Typedef;
typedef enum {
  CONTROLLER_FREE = 0,
  CONTROLLER_BUSY

}WorkStatus_Typedef;
#endif
