#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "stm32f10x.h"
#include "MicroController/MyUart.h"
#include "BoxController.h"
#include "Devices/AutonomousCommFrame.h"
#include "main.h"

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

char enterAtMode();
char releaseAtMode();
char enterSleepMode();
char releaseSleepMode();
char wakup();
char isSleep(char *result);
char isWake(char * result);
char ping();
char getAdcValue();
char getAddress();
char getAdvertising();
char setAdvertising();
char getAdvertiseType();
char getBaudrate();
char setBaudrate();
char getCharacteristic();
char setCharacteristic();
char getName();
char setName();
char getPassword();
char setPassword();
char setUpBeacon();
char startBeacon();
char stopBeacon();
char getBeaconAddress();
char setBeaconAddress();

char getPowerOutput(); // 0: normal, 1: max, defaule:1
char setPowerOutput(char pw_output);

char getParityBit(); // 0:None, 1:Even, 2: ODD, defaule:0
char setParityBit();

char getModulePower(); // 0:-23dbm,1: -6dbm, 2: 0dbm,3:6dbm, default:2
char setModulePower();

char getSleepMode(); //  0: Auto Sleep, 1: dont auto sleep, defaut:1
char setSleepMode(char sleep_mode);
char getAdvertisingMode(); // 0: normal advertising, 1: realiable adverising
char setAdvertisingMode(char mode);
char factoryReset();
char getWorkingMode();
char setWorkingMode(char mode); // 0L peripheral, 1: central, df:0
char getRSSI(void *data, int *length);


char reset();


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

