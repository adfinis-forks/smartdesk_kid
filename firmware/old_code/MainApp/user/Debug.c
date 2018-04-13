#include "Debug.h"
#include "BoxController.h"
#include "MicroController/eeprom.h"


USART_HandlesTerminatorTypedef DEBUG_RX_Handles;	
uint8_t rxBuf[30];
uint8_t count = 0;
extern ErrorStatus  HSEStartUpStatus;
extern FLASH_Status FlashStatus;
extern uint16_t VarValue;
extern uint16_t VirtAddVarTab[NumbOfVar];
extern uint16_t eData[4];

void DEBUG_UartFullFcn(void){
    printf("Debug Uart Full\n");
    UARTx_ClearRxBuffer(&DEBUG_RX_Handles);
}
void DEBUG_LfFcn(void){
    //printf("Debug LF Fcn called\n");
    //UARTx_ClearRxBuffer(&DEBUG_RX_Handles);
    DEBUG_RX_Handles.receiveFlag = FINISHED;
    DEBUG_Update();
}
void DEBUG_Init(void){
   // Init data handle for uart1
	DEBUG_RX_Handles.terminator = TERMINATOR_LF;
	DEBUG_RX_Handles.byteIndex = -1;
	DEBUG_RX_Handles.rxBufPtr = &rxBuf[0];
	DEBUG_RX_Handles.maxRxLength = sizeof(rxBuf);
    DEBUG_RX_Handles.bufferFullCallback = &DEBUG_UartFullFcn;
    DEBUG_RX_Handles.LFCallback = &DEBUG_LfFcn;
    UART5_defaultInit(57600);    
}
void DEBUG_Update(void){
    //Nhan Duoc Ky tu ket thuc chuoi
    if(DEBUG_RX_Handles.receiveFlag == FINISHED){
        if ((strncmp("mic\n",(const char *)DEBUG_RX_Handles.rxBufPtr, strlen("mic\n")) == 0)){
          printf("trigger the led for mic\n");
        }
        UARTx_ClearRxBuffer(&DEBUG_RX_Handles);
    }
}

void DEBUG_GetString(void){
    //UART5_getString(USART_ReceiveData(UART5),&DEBUG_RX_Handles);
}



