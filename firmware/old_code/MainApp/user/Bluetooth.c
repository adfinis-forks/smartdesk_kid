#include "Bluetooth.h"
#define RPI_START_BYTE 0xFF
#define RPI_STOP_BYTE 0xFA

static uint8_t rpiDmaBuf[4];
static uint8_t rpiRxBuf[120];
static uint8_t rpiTxBuf[12];
static AutonomousCommFrame_Typedef dataFrame;
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
static void RPI_PutBytes(uint8_t * _data, uint8_t length){
    UARTx_PutBytesDMA(USART2,_data,length);
}
static void RPI_GetInfoFromPackage(void){
  //uint8_t instruction = RpiHandles.FrameHandles->structData->Instruction;
  //uint8_t address = RpiHandles.FrameHandles->structData->Address;
  // Get a write Instruction
  uint8_t instruction = RpiHandles.frameObj->Instruction;
  uint8_t address = RpiHandles.frameObj->Address;
  uint16_t data = AutonomousCommFrameCmd.GetDataFromStruct(RpiHandles.frameObj);
  if(instruction == Instructions.Write){
     if (address == ControlTableAddress.Up){   // Up
        BOX_CONTROLLER_SetState(BOX_STATE_UP,0);
        #ifdef DEBUG_RPI
            printf("[I]-Move Up From RPI\n");
        #endif
     
     }
     else if ( address == ControlTableAddress.Down){  // Down
        BOX_CONTROLLER_SetState(BOX_STATE_DOWN,0);
        #ifdef DEBUG_RPI
            printf("[I]-Move Down From RPI\n");
        #endif
     }
     else if (address == ControlTableAddress.Stop){ // Stop
        
        BOX_CONTROLLER_SetState(BOX_STATE_STOP,0);
        #ifdef DEBUG_RPI
            printf("[I]-Stop From RPI\n");
        #endif
     }
     else if (address == ControlTableAddress.SetPointHeight) { // Set Height
        data = data /10;
        data = data*10;
        BOX_CONTROLLER_SetHeight(data);
        #ifdef DEBUG_RPI
            printf("[I]-Set Height From RPI %d\n",data);
        #endif
        }        
     else if (address == ControlTableAddress.SaveToPos1){  // Save To Sit Position
         if (data > 0){
            BOX_CONTROLLER_SetState(BOX_STATE_SAVE_SIT,0);
            #ifdef DEBUG_RPI
                printf("Save Sit Pos From RPI\n");
            #endif
         }
         
     }
     
     else if (address == ControlTableAddress.MoveToPos1){  // Move To Sit Position
       BOX_CONTROLLER_SetState(BOX_STATE_SIT, 0 );
       #ifdef DEBUG_RPI
            printf("Move To Sit Position From RPI\n");
       #endif
     }
     
     else if (address == ControlTableAddress.SaveToPos2){ //Save To stand position
         if (data > 0){
            BOX_CONTROLLER_SetState(BOX_STATE_SAVE_STAND,0);
            #ifdef DEBUG_RPI
                printf("Save Stand Pos From RPI\n");
            #endif
         }
     }
     
     else if (address == ControlTableAddress.MoveToPos2){    // Move to stand position    
          BOX_CONTROLLER_SetState(BOX_STATE_STAND, 0);
          #ifdef DEBUG_RPI
                printf("Move to Stand Pos From RPI\n");
          #endif
     }
} 
  // Get a read Instruction
  else if (instruction == Instructions.Read){
     if(address == ControlTableAddress.Height){ // Read Current Height
        dataFrame.Instruction = Instructions.Return;
        dataFrame.Address = ControlTableAddress.Height;      
        uint16_t h = TIM_BOX_GetHeight()*10;
        dataFrame.Params[1] = (uint8_t)h;
        dataFrame.Params[0] =  (uint8_t)(h>>8);
        AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
        AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
        RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));

     }
     else if (address == ControlTableAddress.SaveToPos1){ // Read Sit Pos
        dataFrame.Instruction = Instructions.Return;
        dataFrame.Address = ControlTableAddress.SaveToPos1;
        uint16_t h = TimBoxCmd.GetSitPos();
        dataFrame.Params[1] = (uint8_t)h;
        dataFrame.Params[0] = (uint8_t)(h>>8);
        AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
        AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
        RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));
        #ifdef DEBUG_RPI
            printf("OK");
        #endif
     }
     else if(address == ControlTableAddress.SaveToPos2){  // Read Stand Pos
        dataFrame.Instruction = Instructions.Return;
        dataFrame.Address = ControlTableAddress.SaveToPos2;
        uint16_t h = TimBoxCmd.GetStandPos();
        dataFrame.Params[1] = (uint8_t)h;
        dataFrame.Params[0] = (uint8_t)(h>>8);
        AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
        AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
        RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));
        #ifdef DEBUG_RPI
            printf("OK");
        #endif
        
     }
    
  } 
}

/*

*/
void RPI_Init(void){

    dataFrame.StartByte1 = 0xFF;
    dataFrame.StartByte2 = 0xFF;
    dataFrame.LengthOfParams = 2;
    dataFrame.ProductId = ProductId.SmartDesk;
    dataFrame.ComponentId = ComponentId.UpDownMotor;
    dataFrame.Instruction = Instructions.Write;
    dataFrame.Address = ControlTableAddress.Up;
    dataFrame.Params[0] = 0;
    dataFrame.Params[1] = 0;
    dataFrame.CheckSum = 10;
    dataFrame.EndByte1 = 0xFA;
    dataFrame.EndByte2 = 0xFA;

    // init data handle for uart2
    //UART2_RxDmaInit(57600,rpiDmaBuf,sizeof(rpiDmaBuf));
    UARTx_Init(USART2,9600);
    UARTx_RxDmaInit(USART2, rpiDmaBuf, sizeof(rpiDmaBuf)); 
    UARTx_SetRxDmaIntPriority(USART2,NVIC_PriorityGroup_0,2,0); 
   //UARTx_SetTxDmaIntPriority(USART1,NVIC_PriorityGroup_0,14,0);         
    UARTx_Start(USART2);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	// Init for DETECT_PIN
	if(RPI_DETECT_PORT == GPIOA){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if(RPI_DETECT_PORT == GPIOB){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if(RPI_DETECT_PORT == GPIOC){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if(RPI_DETECT_PORT == GPIOD){
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = RPI_DETECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RPI_DETECT_PORT, &GPIO_InitStructure); 
    RpiCmds.SendControllerStatus(CONTROLLER_FREE);
}
void RPI_Update(void){
  WorkStatus_Typedef temState = TimBoxCmd.GetControllerStatus();
  if (controllerState != temState){
       controllerState = temState; 
       RpiCmds.SendControllerStatus(controllerState); 
  }
  
  if(RPI_DMA_FLAG == SET){
    uint8_t  i = 0;
    uint16_t index = RpiHandles.bufferIndex; // get current Index of rx buffer
    uint16_t length = AutonomousFrame.FrameLength -1; 
    if (index >= length)  { // buffer dai nhan toi thieu 12 byte moi bat dau lay thong tin
      
        while (i <=  index - length){        
          if (AutonomousCommFrameCmd.IsValidPacket(&(RpiHandles.rxBuf_ptr[i]),AutonomousFrame.FrameLength)){
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
void RPI_Run(void){}
ConnectStatus_Typedef RPI_ConnectStatus(void){
    return (ConnectStatus_Typedef)GPIO_ReadInputDataBit(RPI_DETECT_PORT,RPI_DETECT_PIN);   
}
void RPI_rxCpy(void){
  if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length)){
      memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],RpiHandles.dmaBuff_ptr,RpiHandles.dmaBuff_length);
      RpiHandles.bufferIndex += RpiHandles.dmaBuff_length;
  }
  else{
    RpiHandles.bufferIndex = 0;
    memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);    
  }
}


void RPI_PutFrame(AutonomousCommFrame_Typedef *frame){
    
}
void RPI_DmaCallback(DMA_RX_FLAG_Typedef flag){
  switch (flag){
  case HALF_COMPLETED:
    if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length)){
        memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],RpiHandles.dmaBuff_ptr,RpiHandles.dmaBuff_length/2);
        RpiHandles.bufferIndex += RpiHandles.dmaBuff_length/2;
    }
    else{
      RpiHandles.bufferIndex = 0;
      memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);
    }
    break;
  case FULL_COMPLETED:
    if(RpiHandles.bufferIndex < (RpiHandles.rxBuf_length - RpiHandles.dmaBuff_length/2)){
        memcpy(&RpiHandles.rxBuf_ptr[RpiHandles.bufferIndex],&RpiHandles.dmaBuff_ptr[RpiHandles.dmaBuff_length/2],RpiHandles.dmaBuff_length/2);
        RpiHandles.bufferIndex += RpiHandles.dmaBuff_length/2;
    }
    else{
      RpiHandles.bufferIndex = 0;
      memset(RpiHandles.rxBuf_ptr, 0 , RpiHandles.rxBuf_length);
    }
    break;
  }
  RPI_DMA_FLAG = SET; 
    
}


void RPI_UpdateCurrentHeight(uint16_t height){
    dataFrame.Instruction = Instructions.Update;
    dataFrame.Address = ControlTableAddress.Height;
    dataFrame.Params[1] = (uint8_t)height;
    dataFrame.Params[0] = (uint8_t)(height>>8);
    AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
    AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
    RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));
}

void RPI_SendMicTrigger(void){
    dataFrame.Instruction = Instructions.Update;
    dataFrame.Address = ControlTableAddress.Mic;
    dataFrame.Params[1] = (uint8_t)0;
    dataFrame.Params[0] = (uint8_t)1;
    AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
    AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
    RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));    
}

void RPI_SendControllerStatus(WorkStatus_Typedef _status){
    dataFrame.Instruction = Instructions.Update;
    dataFrame.Address = ControlTableAddress.BoxStatus;
    dataFrame.Params[0] = (uint8_t)0;
    dataFrame.Params[1] = (uint8_t)_status;    
    AutonomousCommFrameCmd.UpdateStructChecksum(&dataFrame);
    AutonomousCommFrameCmd.Struct2Array(&dataFrame,rpiTxBuf);
    RPI_PutBytes(rpiTxBuf,(unsigned int)sizeof(rpiTxBuf));     

}



