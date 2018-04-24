#include "Bluetooth.h"
#define RPI_START_BYTE 0xFF
#define RPI_STOP_BYTE 0xFA
#define BT_PORT GPIOA
#define BT_RESET_PIN  GPIO_Pin_1
#define BT_LED_PIN  GPIO_Pin_2
#define BT_KEY_PIN  GPIO_Pin_3
static uint8_t rpiDmaBuf[4];
static uint8_t rpiRxBuf[120];
static uint8_t rpiTxBuf[12];
static FlagStatus RPI_DMA_FLAG = RESET;
static AutonomousCommFrame_Typedef rpiFrame;
static RPI_HandlesTypedef RpiHandles = {
  &rpiFrame,
  rpiDmaBuf,
  sizeof(rpiDmaBuf),
  rpiRxBuf,
  sizeof(rpiRxBuf),
  0
};

/*
Static Functions
*/ 
static WorkStatus_Typedef  controllerState = CONTROLLER_FREE;
static void RPI_PutBytes(uint8_t * _data, uint8_t length)
{
  UARTx_PutBytesDMA(USART2,_data,length);
}
static void RPI_GetInfoFromPackage(void)
{
}
void RPI_Init(void){
  // init data handle for uart2
  UARTx_Init(USART2,9600);
  UARTx_RxDmaInit(USART2, rpiDmaBuf, sizeof(rpiDmaBuf)); 
  UARTx_SetRxDmaIntPriority(USART2,NVIC_PriorityGroup_0,2,0);          
  UARTx_Start(USART2);
  
  GPIO_InitTypeDef GPIO_InitStructure;
  // Init for DETECT_PIN
  if(BT_PORT == GPIOA)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  }
  else if(BT_PORT == GPIOB){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  }
  else if(BT_PORT == GPIOC)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  }
  else if(BT_PORT == GPIOD)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  }
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Pin = BT_RESET_PIN|BT_KEY_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BT_PORT, &GPIO_InitStructure);    
  RpiCmds.SendControllerStatus(CONTROLLER_FREE);
}
void RPI_Update(void)
{
  WorkStatus_Typedef temState = TimBoxCmd.GetControllerStatus();
  if (controllerState != temState){
    controllerState = temState; 
    RpiCmds.SendControllerStatus(controllerState); 
  }
  
  if(RPI_DMA_FLAG == SET)
  {   
    uint8_t  i = 0;
    uint16_t index = RpiHandles.bufferIndex; // get current Index of rx buffer
    uint16_t length = AutonomousFrame.FrameLength -1; 
    if (index >= length)  { // buffer dai nhan toi thieu 12 byte moi bat dau lay thong tin
      
      while (i <=  index - length)
      {        
        if (AutonomousCommFrameCmd.IsValidPacket(&(RpiHandles.rxBuf_ptr[i]),AutonomousFrame.FrameLength))
        {
          AutonomousCommFrameCmd.Array2Struct(&RpiHandles.rxBuf_ptr[i],AutonomousFrame.FrameLength,RpiHandles.frameObj);
          /*
          Get information in packet from Raspberry
          */
          RPI_GetInfoFromPackage();
          i =  index + length; 
          RpiHandles.bufferIndex = 0;
        }
        else i += 1;
      }
      
    }
    
    RPI_DMA_FLAG = RESET;
  }
}
void RPI_Run(void)
{
  
}
ConnectStatus_Typedef RPI_ConnectStatus(void)
{
//  return (ConnectStatus_Typedef)GPIO_ReadInputDataBit(BT_PORT,RPI_DETECT_PIN);   
}
void RPI_rxCpy(void)
{ 
  if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length))
  {
    memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],RpiHandles.dmaBuff_ptr,RpiHandles.dmaBuff_length);
    RpiHandles.bufferIndex += RpiHandles.dmaBuff_length;
  }
  else
  {
    RpiHandles.bufferIndex = 0;
    memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);    
  }
}


void RPI_PutFrame(AutonomousCommFrame_Typedef *frame)
{
  
}
void RPI_DmaCallback(DMA_RX_FLAG_Typedef flag)
{
  switch (flag)
  {
  case HALF_COMPLETED:
    if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length))
    {
      memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],RpiHandles.dmaBuff_ptr,RpiHandles.dmaBuff_length/2);
      RpiHandles.bufferIndex += RpiHandles.dmaBuff_length/2;
    }
    else
    {
      RpiHandles.bufferIndex = 0;
      memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);
    }
    break;
  case FULL_COMPLETED:
    if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length/2))
    {
      memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],&RpiHandles.dmaBuff_ptr[RpiHandles.dmaBuff_length/2],RpiHandles.dmaBuff_length/2);
      RpiHandles.bufferIndex += RpiHandles.dmaBuff_length/2;
    }
    else
    {
      RpiHandles.bufferIndex = 0;
      memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);
    }
    break;
  }
  RPI_DMA_FLAG = SET; 
  
}


void RPI_UpdateCurrentHeight(uint16_t height)
{
}

void RPI_SendMicTrigger(void)
{    
}

void RPI_SendControllerStatus(WorkStatus_Typedef _status)
{

}

char enterAtMode()
{
  
}
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



