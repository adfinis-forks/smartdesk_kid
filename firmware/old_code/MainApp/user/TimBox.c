#include "TimBox.h"
#include "main.h"
#include "TimHandset.h"
#define DETECT_PORT             GPIOB
#define DETECT_PIN              GPIO_Pin_12
#define UP_DOWN_SLEEP_PORT      GPIOC
#define UP_PIN                  GPIO_Pin_6
#define SLEEP_PIN	        GPIO_Pin_7
#define DOWN_PIN                GPIO_Pin_8
//#define  TIM_BOX_RX_HEADER 0x98

/*
    Handle quan ly uart
*/
static uint16_t minHeight = 400;
static uint16_t maxHeight = 1300;
static uint8_t timotionDmaBuf[12];
static uint8_t ledCount = 0;
uint8_t resbuf[6] = {0,0,0,0,0,0};
uint8_t longTimRxBuf[30];
uint8_t dmaBufLength = sizeof(timotionDmaBuf);
uint8_t timRxBufLength = sizeof(longTimRxBuf);
const uint16_t sitPosAddr = 0x1111;
const uint16_t standPosAddr = 0x2222;
uint16_t VirtAddVarTab[NumbOfVar] = {0x1111, 0x2222, 0x7777,0x8888};
uint16_t sitPosVal;
uint16_t standPosVal;
uint8_t rxTxDma = 0;
uint8_t bufIndex = 0;
uint8_t check = 0;
static uint8_t detectPinStatus;
uint8_t const upCmd[5]   =  {0xD8, 0xD8, 0x66, 0x02, 0x02};
uint8_t const downCmd[5]  = {0xD8, 0xD8, 0x66, 0x01, 0x01};
uint8_t const stopCmd[5]  = {0xD8, 0xD8, 0x66, 0x00, 0x00};
uint8_t const pos1Cmd[5]  = {0xD8, 0xD8, 0x66, 0x04, 0x04};
uint8_t const pos2Cmd[5]  = {0xD8, 0xD8, 0x66, 0x08, 0x08};
uint8_t const pos3Cmd[5]  = {0xD8, 0xD8, 0x66, 0x10, 0x10};
uint8_t const pos4Cmd[5]  = {0xD8, 0xD8, 0x66, 0x20, 0x20};
uint8_t const saveCmd[5]  = {0xD8, 0xD8, 0x66, 0x40, 0x40};
uint8_t const resetCmd[5] = {0xD8, 0xD8, 0x66, 0x03, 0x03};
uint8_t timTxBuf[5] = {0xD8, 0xD8, 0x66, 0x02, 0x02};
int16_t posError; // Sai So Vi Tri , Dung cho ham setHeight
uint16_t delayTimeCount = 0; // Dem thoi gian cong don sau moi lan chay
OPERATE_MODE_Typedef timotionMode = MODE_MANUAL;
static uint16_t currentHeight = 710;
static uint16_t setpointHeight = 1000;
static uint16_t heightCheck = 0;
static TIMOTION_STATE_Typedef curState = TIMOTION_STATE_FREE;
static TIMOTION_STATE_Typedef preState = TIMOTION_STATE_FREE;
SETHEIGHT_STATTUS_Typedef setHeightStatus = SET_HEIGHT_INCOMPLETE;
FlagStatus DMA_FLAG = RESET;
TimotionRxStruct_Typedef rxFrame;
TimotionTxStruct_Typedef txFrame;
static uint8_t subState = 0;
static uint8_t actionCount = 0;
static uint8_t saveCount = 0;
static uint8_t timboxStatus = STOPPED;
static WorkStatus_Typedef controlState = CONTROLLER_FREE;
static TimotionHandle_Typedef TimboxHandles = {
  &rxFrame,
  &txFrame,
  timotionDmaBuf,
  sizeof(timotionDmaBuf),
  longTimRxBuf,
  sizeof(longTimRxBuf),
  0
 
};

/*
Static function
*/
static void setControllerState(WorkStatus_Typedef _state){
   controlState = _state;  
}
static void setState(TIMOTION_STATE_Typedef _state){
  if(_state != curState){
      actionCount = 0;
      subState = 0; 
  }
  preState = curState;;
  curState = _state;
  TIM_SetCounter(TIM7,0);
  if(_state != TIMOTION_STATE_FREE){
     setControllerState(CONTROLLER_BUSY);
  }
  else{
    setControllerState(CONTROLLER_FREE);    
  }
}
static TIMOTION_STATE_Typedef getCurState(void){
  return curState;
}
static TIMOTION_STATE_Typedef getPreState(void){
  return preState;
}
static uint16_t getCurHeight(void){
  return currentHeight;
}
static void setCurHeight(uint16_t _height){
  currentHeight = _height;
}
static void setBoxStatus(uint8_t _status){
  timboxStatus = _status; 
}
static uint8_t getBoxStatus(void){
    return timboxStatus;
}
static void setMode(OPERATE_MODE_Typedef _mode){
    timotionMode = _mode;
   
}
static int16_t GetHeightFromLongBuffer(void){
  int16_t res = -1;
  uint8_t i = 0;
  /*
  Get Current Index of long buffer
  */
  uint16_t index = TimboxHandles.bufferIndex;
  /*
  Get requirement length of the RX packets
  */
  uint16_t length = TimboxFrameHeader.rxFrameLength - 1;
  /*
  Current Index must greater than requirement length
  */
  if (index >= length){
    while( i <= index - length){
      if (TimCommFrameCmd.isValidRxData(&TimboxHandles.rxBuf_ptr[i],TimboxFrameHeader.rxFrameLength)){ // check valid data befor copy to struct
           TimCommFrameCmd.RxArray2Struct(&TimboxHandles.rxBuf_ptr[i],TimboxFrameHeader.rxFrameLength,TimboxHandles.rxFrameObj); // copy to struct
           // Get information
           res = TimCommFrameCmd.GetRxData(TimboxHandles.rxFrameObj);
           setBoxStatus(TimCommFrameCmd.GetRxParam(TimboxHandles.rxFrameObj));
           i =  index + length; 
           TimboxHandles.bufferIndex = 0;
      } 
      else {
        i++;
      }
    }
  }
  return res;
    
}

uint8_t TIM_BOX_Init(void){
    #ifdef FIST_FLASH
        MY_FLASH_Write(sitPosAddr, 770);
        MY_FLASH_Write(standPosAddr, 1070);
        
    #endif
    sitPosVal = MY_FLASH_Read(sitPosAddr);
    standPosVal = MY_FLASH_Read(standPosAddr);
    GPIO_InitTypeDef GPIO_InitStructure;
    // Init for DETECT_PIN
    if(DETECT_PORT == GPIOA){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if(DETECT_PORT == GPIOB){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    else if(DETECT_PORT == GPIOC){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
    else if(DETECT_PORT == GPIOD){
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = DETECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DETECT_PORT, &GPIO_InitStructure);
    detectPinStatus = GPIO_ReadInputDataBit(DETECT_PORT,DETECT_PIN);
    //Init Up,Down, Sleep Pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = UP_PIN|DOWN_PIN|SLEEP_PIN;
    GPIO_Init(UP_DOWN_SLEEP_PORT,&GPIO_InitStructure);
    GPIO_ResetBits(UP_DOWN_SLEEP_PORT,UP_PIN|DOWN_PIN|SLEEP_PIN);
    
    //GPIO_SetBits(UP_DOWN_SLEEP_PORT,UP_PIN|DOWN_PIN|SLEEP_PIN);
    // Khoi tao handle quan ly UART
    if(TimBoxCmd.GetConnectStatus()){
      UARTx_Init(USART3,9600);
      UARTx_RxDmaInit(USART3, timotionDmaBuf, sizeof(timotionDmaBuf)); 
      UARTx_SetRxDmaIntPriority(USART3,NVIC_PriorityGroup_0,0,0); 
      //UARTx_SetTxDmaIntPriority(USART3,NVIC_PriorityGroup_0,15,10);         
      UARTx_Start(USART3);
      //TIM_HANDSET_Init();   
      TimHandset.Init();
      TIM_BOX_SleepOn();
      printf("TTimotion Control Box detect and init\n");
      return 1;
    }
    else{
        return 0;
    }
    
}
void TIM_BOX_Update(){
  /*
    Xu ly data khi DMA finish
  */
    if(DMA_FLAG == SET){
        int16_t h = GetHeightFromLongBuffer()*10; 
        
        
        if(h > 0) {
            // Update height to handset
          if(TIM_HS_GetDisplayPrio() == LOW_PRIO){
            TimHandset.DisplayHeight((uint8_t)(h/10));
          }
            
            if(h != TIM_BOX_GetHeight()) {
                // update height to contorller
                setCurHeight(h);
            }  
        }
        
       DMA_FLAG = RESET;
    }
    /*
      Tinh sai so khi vao che do set height
    */
    if(timotionMode == MODE_AUTO){
        posError = (int16_t)setpointHeight - (int16_t)currentHeight;
#ifdef DEBUG_TIMOTION
        printf("[I]-Error:%d\n",posError);
#endif
        if(posError <= -10){
            setHeightStatus = SET_HEIGHT_INCOMPLETE;
            TimBoxCmd.SetState(TIMOTION_STATE_DOWN);
        } else if (posError >= 10){                     
            setHeightStatus = SET_HEIGHT_INCOMPLETE;
            TimBoxCmd.SetState(TIMOTION_STATE_UP);
        } else{
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
            setHeightStatus = SET_HEIGHT_COMPLETE;
            TimBoxCmd.SetState(TIMOTION_STATE_RPI_STOP); 
            
        }
    } 
}
void TIM_BOX_Run(void){
    TIMOTION_STATE_Typedef temCurState = getCurState();
    TIMOTION_STATE_Typedef temPreState = getPreState();
    switch (temCurState){
    // For Handset
    case TIMOTION_STATE_HANDSET_UP:                                          // Handset Up
      controlState = CONTROLLER_BUSY;
        // Trong 3 chu ky lenh dau tien
      if (temCurState != temPreState){
        switch (subState){
        case 0: //  Lan Chay 1
          TimBoxCmd.Stop();
#ifdef DEBUG_TIMOTION
          printf("[I] - Move Up By Handset\n");
#endif
          subState++;
          break;
        case 1: // Lan Chay 2
          TimBoxCmd.Stop();
          subState++;
          break;
        case 2: // lan chay 3
          TimBoxCmd.Stop();
          subState = 0;
          setState(TIMOTION_STATE_HANDSET_UP);
          break;
        }    
      }
      // Trong nhung chu ky lenh tiep theo
      else{
        TimBoxCmd.Up();
      }
      break;
    case TIMOTION_STATE_HANDSET_DOWN:                               // Handset Down
      controlState = CONTROLLER_BUSY;
      // Trong 3 lchu ky lenh dau tien
      if(temCurState != temPreState){
        switch(subState){
        case 0:
          TimBoxCmd.Stop();
#ifdef DEBUG_TIMOTION
          printf("[I]-Move Down By Handset\n");
#endif
          subState++;
          
          break;
        case 1:
          TimBoxCmd.Stop();
          subState++;
          
          break;
        case 2:
          TimBoxCmd.Stop();
          subState = 0;
          setState(TIMOTION_STATE_HANDSET_DOWN);
          break;
        }
      }
      // Trong nhung chu ky lenh sau
      else {
        TimBoxCmd.Down();
      }
      break;
    case TIMOTION_STATE_HANDSET_STOP:                               //Handset Stop
      controlState = CONTROLLER_BUSY;
        /*
        Luu y nhung truong hop co the stop bang handset
        */
      if(actionCount < 3){
        actionCount++;
        if(actionCount == 1){
#ifdef DEBUG_TIMOTION
            printf("[I]-Stop by handset\n");
#endif        
        }
        TimBoxCmd.Stop();
      }
      else{
        actionCount = 0;
        setState(TIMOTION_STATE_FREE);
      }
      break;
    case TIMOTION_STATE_HANDSET_SAVE:                               //Handset Save
        controlState = CONTROLLER_BUSY;      
        TimBoxCmd.Save();
        printf("Save mode\n");
        break;
    case TIMOTION_STATE_HANDSET_SAVE2POS1:                          // Handset Save Pos1
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // Gui 2 Lenh Stop
            if (actionCount < 2 ){
                TimBoxCmd.Stop();
                actionCount++;
                if(actionCount == 1){
    #ifdef DEBUG_TIMOTION
                  printf("[I]-Entering Saving Mode\n");
    #endif
                }
            }
            else{
                actionCount = 0;
                subState = 1;
            }
            break;
        case 1: // Gui 2 Lenh Luu Vao Pos1
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.MovePos1();
            }
            else{
                actionCount = 0;
                subState = 2;
    #ifdef DEBUG_TIMOTION
                printf("[I]-Saved to Sit Pos\n");
    #endif
            }
            break;
        case 2: // Gui 2 Lenh Stop
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.Stop();
            }
            else{
                actionCount = 0;
                subState = 0;
                setState(TIMOTION_STATE_FREE);
    #ifdef DEBUG_TIMOTION
                printf("[I]-Saved to Sit Pos Finished\n");
    #endif
            }
            break;   
        }
        break;
    case TIMOTION_STATE_HANDSET_SAVE2POS2:                          // Handset Save Pos2
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // Gui 2 Lenh Stop
            if (actionCount < 2 ){
                TimBoxCmd.Stop();
                actionCount++;
            }
            else{
                actionCount = 0;
                subState = 1;
            }
            break;
        case 1: // Gui 2 Lenh Luu Vao Pos2
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.MovePos2();
            }
            else{
                actionCount = 0;
                subState = 2;
            }
            break;
        case 2: // Gui 2 Lenh Stop
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.Stop();
            }
            else{
                actionCount = 0;
                subState = 0;
                setState(TIMOTION_STATE_FREE);
            }
            break;   
        }        
        break;
    case TIMOTION_STATE_HANDSET_SAVE2POS3:                          // Handset Save Pos3
        controlState = CONTROLLER_BUSY;        
        switch(subState){
        case 0: // Gui 2 Lenh Stop
            if (actionCount < 2 ){
                TimBoxCmd.Stop();
                actionCount++;
            }
            else{
                actionCount = 0;
                subState = 1;
            }
            break;
        case 1: // Gui 2 Lenh Luu Vao Pos3
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.MovePos3();
            }
            else{
                actionCount = 0;
                subState = 2;
            }
            break;
        case 2: // Gui 2 Lenh Stop
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.Stop();
            }
            else{
                actionCount = 0;
                subState = 0;
                setState(TIMOTION_STATE_FREE);
            }
            break;   
        }      
        break;
    case TIMOTION_STATE_HANDSET_SAVE2POS4:                          // Handset Save Pos4
        controlState = CONTROLLER_BUSY;      
        switch(subState){
        case 0: // Gui 2 Lenh Stop
            if (actionCount < 2 ){
                TimBoxCmd.Stop();
                actionCount++;
            }
            else{
                actionCount = 0;
                subState = 1;
            }
            break;
        case 1: // Gui 2 Lenh Luu Vao Pos4
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.MovePos4();
            }
            else{
                actionCount = 0;
                subState = 2;
            }
            break;
        case 2: // Gui 2 Lenh Stop
            if (actionCount < 2){
                actionCount++;
                TimBoxCmd.Stop();
            }
            else{
                actionCount = 0;
                subState = 0;
                setState(TIMOTION_STATE_FREE);
            }
            break;   
        }      
        break;
        
    /*Save Pos Auto*/
    case  TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO:                    // Handset save Pos1 Auto
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos1
        if(actionCount < 6){
            TimBoxCmd.MovePos1();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Sit Pos Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }
      break;
    case  TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO:                    // Handset save Pos2 Auto    
      controlState = CONTROLLER_BUSY;            
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos2
        if(actionCount < 6){
            TimBoxCmd.MovePos2();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Stand Pos Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }      
      break;
    case  TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO:                    // Handset save Pos3 Auto
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos3
        if(actionCount < 6){
            TimBoxCmd.MovePos3();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Pos3 Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }      
      break;
    case  TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO:                    // Handset save Pos4 Auto
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos4
        if(actionCount < 6){
            TimBoxCmd.MovePos4();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Pos4 Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }      
      break;
    case TIMOTION_STATE_HANDSET_MOVE2POS1:                          // Handset Move Pos1
      controlState = CONTROLLER_BUSY;      
      switch (subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
          if (temCurState != temPreState){
            setState(TIMOTION_STATE_HANDSET_MOVE2POS1);
    #ifdef DEBUG_TIMOTION
            
            printf("[I]- Sitdown - Handset\n");
    #endif
          }
        }
        else{
          subState = 1;
          actionCount = 0;
        }
        break;
      case 1: // Gui Lenh SitDown
        if(actionCount < 10) actionCount++;
        if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
          printf("[I]Sit Height = %d\n",getCurHeight());
    #endif 
        }
        TimBoxCmd.MovePos1(); 
        break;
      }
      break;
    case TIMOTION_STATE_HANDSET_MOVE2POS2:                          // Handset Move Pos2
        controlState = CONTROLLER_BUSY;      
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS2);
        #ifdef DEBUG_TIMOTION
                
                printf("[I]- Stand Up - Handset\n");
        #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh Standup
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
        #ifdef DEBUG_TIMOTION
              printf("[I]Stand Height = %d\n",getCurHeight());
        #endif 
            }
            TimBoxCmd.MovePos2();
            break;
          case 2:
            break;
            
          }
        break;
    case TIMOTION_STATE_HANDSET_MOVE2POS3:                          // Handset Move Pos3
        controlState = CONTROLLER_BUSY;      
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS3);
    #ifdef DEBUG_TIMOTION
                
                printf("[I]- Move Pos3 - Handset\n");
    #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh move to Pos3
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
              printf("[I]Pos3 Height = %d\n",getCurHeight());
    #endif 
            }
            TimBoxCmd.MovePos3();
            
            break;
          case 2:
            break;
            
          }
        break;
    case TIMOTION_STATE_HANDSET_MOVE2POS4:                          // Handset Move Pos4
        controlState = CONTROLLER_BUSY;      
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS4);
    #ifdef DEBUG_TIMOTION
                
                printf("[I]- Move Pos4 - Handset\n");
    #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh move to pos4
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
              printf("[I]Pos4 Height = %d\n",getCurHeight());
    #endif 
            }
            TimBoxCmd.MovePos4();
            
            break;
          case 2:
            break;
            
          }
        break;
    case TIMOTION_STATE_HANDSET_RESET:                              // Handset Reset
      controlState = CONTROLLER_BUSY;      
      if (ledCount < 9){
        ledCount++;
        if (subState < 3){
            if(ledCount == 4)
            if (ledCount == 8){
                ledCount = 0;
            }        
        }
        if(subState == 3){
            if(ledCount == 1)
            if (ledCount == 2){
                ledCount = 0;
                
            }            
        }  
      }
      else{
        ledCount = 0;
      }
      switch(subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          actionCount = 0;
          subState = 1;
 #ifdef DEBUG_TIMOTION
          printf("[I]-Entering Reset Mode\n");
#endif         
        }
        break;
      case 1: // Gui Lenh Reset Cho den khi kich hoat mode
        if (getCurHeight() != 2550){
          TimBoxCmd.Reset();
        }
        else {
          actionCount = 0;
          subState = 2;
          TimBoxCmd.Stop();
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Mode Activated\n");
#endif
        }
        break; 
      case 2:// Gui 2 lenh Stop
        if (actionCount < 2 ){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          subState = 3;
          ledCount = 0;
          
          actionCount = 0;
#ifdef DEBUG_TIMOTION
          printf("[I]-Moving the Desk down\n");
#endif         
        }
        break;
      case 3: // 
        if (getCurHeight() == 2550){
          TimBoxCmd.Down();
        }
        else{
          TimBoxCmd.Stop();
          actionCount = 0;
          subState = 4;
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Finished, Current Height:%d\n",getCurHeight());
#endif
          //setState(TIMOTION_STATE_FREE);
          
        }
        break;
      case 4:
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset MCU\n");
#endif
        while(1);
      }
      break;
                
    // For Mic-Button Led
    case TIMOTION_STATE_MBL_UP:                                     // MBL Up
        controlState = CONTROLLER_BUSY;
      if (temCurState != temPreState){
        switch (subState){
        case 0:
          TimBoxCmd.Stop();
          subState = 1;
          break;
        case 1:
          TimBoxCmd.Stop();
          subState = 2;
          
          break;
        case 2:
          TimBoxCmd.Stop();
          subState = 0;
          setState(TIMOTION_STATE_MBL_UP);
#ifdef DEBUG_TIMOTION
          printf("[I]- Move Up By MBL\n");
#endif
          break;
        }
      }
      else {
        TimBoxCmd.Up();
      }
      break;
    case TIMOTION_STATE_MBL_DOWN:                                   // MBL Down
        controlState = CONTROLLER_BUSY;
      if (temCurState != temPreState){
        switch (subState){
        case 0:
          TimBoxCmd.Stop();
          subState = 1;
          break;
        case 1:
          TimBoxCmd.Stop();
          subState = 2;
          break;
        case 2:
          TimBoxCmd.Stop();
          subState = 0;
          setState(TIMOTION_STATE_MBL_DOWN);
#ifdef DEBUG_TIMOTION
          printf("[I]- Move down By MBL\n");
#endif
          break;
        }
      }
      else {
        TimBoxCmd.Down();
      }
      break;
    case TIMOTION_STATE_MBL_STOP:                                   // MBL Stop
        controlState = CONTROLLER_BUSY;
      if (actionCount < 5 ){
        actionCount++;
        TimBoxCmd.Stop();        
      }
      else{
        actionCount = 0;
        setState(TIMOTION_STATE_FREE); 
#ifdef DEBUG_TIMOTION
          printf("[I]- Stop By MBL\n");
#endif         
      }
        break;
    case TIMOTION_STATE_MBL_RESET:                                  // MBL Reset
        controlState = CONTROLLER_BUSY;
      if (ledCount < 9){
        ledCount++;
        if (subState < 3){
            if(ledCount == 4);
            if (ledCount == 8){
                ledCount = 0;
            }        
        }
        if(subState == 3){
            if(ledCount == 1);
            if (ledCount == 2){
                ledCount = 0;
                
            }            
        }  
      }
      else{
        ledCount = 0;
      }
      switch(subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          actionCount = 0;
          subState = 1;
 #ifdef DEBUG_TIMOTION
          printf("[I]-Entering Reset Mode\n");
#endif         
        }
        break;
      case 1: // Gui Lenh Reset Cho den khi kich hoat mode
        if (getCurHeight() != 2550){
          TimBoxCmd.Reset();
        }
        else {
          actionCount = 0;
          subState = 2;
          TimBoxCmd.Stop();
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Mode Activated\n");
#endif
        }
        break; 
      case 2:// Gui 2 lenh Stop
        if (actionCount < 2 ){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          subState = 3;
          ledCount = 0;
          actionCount = 0;
#ifdef DEBUG_TIMOTION
          printf("[I]-Moving the Desk down\n");
#endif         
        }
        break;
      case 3: // 
        if (getCurHeight() == 2550){
          TimBoxCmd.Down();
        }
        else{
          TimBoxCmd.Stop();
          actionCount = 0;
          subState = 4;
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Finished, Current Height:%d\n",getCurHeight());
#endif
          //setState(TIMOTION_STATE_FREE);
          
        }
        break;
      case 4:
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset MCU\n");
#endif       
        while(1);
      }      
      break;
    case TIMOTION_STATE_MBL_SIT:                                    // MBL Sit
      controlState = CONTROLLER_BUSY;      
      switch (subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
          if (temCurState != temPreState){
            setState(TIMOTION_STATE_MBL_SIT);
#ifdef DEBUG_TIMOTION
            
            printf("[I]- Sitdown - MBL\n");
#endif
          }
        }
        else{
          subState = 1;
          actionCount = 0;
        }
        break;
      case 1: // Gui Lenh SitDown
        if(actionCount < 10) actionCount++;
        if (actionCount == 4){
#ifdef DEBUG_TIMOTION
          printf("[I]Sit Height = %d\n",getCurHeight());
#endif 
        }
        TimBoxCmd.MovePos1();  
        break;
      }      
      break;
    case TIMOTION_STATE_MBL_STAND:                                  // MBL Stand
      controlState = CONTROLLER_BUSY;      
      switch (subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
          if (temCurState != temPreState){
            setState(TIMOTION_STATE_MBL_STAND);
#ifdef DEBUG_TIMOTION
            
            printf("[I]- Standup - MBL\n");
#endif
          }
        }
        else{
          subState = 1;
          actionCount = 0;
        }
        break;
      case 1: // Gui Lenh Stand up
        if(actionCount < 10) actionCount++;
        if (actionCount == 4){
#ifdef DEBUG_TIMOTION
          printf("[I]Stand Height = %d\n",getCurHeight());
#endif 
        }
        TimBoxCmd.MovePos2();  
        break;
      }      
      break;
    case TIMOTION_STATE_MBL_SAVE_SIT:                               // MBL Save Sit
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos1
        if(actionCount < 6){
            TimBoxCmd.MovePos1();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Sit Pos Finished\n");
#endif            
        }        
        break;
        
      }
      break;
    case TIMOTION_STATE_MBL_SAVE_STAND:                             // MBL Save Stand
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos2
        if(actionCount < 6){
            TimBoxCmd.MovePos2();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Stand Pos Finished\n");
#endif            
            
        }        
        break;
        
      }
      break;
/*
For RPI      
*/
    case TIMOTION_STATE_RPI_UP:                                     // RPI Up
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // gui 3 lenh stop
          if(actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
          }
          else{
            actionCount = 0;
            subState = 1;
          }
          break;
        case 1:
            TimBoxCmd.Up();          
            break;
        }      
        break;
    case TIMOTION_STATE_RPI_DOWN:                                   // RPI Down
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // gui 3 lenh stop
          if(actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
          }
          else{
            actionCount = 0;
            subState = 1;
          }
          break;
        case 1:
            TimBoxCmd.Down();          
            break;
        }      
        break;
    case TIMOTION_STATE_RPI_STOP:                                   // RPI Stop
        controlState = CONTROLLER_BUSY;
      if (actionCount < 5 ){
        actionCount++;
        TimBoxCmd.Stop();        
      }
      else{
        actionCount = 0;
        setState(TIMOTION_STATE_FREE); 
#ifdef DEBUG_TIMOTION
          printf("[I]- Stop By RPI\n");
#endif         
      }      
      break;
    case TIMOTION_STATE_RPI_RESET:                                  // RPI Reset
      controlState = CONTROLLER_BUSY;      
      if (ledCount < 9){
        ledCount++;
        if (subState < 3){
            if(ledCount == 4);
            if (ledCount == 8){
                ledCount = 0;
            }        
        }
        if(subState == 3){
            if(ledCount == 1);
            if (ledCount == 2){
                ledCount = 0;
                
            }            
        }  
      }
      else{
        ledCount = 0;
      }
      switch(subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          actionCount = 0;
          subState = 1;
 #ifdef DEBUG_TIMOTION
          printf("[I]-Entering Reset Mode\n");
#endif         
        }
        break;
      case 1: // Gui Lenh Reset Cho den khi kich hoat mode
        if (getCurHeight() != 2550){
          TimBoxCmd.Reset();
        }
        else {
          actionCount = 0;
          subState = 2;
          TimBoxCmd.Stop();
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Mode Activated\n");
#endif
        }
        break; 
      case 2:// Gui 2 lenh Stop
        if (actionCount < 2 ){
          actionCount++;
          TimBoxCmd.Stop();
        }
        else {
          subState = 3;
          ledCount = 0;
          actionCount = 0;
#ifdef DEBUG_TIMOTION
          printf("[I]-Moving the Desk down\n");
#endif         
        }
        break;
      case 3: // 
        if (getCurHeight() == 2550){
          TimBoxCmd.Down();
        }
        else{
          TimBoxCmd.Stop();
          actionCount = 0;
          subState = 4;
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset Finished, Current Height:%d\n",getCurHeight());
#endif
          //setState(TIMOTION_STATE_FREE);
          
        }
        break;
      case 4:
#ifdef DEBUG_TIMOTION
          printf("[I]-Reset MCU\n");
#endif        
        while(1);
      }      
      break;
    case TIMOTION_STATE_RPI_SAVE_POS1:                              // RPI Save Pos1
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos1
        if(actionCount < 6){
            TimBoxCmd.MovePos1();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Sit Pos Finished\n");
#endif            
        }        
        break;
        
      }
      break;
    case TIMOTION_STATE_RPI_SAVE_POS2:                              // RPI Save Pos2                          
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos2
        if(actionCount < 6){
            TimBoxCmd.MovePos2();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Stand Pos Finished\n");
#endif            
            
        }        
        break;
        
      }      
      break;
    case TIMOTION_STATE_RPI_SAVE_POS3:                              // RPI Save Pos3        
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos3
        if(actionCount < 6){
            TimBoxCmd.MovePos3();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Pos3 Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }      
      break;
    case TIMOTION_STATE_RPI_SAVE_POS4:                              // RPI Save Pos4
      controlState = CONTROLLER_BUSY;      
      switch(subState){
      case 0: // gui 3 lenh Stop
        if (actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
        }
        else {
            actionCount = 0;
            subState = 1;
#ifdef DEBUG_TIMOTION
            printf("[I]-Entering Save Mode\n");
#endif
        }
        break;
      case 1: // Gui lenh Save cho den khi kich hoat
        if(getBoxStatus() != SAVE)  {
            TimBoxCmd.Save();
        }
        else{
            saveCount++;
            if(saveCount >= 3){
                saveCount = 0;
                actionCount = 0;
                subState = 2;
#ifdef DEBUG_TIMOTION
                printf("[I]-Save Mode Activated\n");
#endif                
            }
        }
        break;
      case 2: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 3;
        }
        break;
      case 3: // Gui 3 lenh Pos4
        if(actionCount < 6){
            TimBoxCmd.MovePos4();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 4;
#ifdef DEBUG_TIMOTION
            printf("[I]-Save Pos4 Finished\n");
#endif             
        }        
        break;
      case 4: // Gui 3 lenh Stop
        if(actionCount < 3){
            TimBoxCmd.Stop();
            actionCount++;
        }
        else{
            actionCount = 0;
            subState = 0;
            setState(TIMOTION_STATE_FREE);
            
        }        
        break;
        
      }      
      break;
    case TIMOTION_STATE_RPI_MOVE_POS1:                              // RPI Move Pos1
      controlState = CONTROLLER_BUSY;      
      switch (subState){
      case 0: // Gui 3 lenh Stop
        if (actionCount < 3){
          actionCount++;
          TimBoxCmd.Stop();
          if (temCurState != temPreState){
            setState(TIMOTION_STATE_HANDSET_MOVE2POS1);
#ifdef DEBUG_TIMOTION
            
            printf("[I]- Sitdown - RPI\n");
#endif
          }
        }
        else{
          subState = 1;
          actionCount = 0;
        }
        break;
      case 1: // Gui Lenh SitDown
        if(actionCount < 10) actionCount++;
        if (actionCount == 4){
#ifdef DEBUG_TIMOTION
          printf("[I]Sit Height = %d\n",getCurHeight());
#endif 
        }
        TimBoxCmd.MovePos1(); 
        
        break;
      }
      break;
    case TIMOTION_STATE_RPI_MOVE_POS2:                              // RPI Move Pos2    
        controlState = CONTROLLER_BUSY;      
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS2);
    #ifdef DEBUG_TIMOTION
                
                printf("[I]- Stand Up - RPI\n");
    #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh Standup
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
              printf("[I]Stand Height = %d\n",getCurHeight());
    #endif 
            }
            TimBoxCmd.MovePos2();
            
            break;
          case 2:
            break;
            
          }
        break;
    case TIMOTION_STATE_RPI_MOVE_POS3:                              // RPI Move Pos3
        controlState = CONTROLLER_BUSY;
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS3);
    #ifdef DEBUG_TIMOTION
                
                printf("[I]- Move Pos3 - RPI\n");
    #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh move to Pos3
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
              printf("[I]Pos3 Height = %d\n",getCurHeight());
    #endif 
            }
            TimBoxCmd.MovePos3();
            
            break;
          case 2:
            break;
            
          }
        break;
    case TIMOTION_STATE_RPI_MOVE_POS4:                              // RPI Move Pos4    
        controlState = CONTROLLER_BUSY;      
        switch (subState){
          case 0: // Gui 3 lenh Stop
            if (actionCount < 3){
              actionCount++;
              TimBoxCmd.Stop();
              if (temCurState != temPreState){
                setState(TIMOTION_STATE_HANDSET_MOVE2POS4);
    #ifdef DEBUG_TIMOTION
                
                printf("[I]- Move Pos4 - RPI\n");
    #endif
              }
            }
            else{
              subState = 1;
              actionCount = 0;
            }
            break;
          case 1: // Gui Lenh move to pos4
            if(actionCount < 10) actionCount++;
            if (actionCount == 4){
    #ifdef DEBUG_TIMOTION
              printf("[I]Pos4 Height = %d\n",getCurHeight());
    #endif 
            }
            TimBoxCmd.MovePos4();
            
            break;
          case 2:
            break;
            
          }
        break;      
      break;      
    case TIMOTION_STATE_RPI_MIC:                                    // RPI Mic
        controlState = CONTROLLER_BUSY;        
        
        break;
    case TIMOTION_STATE_FREE:                                       // Free    
        actionCount = 0;
        subState = 0;
        ledCount = 0;
        
        TimBoxCmd.Stop();
        if (TimBoxCmd.GetConnectStatus() != detectPinStatus){
            detectPinStatus  = TimBoxCmd.GetConnectStatus();
            if(  detectPinStatus == 0 ){
#ifdef DEBUG_TIMOTION
              printf("[E]-Timotion Control Box Disconnected\n");
#endif
            }
            else{
#ifdef DEBUG_TIMOTION
              printf("[E]-Timotion Control Box Connected\n");
#endif                
            }
        }
        controlState = CONTROLLER_FREE;
        break;
    case TIMOTION_STATE_UP: 
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // gui 3 lenh stop
          if(actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
            
          }
          else{
            actionCount = 0;
            subState = 1;
          }
          break;
        case 1:
            TimBoxCmd.Up();          
            break;
        }      
        break;
    case TIMOTION_STATE_DOWN:
        controlState = CONTROLLER_BUSY;
        switch(subState){
        case 0: // gui 3 lenh stop
          if(actionCount < 3){
            actionCount++;
            TimBoxCmd.Stop();
            
          }
          else{
            actionCount = 0;
            subState = 1;
          }
          break;
        case 1:
            TimBoxCmd.Down();          
            break;
        }      
      break;
    }

}

void TIM_BOX_PutBytes(uint8_t * _data, uint8_t length){
    
  UARTx_PutBytesDMA(USART3,_data,length);
  
  //UARTx_PutBytes(USART3,_data,length);
}

void TIM_BOX_Freset(void){
  TIM_BOX_PutBytes( (uint8_t *)&resetCmd[0],sizeof(resetCmd));  
}
uint8_t TIM_BOX_GetDetectStatus(void){
	//return GPIO_ReadInputDataBit(DETECT_PORT,DETECT_PIN);
  return 1;
}

void TIM_BOX_MoveUp(void){
	
	GPIO_SetBits(UP_DOWN_SLEEP_PORT,UP_PIN);
	GPIO_ResetBits(UP_DOWN_SLEEP_PORT,DOWN_PIN);
}
void TIM_BOX_MoveDown(void){
	GPIO_SetBits(UP_DOWN_SLEEP_PORT,DOWN_PIN);
	GPIO_ResetBits(UP_DOWN_SLEEP_PORT,UP_PIN);
}
void TIM_BOX_Stop(void){
	GPIO_ResetBits(UP_DOWN_SLEEP_PORT,DOWN_PIN);
	GPIO_ResetBits(UP_DOWN_SLEEP_PORT,UP_PIN);
}

int8_t  TIM_BOX_GetPackageBeginIndex(uint8_t *data, uint8_t length){
  int8_t ret = -1;
  if (length > 0){
    for (int i = 0; i < length-1; i++){
       if((data[i] == TimboxFrameHeader.RxStartByte1)&&(data[i+1] == TimboxFrameHeader.RxStartByte2)) {
          ret = i;
          i = length;
       }
       
    }  // End for
    
  } // end if length > 0
  return ret;
  
}
uint8_t TIM_BOX_GetHeightFromPackage(uint8_t *data, uint8_t length){
    int8_t ind =   TIM_BOX_GetPackageBeginIndex(data, length);
    uint8_t val[2];
    uint8_t ret = 0;
    if ((ind >= 0)&&(data[ind + 2] == data[ind+3])){
       val[0] =   data[ind + 4];
       val[1] = data[ind + 5];
       if (val[0] == val[1])
        ret = val[0];      
    }
    return ret;
}
void TIM_BOX_SaveCurrentHeightToPos1(void){

}
void TIM_BOX_SaveCurrentHeightToPos2(void){

}
void TIM_BOX_SaveCurrentHeightToPos3(void){

}
void TIM_BOX_SaveCurrentHeightToPos4(void){

}
void TIM_BOX_MoveToPos1(void){
/*
   uint16_t h;
    if (~EE_ReadVariable(VirtAddVarTab[0],&h)){
       TIM_BOX_SetHeight(h); 
    } 
    */
    TIM_BOX_MovePos1Api();

}
void TIM_BOX_MoveToPos2(void){
/*
   uint16_t h;
    if (~EE_ReadVariable(VirtAddVarTab[1],&h)){
       TIM_BOX_SetHeight(h); 
    }
    */
    TIM_BOX_MovePos2Api();
}
void TIM_BOX_MoveToPos3(void){
/*
   uint16_t h;
    if (~EE_ReadVariable(VirtAddVarTab[2],&h)){
       TIM_BOX_SetHeight(h); 
    }
    */
    TIM_BOX_MovePos3Api();
}
void TIM_BOX_MoveToPos4(void){
/*
    uint16_t h;
    if (~EE_ReadVariable(VirtAddVarTab[3],&h)){
       TIM_BOX_SetHeight(h); 
    }
    */
    TIM_BOX_MovePos4Api();
}
void TIM_BOX_SetHeight(uint16_t height_mm){
  if ((height_mm >= minHeight)&&(height_mm <= maxHeight)){
     setpointHeight =  height_mm  ;
     TimBoxCmd.SetOperationMode(MODE_AUTO);
  }
  else {
#ifdef DEBUG_TIMOTION
    printf("-E- Height value out of range \n");
#endif
  }

}

uint16_t TIM_BOX_GetHeight(void){
    return  currentHeight;
}
void TIM_BOX_SetState(TIMOTION_STATE_Typedef _state){
    setState(_state);
}
TIMOTION_STATE_Typedef  TIM_BOX_GetCurState(void){
    return   curState;
}
TIMOTION_STATE_Typedef TIM_BOX_GetPreState(void){
    return preState;
}
void TIM_BOX_SetMode(OPERATE_MODE_Typedef _mode){
   setMode(_mode);
}
OPERATE_MODE_Typedef TIM_BOX_GetMode(void){
  return  timotionMode;
}
void TIM_BOX_UpdateCurrentHeight(uint16_t height_mm){
    if((height_mm > 600) &&(height_mm < 1500)){
        currentHeight = height_mm;
    }
}

void TIMOTION_DmaCallback(DMA_RX_FLAG_Typedef flag){
  switch (flag){
  case HALF_COMPLETED:
      if(TimboxHandles.bufferIndex < (TimboxHandles.rxBufLength - TimboxHandles.dmaBuffLength)){
        memcpy(&TimboxHandles.rxBuf_ptr[TimboxHandles.bufferIndex],TimboxHandles.dmaBuff_ptr,TimboxHandles.dmaBuffLength/2);
        TimboxHandles.bufferIndex += TimboxHandles.dmaBuffLength/2;
      }
      else{
        TimboxHandles.bufferIndex = 0;
        memset(TimboxHandles.rxBuf_ptr, 0 , TimboxHandles.rxBufLength);
      }
    break;
  case FULL_COMPLETED:
    if(TimboxHandles.bufferIndex < (TimboxHandles.rxBufLength - TimboxHandles.dmaBuffLength/2)){
        memcpy(&TimboxHandles.rxBuf_ptr[TimboxHandles.bufferIndex],&TimboxHandles.dmaBuff_ptr[TimboxHandles.dmaBuffLength/2],TimboxHandles.dmaBuffLength/2);
        TimboxHandles.bufferIndex += TimboxHandles.dmaBuffLength/2;
    }
    else{
      TimboxHandles.bufferIndex = 0;
      memset(TimboxHandles.rxBuf_ptr, 0 , TimboxHandles.rxBufLength);  
    }
    break;
  }
  
  //TIMOTION_rxCpy();
  DMA_FLAG = SET;
}
void TIMOTION_rxCpy(void){
  if(bufIndex <= timRxBufLength - dmaBufLength){
    memcpy(&longTimRxBuf[bufIndex],timotionDmaBuf,sizeof(timotionDmaBuf));
    bufIndex += dmaBufLength; 
  }
  else{
    bufIndex = 0;
    memset(longTimRxBuf,0,timRxBufLength);
  }  
}


void TIM_BOX_HsUp(void){
    //TIM_BOX_UpApi();
    TimBoxCmd.Up();
}
void TIM_BOX_HsDown(void){
    //TIM_BOX_DownApi();
    TimBoxCmd.Down();
}
void TIM_BOX_HsStop(void){
    //TIM_BOX_StopApi();
    TimBoxCmd.Stop();
}
void TIM_BOX_HsReset(void){
  //TIM_BOX_ResetApi();
  TimBoxCmd.Reset();
}
// APIs
void TIM_BOX_UpApi(void){
   TIM_BOX_PutBytes((uint8_t *)&upCmd[0],sizeof(upCmd)); 
}
void TIM_BOX_DownApi(void){
   TIM_BOX_PutBytes((uint8_t *)&downCmd[0],sizeof(downCmd));
}
void TIM_BOX_StopApi(void){
   TIM_BOX_PutBytes((uint8_t *)&stopCmd[0],sizeof(stopCmd));
}
void  TIM_BOX_ResetApi(void){
   TIM_BOX_PutBytes((uint8_t *)&resetCmd[0],sizeof(resetCmd));

}
void  TIM_BOX_MovePos1Api(void){
    TIM_BOX_PutBytes((uint8_t *)&pos1Cmd[0],sizeof(pos1Cmd));
}
void  TIM_BOX_MovePos2Api(void){
   TIM_BOX_PutBytes((uint8_t *)&pos2Cmd[0],sizeof(pos2Cmd));
}
void  TIM_BOX_MovePos3Api(void){
   TIM_BOX_PutBytes((uint8_t *)&pos3Cmd[0],sizeof(pos3Cmd));
}
void  TIM_BOX_MovePos4Api(void){
   TIM_BOX_PutBytes((uint8_t *)&pos4Cmd[0],sizeof(pos4Cmd));
}
void  TIM_BOX_SaveApi(void){
   TIM_BOX_PutBytes((uint8_t *)&saveCmd[0],sizeof(saveCmd)); 
}

void TIM_BOX_MblUp(void){
    //TIM_BOX_UpApi();
    TimBoxCmd.Up();
    
}
void TIM_BOX_MblDown(void){
   //TIM_BOX_DownApi();
   TimBoxCmd.Down();
   //BUTTON_LED_DownMblLed();
}
void TIM_BOX_MblStop(void){
   //TIM_BOX_StopApi();
   //TIM_BOX_StopApi();
   TimBoxCmd.Stop();
   
}
void TIM_BOX_MblMic(void){
}
// state timeout callback
void TIM_BOX_TimeoutFcn(void){
    //TIM_BOX_SetState(TIMOTION_STATE_TIMEOUT);
}
void TIM_BOX_checkHeightCallback(void){
    TIMOTION_STATE_Typedef temState = getCurState();
    if ((temState == TIMOTION_STATE_HANDSET_UP)
        ||(temState == TIMOTION_STATE_HANDSET_DOWN)
        ||(temState == TIMOTION_STATE_HANDSET_MOVE2POS1)
        ||(temState == TIMOTION_STATE_HANDSET_MOVE2POS2)
        ||(temState == TIMOTION_STATE_HANDSET_MOVE2POS3)
        ||(temState == TIMOTION_STATE_HANDSET_MOVE2POS4)
        ||(temState == TIMOTION_STATE_MBL_UP)
        ||(temState == TIMOTION_STATE_MBL_DOWN)
        ||(temState == TIMOTION_STATE_MBL_SIT)
        ||(temState == TIMOTION_STATE_MBL_STAND)
        ||(temState == TIMOTION_STATE_RPI_UP)
        ||(temState == TIMOTION_STATE_RPI_DOWN)
        ||(temState == TIMOTION_STATE_RPI_MOVE_POS1)
        ||(temState == TIMOTION_STATE_RPI_MOVE_POS2)
        ||(temState == TIMOTION_STATE_RPI_MOVE_POS3)
          ||(temState == TIMOTION_STATE_RPI_MOVE_POS4)){
            uint16_t h = getCurHeight();
            if(heightCheck == h){
                setState(TIMOTION_STATE_FREE);
                printf("[I] - No Moment Detected in 3 seconds\n");
            }
            else{
               heightCheck = h; 
            }
            
      }  
}
void  TIM_BOX_SaveSitPos(uint16_t height_mm){
    MY_FLASH_Write(sitPosAddr,height_mm);
}
void  TIM_BOX_SaveStandPos(uint16_t height_mm){
    MY_FLASH_Write(standPosAddr, height_mm);
}
void  TIM_BOX_SleepOn(void){
  GPIO_ResetBits(UP_DOWN_SLEEP_PORT,SLEEP_PIN);
}
void  TIM_BOX_SleepOff(void){
  GPIO_ResetBits(UP_DOWN_SLEEP_PORT,SLEEP_PIN);
}
uint16_t TIM_BOX_ReadSitPosHeight(void){
    return MY_FLASH_Read(sitPosAddr);
}
uint16_t TIM_BOX_ReadStandPosHeight(void){
    return MY_FLASH_Read(standPosAddr);
}
WorkStatus_Typedef TIM_BOX_GetControllerState(void){
    return controlState;
}

uint8_t TIMBOX_GetPacketStatus(void){
    return timboxStatus;
}


