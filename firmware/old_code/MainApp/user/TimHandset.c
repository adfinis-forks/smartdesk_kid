#include "AddonBoard.h"
#include "TimBox.h"
#define  TIM_HANDSET_RX_HEADER 0xD8
static uint8_t handsetRxBuf[5];
static uint8_t handsetLongRxBuf[28];
static uint8_t timHansdsetBufIndex = 0;
static uint8_t handsetDmaBufLength = sizeof(handsetRxBuf);
static uint8_t handsetRxBufLength = sizeof(handsetLongRxBuf);
static uint8_t hsTxBuf[6] = {0x98, 0x98 , 0x03 , 0x03 , 0x00 , 0x00};
static FlagStatus HANDSET_DMA_FLAG = RESET;
static TIM_HANDSET_DISPLAY_PRIO_Typedef displayPrio = LOW_PRIO;
static TIM_HANDSET_CMD_Typedef curCmd = CMD_NULL;

void TIM_HANDSET_putBytes(uint8_t *data, uint8_t length){
    //Put data in non blocking mode
    UARTx_PutBytesDMA(USART1,data,length);
}
void TIM_HANDSET_Init(void){
   UARTx_Init(USART1,9600);
   UARTx_RxDmaInit(USART1, handsetRxBuf, sizeof(handsetRxBuf)); 
   UARTx_SetRxDmaIntPriority(USART1,NVIC_PriorityGroup_0,1,0); 
   //UARTx_SetTxDmaIntPriority(USART1,NVIC_PriorityGroup_0,14,0);         
   UARTx_Start(USART1); 
}
void TIM_HANDSET_Update(void){
  //
   if(HANDSET_DMA_FLAG == SET){
    TIM_HANDSET_CMD_Typedef cmd =  TIM_HANDSET_GetCommandFromDataPackage();
    //printf("CMD=%x\n",cmd);
    TIMOTION_STATE_Typedef ccurState = TimBoxCmd.GetCurrentState();
    TIMOTION_STATE_Typedef preState = TimBoxCmd.GetPreviousState();
    if ((cmd != CMD_NULL)&&(cmd != curCmd)){
      curCmd = cmd;
      switch (cmd){
        case CMD_UP:
            //
          if (ccurState != TIMOTION_STATE_HANDSET_RESET){
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_UP);
          }
            
            
            break;
        case CMD_DOWN:
            if (ccurState != TIMOTION_STATE_HANDSET_RESET){
                TimBoxCmd.SetOperationMode(MODE_MANUAL);
                TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_DOWN);            
            }
            
            break;
        case CMD_STOP: 
                                    
            if ((ccurState == TIMOTION_STATE_HANDSET_UP)
                ||(ccurState == TIMOTION_STATE_HANDSET_DOWN)
                ||(ccurState == TIMOTION_STATE_HANDSET_RESET)
                ||(ccurState == TIMOTION_STATE_HANDSET_SAVE)){
                TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_STOP);
            }
            break;
        case CMD_RESET:
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_RESET);            
            break;
        case CMD_SAVE:

            if ((ccurState != TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO)
                   &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO)){
                        TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE);       
                   }
            break;
        case CMD_POS1:
          if(ccurState == TIMOTION_STATE_HANDSET_SAVE ){
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS1);
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
          }
          else{
             if ((ccurState != TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO)){
                    TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_MOVE2POS1);
                    TimBoxCmd.SetOperationMode(MODE_MANUAL);       
                   }
            
          }
            break;
        case CMD_POS2:
          if(ccurState == TIMOTION_STATE_HANDSET_SAVE ){
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS2);
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
          }
          else{
             if ((ccurState != TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO)){
                    TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_MOVE2POS2);
                    TimBoxCmd.SetOperationMode(MODE_MANUAL);       
                   }            

          }
            break;
        case CMD_POS3:
          if(ccurState == TIMOTION_STATE_HANDSET_SAVE ){
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS3);
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
          }
          else{
             if ((ccurState != TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO)){
                    TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_MOVE2POS3);
                    TimBoxCmd.SetOperationMode(MODE_MANUAL);       
                   }            

          }
            break;
        case CMD_POS4:
          if(ccurState == TIMOTION_STATE_HANDSET_SAVE ){
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS4);
            TimBoxCmd.SetOperationMode(MODE_MANUAL);
          }
          else{
             if ((ccurState != TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO)
                 &&(ccurState != TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO)){
                    TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_MOVE2POS4);
                    TimBoxCmd.SetOperationMode(MODE_MANUAL);       
                   }            

          }
            break;
        case CMD_SAVE_POS1:
            TIM_HANDSET_DisplayHeight(11);
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS1_AUTO);
          break;
        case CMD_SAVE_POS2:
            TIM_HANDSET_DisplayHeight(22);
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS2_AUTO);
            break;
        case CMD_SAVE_POS3:
            TIM_HANDSET_DisplayHeight(33);
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS3_AUTO);
            break;
        case CMD_SAVE_POS4:
            TIM_HANDSET_DisplayHeight(44);
            TimBoxCmd.SetState(TIMOTION_STATE_HANDSET_SAVE2POS4_AUTO);
            break;
        case CMD_NULL:
            break;
            
      }
      }
     HANDSET_DMA_FLAG = RESET;
   }
}
void TIM_HANDSET_DisplayHeight(uint8_t height){
    hsTxBuf[2] = TimBoxCmd.GetPacketStatus();
    hsTxBuf[3] = TimBoxCmd.GetPacketStatus();
    hsTxBuf[4] = height;
    hsTxBuf[5] = height;
    TIM_HANDSET_putBytes(&hsTxBuf[0],6);
}

void TIM_HANDSET_Run(){
    

}
void TIM_HS_SetDisplayPrio(TIM_HANDSET_DISPLAY_PRIO_Typedef _prio){
    displayPrio = _prio;
}
TIM_HANDSET_DISPLAY_PRIO_Typedef TIM_HS_GetDisplayPrio(void){
    return displayPrio;
}
TIM_HANDSET_CMD_Typedef TIM_HANDSET_GetCommandFromDataPackage(void){
      TIM_HANDSET_CMD_Typedef ret = CMD_NULL;
      int i = 0;
       while(i < (handsetRxBufLength - 5)){
         if((handsetLongRxBuf[i] == TIM_HANDSET_RX_HEADER)&&(handsetLongRxBuf[i+1] == TIM_HANDSET_RX_HEADER)){
             if(handsetLongRxBuf[i+3] == handsetLongRxBuf[i+4]){
                ret = (TIM_HANDSET_CMD_Typedef)handsetLongRxBuf[i+4];
                i += 5;
             }
         }
         else i += 1;
       }
      return ret;
}
void TIM_HANDSET_DmaCallback(void){
    TIM_HANDSET_rxCpy();
    HANDSET_DMA_FLAG = SET;
}
void TIM_HANDSET_rxCpy(void){
  if(timHansdsetBufIndex <= handsetRxBufLength - handsetDmaBufLength){
    memcpy(handsetLongRxBuf + timHansdsetBufIndex,handsetRxBuf,sizeof(handsetRxBuf));
    timHansdsetBufIndex += handsetDmaBufLength;
  }
  else{
    timHansdsetBufIndex = 0;
    memset(handsetLongRxBuf,0,handsetRxBufLength);
  }
}
