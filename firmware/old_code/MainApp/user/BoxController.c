#include "BoxController.h"
uint8_t rx5Buf[20];
uint8_t up_status = 0;
uint8_t timeCheck = 0;
uint8_t checkInit = 0;

void BOX_CONTROLLER_Init(void){
  while(checkInit == 0){
    if (TIM_BOX_Init()){
       checkInit = 1; 
    }
  } 
}
void BOX_CONTROLLER_Update(void){
    if(TimBoxCmd.GetConnectStatus()){
        TIM_BOX_Update();
    }
    else{
        while(1);
    }
}

void BOX_CONTROLLER_Run(void){
    // Timotion Control Box is available
    if(TimBoxCmd.GetConnectStatus()){
        TIM_BOX_Run();
    }

}
void BOX_CONTROLLER_SetState( BOX_STATE_Typedef state,unsigned short data){
    TIMOTION_STATE_Typedef curState = TimBoxCmd.GetCurrentState();
    TIMOTION_STATE_Typedef preState = TimBoxCmd.GetPreviousState();
    switch (state){
    case BOX_STATE_UP:
        if(TimBoxCmd.GetConnectStatus()){
            TimBoxCmd.SetState(TIMOTION_STATE_RPI_UP);
        }

            
        break;
    case BOX_STATE_DOWN:
        if(TimBoxCmd.GetConnectStatus()) {
            TimBoxCmd.SetState(TIMOTION_STATE_MBL_DOWN);
        }   
        break;
    case BOX_STATE_STOP:
        if(TimBoxCmd.GetConnectStatus()){           
           TimBoxCmd.SetOperationMode(MODE_MANUAL);
           TimBoxCmd.SetState(TIMOTION_STATE_MBL_STOP);
        }
        break;
    case BOX_STATE_MIC:
         if(TimBoxCmd.GetConnectStatus())
         { 
            TimBoxCmd.SetState(TIMOTION_STATE_MBL_MIC);
         }
        break;
    case BOX_STATE_FREE:
        break;
    case BOX_STATE_SAVE_SIT:
        if(TimBoxCmd.GetConnectStatus()){
            TimBoxCmd.SetState(TIMOTION_STATE_MBL_SAVE_SIT);
        }
        break;
    case BOX_STATE_SAVE_STAND:
        if(TimBoxCmd.GetConnectStatus()){
            TimBoxCmd.SetState(TIMOTION_STATE_MBL_SAVE_STAND);
        }
        break;
    case BOX_STATE_SIT:
        if(TimBoxCmd.GetConnectStatus()){
            TimBoxCmd.SetState(TIMOTION_STATE_MBL_SIT);
        }
        break;
    case BOX_STATE_STAND:
        if(TimBoxCmd.GetConnectStatus()){
             TimBoxCmd.SetState(TIMOTION_STATE_MBL_STAND);
        }
        break;
    case BOX_STATE_RESET:
        if(TimBoxCmd.GetConnectStatus()){
             TimBoxCmd.SetState(TIMOTION_STATE_RPI_RESET);
        }
        break;
    }
}
void BOX_CONTROLLER_SetHeight(uint16_t height_mm){
    if(TimBoxCmd.GetConnectStatus()){
        TimBoxCmd.SetSetpointHeight(height_mm);
    }
}
uint16_t BOX_CONTROLLER_GetHeight(void){
    uint16_t h;
    if(TimBoxCmd.GetConnectStatus()){
      h=TimBoxCmd.GetCurrentHeight();
    } 
    return h;
}
void BOX_CONTROLLER_SetMode(OPERATE_MODE_Typedef _mode){
    if (TimBoxCmd.GetConnectStatus()){
        TimBoxCmd.SetOperationMode(_mode);
    }
}
