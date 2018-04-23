#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "stm32f10x.h"
#include "MicroController/MyUart.h"
#include "BoxController.h"
#include "Devices/AutonomousCommFrame.h"
#include "main.h"
#define RPI_DETECT_PORT GPIOA
#define RPI_DETECT_PIN  GPIO_Pin_1
typedef struct {
   AutonomousCommFrame_Typedef *frameObj;
   uint8_t * dmaBuff_ptr;
   uint16_t dmaBuff_length;
   uint8_t * rxBuf_ptr;
   uint16_t  rxBuf_length;
   uint16_t  bufferIndex; 
} RPI_HandlesTypedef;
void RPI_Init(void);
void RPI_Update(void);
void RPI_Run(void);

// Apis
ConnectStatus_Typedef RPI_ConnectStatus(void);
void RPI_UpdateCurrentHeight(uint16_t height);
void RPI_SendMicTrigger(void);
void RPI_SendControllerStatus(WorkStatus_Typedef _status);
//Callbacks
void RPI_DmaCallback(DMA_RX_FLAG_Typedef flag);


static struct {
  void (*Init)(void);
  void (*Update)(void);
  void (*Run)(void);
  void (*UpdateCurrentHeight)(uint16_t);
  void (*SendMicTrigger)(void);
  void (*SendControllerStatus)(WorkStatus_Typedef);
  ConnectStatus_Typedef (*GetConnectStatus)(void);
  void (*Callback)(DMA_RX_FLAG_Typedef);
}
RpiCmds = {
    RPI_Init,
    RPI_Update,
    RPI_Run,
    RPI_UpdateCurrentHeight,
    RPI_SendMicTrigger,
    RPI_SendControllerStatus,
    RPI_ConnectStatus,
    RPI_DmaCallback
};


#endif

