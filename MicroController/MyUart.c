//#include <stdio.h>
#include "MyUart.h"
#include "stdio.h"
#include "stdlib.h"
/*COMMON */
volatile uint8_t rDat[30]= {0,0,0};
#ifdef USE_STM8
uint8_t rxBuf[20];
uint8_t byteIndex;
uint8_t byteCount;
/*
  baudrate: 9600bits, luu y chi config ham nay sau khi da config Clock
  data length: 8bit
  Stopbit: 1
  Parity: No
  Use USART: no
  Mode: enable TX and RX
  void UART1_Init(uint32_t BaudRate,
                  UART1_WordLength_TypeDef WordLength,
                  UART1_StopBits_TypeDef StopBits,
                  UART1_Parity_TypeDef Parity,
                  UART1_SyncMode_TypeDef SyncMode,
                  UART1_Mode_TypeDef Mode);
  */
void UART1_init(uint32_t _baud){
  // Reset Tat ca cac thanh ghi lien quan den module UART1
  UART1_DeInit();
  // Khoi tao module UATT1

  UART1_Init((uint32_t)_baud, UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE);

  // Config Enable/Disable Interrupt
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

}


void UART1_putChar(char c){
  UART1_SendData8((uint8_t)c);
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}


void UART1_putString(const char *s){
  uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART1_putChar((char)s[i]);

  }

}


void UART1_putByte(uint8_t _data){
  UART1_SendData8(_data);
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}


RX_FLAG_Typedef UART1_getString(char c,TERMINATOR_Typedef _terminator){
  RX_FLAG_Typedef result = ONGOING;
  // Cong don data vao buffer
  rxBuf[byteIndex] = c;
	byteIndex++;
  // Check Dieu Kien Ket Thuc Chuoi
  switch (_terminator){
  case TERMINATOR_LF:
    if(c == '\n'){
        byteCount = byteIndex;
        byteIndex = 0;
        result = FINISHED;
    }


    break;
  case TERMINATOR_CR:
    if(c == '\r'){
        byteCount = byteIndex;
        byteIndex = 0;
        result = FINISHED;

    }
      break;
  case TERMINATOR_CRLF:
    if((rxBuf[byteIndex-1] == '\n')&&(rxBuf[byteIndex-2] == '\r')){
        byteCount = byteIndex;
        byteIndex = 0;
        result = FINISHED;
    }
    break;

  }
  return result;
}


uint16_t UART1_getRxLength(void){
    return byteCount;

}


void UART1_resetRxBuffer(void){
  memset(rxBuf,0,sizeof(rxBuf));
  byteCount = 0;
  byteIndex = 0;

}


void UART1_putBytes(void* _data, unsigned int len){
  unsigned char *x = (unsigned char *)_data;
  for (int i = 0; i < len; i++){
    UART1_putByte(x[i]);

  }


}


void UART1_GetData(USART_HandlesTerminatorTypedef *_terminatorHandles ) {
   UARTx_getString(UART1_GetByte(),_terminatorHandles);

}
uint8_t  UART1_GetByte(void){
   return UART1_ReceiveData8();
}




#endif

/*For STM32F10X*/
#ifdef USE_STM32F1
uint8_t tx1Buf[5];
void UARTx_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
  // Add received data to buffer

    if(_terminatorHandles->byteIndex < _terminatorHandles->maxRxLength - 1){
        _terminatorHandles->byteIndex++;
        _terminatorHandles->rxBufPtr[_terminatorHandles->byteIndex] = _data;
        // Kiem Tra Dieu Kien Ket thuc chuoi
        switch(_terminatorHandles->terminator){

            case TERMINATOR_CR:
                if(_data == '\r'){
                    _terminatorHandles->LFCallback();
                }
                break;
            case TERMINATOR_LF:
                if(_data == '\n'){


                    if(_terminatorHandles->LFCallback != 0)
                    _terminatorHandles->LFCallback();
                }
                break;
            case TERMINATOR_CRLF:
                if((_data == '\n')&&(_terminatorHandles->lastByte == '\r')){
                    if(_terminatorHandles->LFCallback != 0)
                    (*_terminatorHandles->LFCallback)();
                }
                break;
            case TERMINATOR_COUNT:
                if(_terminatorHandles->byteIndex >= _terminatorHandles->byteCount -1){
                    if(_terminatorHandles->LFCallback != 0)
                    _terminatorHandles->LFCallback();

                }
                break;
           case TERMINATOR_USER_DEFINE:
                if(_terminatorHandles->LFCallback != 0)
                    _terminatorHandles->LFCallback();
                break;
        };
    }
    else{
        _terminatorHandles->byteIndex = -1;
        if(_terminatorHandles->bufferFullCallback != 0)
        (_terminatorHandles->bufferFullCallback());
    }
}
void UARTx_ClearRxBuffer(USART_HandlesTerminatorTypedef *_terminatorHandles){
    memset(_terminatorHandles->rxBufPtr, 0, _terminatorHandles->maxRxLength);
    _terminatorHandles->byteIndex = -1;
    _terminatorHandles->beginInd[0] = -1;
    _terminatorHandles->beginInd[1] = -1;
    _terminatorHandles->endInd = -1;
    _terminatorHandles->receiveFlag = ONGOING;
}
uint16_t UARTx_GetRxLength(USART_HandlesTerminatorTypedef *_terminatorHandles){
    return _terminatorHandles->byteIndex +1;
}

void UARTx_Init(USART_TypeDef *mUart,uint32_t _baud){
    if (mUart == USART1){
        //1 Init
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        //2 Reset All Register
        USART_DeInit(USART1);
        //3 RCC Config
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        //4 Configure USART Tx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //5 Configure USART Rx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //6 UART Config
        USART_InitStructure.USART_BaudRate = _baud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        //7 Configure USART1
        USART_Init(USART1, &USART_InitStructure);
    }
    else if (mUart == USART2){
        //1 Init
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        //2 Reset All Register
        USART_DeInit(USART2);
        //3 RCC Config
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        //4 Configure USART Tx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //5 Configure USART Rx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //6 UART Config
        USART_InitStructure.USART_BaudRate = _baud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        //7 Configure USART2
        USART_Init(USART2, &USART_InitStructure);

    }
    else if (mUart == USART3){
        //1 Init
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        //2 Reset All Register
        USART_DeInit(USART3);
        //3 RCC Config
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        //4 Configure USART Tx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        //5 Configure USART Rx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        //6 UART Config
        USART_InitStructure.USART_BaudRate = _baud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        //7 Configure USART2
        USART_Init(USART3, &USART_InitStructure);

    }
    else if (mUart == UART4){
        //1 Init
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        //2 Reset All Register
        //USART_DeInit(UART4);
        //3 RCC Config
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        //4 Configure USART Tx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        //5 Configure USART Rx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        //6 UART Config
        USART_InitStructure.USART_BaudRate = _baud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;



        //7 Configure USART4
        USART_Init(UART4, &USART_InitStructure);
        UART4->DR = 0;

    }
    else if (mUart == UART5){
        //1 Init
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        //2 Reset All Register
        USART_DeInit(UART5);
        //3 RCC Config
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
        //4 Configure USART Tx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        //5 Configure USART Rx as alternate function
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        //6 UART Config
        USART_InitStructure.USART_BaudRate = _baud;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        //7 Configure USART5
        USART_Init(UART5, &USART_InitStructure);
    }

}

void UARTx_RxDmaInit(USART_TypeDef *mUart,uint8_t *rxBuf, uint16_t length){
    DMA_InitTypeDef DMA_InitStructure;
    DMA_Channel_TypeDef  *uartRxDmaChannel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(rxBuf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    if (mUart == USART1){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uartRxDmaChannel =  DMA1_Channel5;

    }
    else if (mUart ==  USART2){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uartRxDmaChannel = DMA1_Channel6 ;

    }
    else if (mUart == USART3){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uartRxDmaChannel = DMA1_Channel3;
    }
    else if (mUart == UART4){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
        uartRxDmaChannel = DMA2_Channel3;
    }
    DMA_DeInit(uartRxDmaChannel);
    DMA_Init(uartRxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(uartRxDmaChannel,ENABLE);
    USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
}
void UARTx_TxDmaInit(USART_TypeDef *mUart, uint8_t * data, uint16_t length){
    DMA_InitTypeDef DMA_InitStructure;
    DMA_Channel_TypeDef  *uarttxDmaChannel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(data);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    if (mUart == USART1){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel =  DMA1_Channel4;

    }
    else if (mUart ==  USART2){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel = DMA1_Channel7 ;

    }
    else if (mUart == USART3){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel = DMA1_Channel2;
    }
    else if (mUart == UART4){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
        uarttxDmaChannel = DMA2_Channel5;
    }
    DMA_DeInit(uarttxDmaChannel);
    DMA_Init(uarttxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(uarttxDmaChannel,ENABLE);
    USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
}
void UARTx_TxDmaDefaultInit(USART_TypeDef *mUart,uint32_t _baud, uint8_t * data, uint16_t length){
    UARTx_Init(mUart,_baud);
    UARTx_TxDmaInit(mUart, data, length);
    UARTx_SetTxDmaIntPriority(mUart,0,0,0);
    UARTx_Start(mUart);
}

void UARTx_RxDmaDefaultInit(USART_TypeDef *mUart,uint32_t _baud, uint8_t *rxBuf, uint16_t length){
    UARTx_Init(mUart,_baud);
    UARTx_RxDmaInit(mUart, rxBuf, length);
    UARTx_SetRxDmaIntPriority(mUart,0,0,0);
    UARTx_Start(mUart);
}

void UARTx_Start(USART_TypeDef *mUart){
    if (mUart == USART1){
        USART_Cmd(USART1, ENABLE);
    }
    else if (mUart == USART2){
        USART_Cmd(USART2, ENABLE);

    }
    else if (mUart == USART3){
        USART_Cmd(USART3, ENABLE);

    }
    else if (mUart == UART4){
        USART_Cmd(UART4, ENABLE);

    }
    else if (mUart == UART5){
        USART_Cmd(UART5, ENABLE);

    }

}
void UARTx_Stop(USART_TypeDef *mUart){
    if (mUart == USART1){
        USART_Cmd(USART1, DISABLE);
    }
    else if (mUart == USART2){
        USART_Cmd(USART2, DISABLE);
    }
    else if (mUart == USART3){
        USART_Cmd(USART3, DISABLE);
    }
    else if (mUart == UART4){
        USART_Cmd(UART4, DISABLE);
    }
    else if (mUart == UART5){
        USART_Cmd(UART5, DISABLE);
    }

}
void UARTx_EnableRxInt(USART_TypeDef *mUart){
    if (mUart == USART1){
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    }
    else if (mUart == USART2){
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    }
    else if (mUart == USART3){
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    }
    else if (mUart == UART4){
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    }
    else if (mUart == UART5){
        USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    }

}
void UARTx_DisableRxInt(USART_TypeDef *mUart){
    if (mUart == USART1){
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
    else if (mUart == USART2){
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    }
    else if (mUart == USART3){
        USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    }
    else if (mUart == UART4){
        USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    }
    else if (mUart == UART5){
        USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
    }

}
void UARTx_EnableTxInt(USART_TypeDef *mUart){
    if (mUart == USART1){
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
    else if (mUart == USART2){
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    }
    else if (mUart == USART3){
        USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    }
    else if (mUart == UART4){
        USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
    }
    else if (mUart == UART5){
        USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
    }

}
void UARTx_disableTxInt(USART_TypeDef *mUart){
    if (mUart == USART1){
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }
    else if (mUart == USART2){
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    }
    else if (mUart == USART3){
        USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    }
    else if (mUart == UART4){
        USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
    }
    else if (mUart == UART5){
        USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
    }

}
void UARTx_PutByte(USART_TypeDef *mUart, uint8_t _data){
    if (mUart == USART1){
        USART_SendData(USART1, (uint16_t) _data);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
    else if (mUart == USART2){
        USART_SendData(USART2, (uint16_t) _data);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

    }
    else if (mUart == USART3){
        USART_SendData(USART3, (uint16_t) _data);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    }
    else if (mUart == UART4){
        USART_SendData(UART4, (uint16_t) _data);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
    }
    else if (mUart == UART5){
        USART_SendData(UART5, (uint16_t) _data);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
    }

}
void UARTx_PutBytes(USART_TypeDef *mUart, uint8_t *_data, uint16_t length){
  uint8_t *x = (uint8_t *)_data;
  for (uint16_t i = 0; i < length; i++){
    UARTx_PutByte(mUart,x[i]);
  }

}
void UARTx_PutBytesDMA(USART_TypeDef *mUart, uint8_t *_data, uint16_t length){

    DMA_InitTypeDef DMA_InitStructure;
    DMA_Channel_TypeDef  *uarttxDmaChannel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(_data);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    if (mUart == USART1){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel =  DMA1_Channel4;

    }
    else if (mUart ==  USART2){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel = DMA1_Channel7 ;

    }
    else if (mUart == USART3){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        uarttxDmaChannel = DMA1_Channel2;
    }
    else if (mUart == UART4){
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
        uarttxDmaChannel = DMA2_Channel5;
    }
    DMA_DeInit(uarttxDmaChannel);
    DMA_Init(uarttxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(uarttxDmaChannel,ENABLE);
    USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);


}
void UARTx_SetTxRxIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;


    if (mUart == USART1){
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

    }
    else if (mUart == USART2){
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    }
    else if (mUart == USART3){
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    }
    else if (mUart == UART4){
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    }
    else if (mUart == UART5){
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    }
    NVIC_Init(&NVIC_InitStructure);

}
void UARTx_SetRxDmaIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(_priorityGroup);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _preemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    if (mUart == USART1){
        USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
        DMA_ITConfig(DMA1_Channel5,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    else if (mUart == USART2){
        USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
        DMA_ITConfig(DMA1_Channel6,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);

    }
    else if (mUart == USART3){
        USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
        DMA_ITConfig(DMA1_Channel3,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    else if (mUart == UART4){
        USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
        DMA_ITConfig(DMA2_Channel3,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    NVIC_Init(&NVIC_InitStructure);
}
void UARTx_SetTxDmaIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(_priorityGroup);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _preemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    if (mUart == USART1){
        USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
        DMA_ITConfig(DMA1_Channel4,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    else if (mUart == USART2){
        USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
        DMA_ITConfig(DMA1_Channel7,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);

    }
    else if (mUart == USART3){
        USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
        DMA_ITConfig(DMA1_Channel2,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    else if (mUart == UART4){
        USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
        DMA_ITConfig(DMA2_Channel5,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    }
    NVIC_Init(&NVIC_InitStructure);

}


void UART1_init(uint32_t _baud){
	UARTx_Init(USART1,_baud);
}
void UART1_start(void){

    UARTx_Start(USART1);
}
void UART1_stop(void){

    UARTx_Stop(USART1);
}
void UART1_enableRxInt(void){
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
void UART1_disableRxInt(void){
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}
void UART1_enableTxInt(void){
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}
void UART1_disableTxInt(void){
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}
void UART1_putChar(char c){
	USART_SendData(USART1, (uint16_t) c);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void UART1_putString(const char *s){
	uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART1_putChar((char)s[i]);

  }
}
void UART1_putByte(uint8_t _data){
	USART_SendData(USART1, (uint16_t) _data);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
void UART1_putBytes(void* _data, unsigned int len){
	uint8_t *x = (uint8_t *)_data;
  for (int i = 0; i < len; i++){
    UART1_putByte(x[i]);

  }
}
void UART1_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
	 UARTx_getString(_data, _terminatorHandles );
}


uint16_t UART1_getRxLength(void){
	return 0;
}
void UART1_resetRxBuffer(void){

}
void UART1_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART1_defaultInit(uint32_t _baud){
	UART1_init(_baud);
	UART1_enableRxInt();
	UART1_setPriority(NVIC_PriorityGroup_0,0,0);
	UART1_start();
}
void UART1_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    DMA_Channel_TypeDef  *uart1RxDmaChannel = DMA1_Channel5;
    USART_TypeDef *mUart = USART1;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC Config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*Setup DMA*/

    DMA_DeInit(uart1RxDmaChannel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(rxBuf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(uart1RxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(uart1RxDmaChannel,ENABLE);
    /*Setup GPIO*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*Setup UART*/
	USART_DeInit(mUart);
	USART_InitStructure.USART_BaudRate = _baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(mUart, &USART_InitStructure);
    //Enable USART2 DMA Rx
    USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
    // Enable  DMA Channel

    // Enable UART

    DMA_ITConfig(uart1RxDmaChannel,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(mUart, ENABLE);
}
void UART1_TxDmaInit(uint32_t _baud, uint8_t length){
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    DMA_Channel_TypeDef  *txDmaChannel = DMA1_Channel4;
    USART_TypeDef *mUart = USART1;

    //RCC Config
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*Setup DMA*/

    DMA_DeInit(txDmaChannel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(tx1Buf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(txDmaChannel,&DMA_InitStructure);
    DMA_Cmd(txDmaChannel,ENABLE);
    /*Setup GPIO*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*Setup UART*/
	USART_DeInit(mUart);
	USART_InitStructure.USART_BaudRate = _baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(mUart, &USART_InitStructure);
    //Enable USART2 DMA Tx
    USART_DMACmd(mUart, USART_DMAReq_Tx, ENABLE);
    // Enable UART
    USART_Cmd(mUart, ENABLE);
}
//UART2
void UART2_init(uint32_t _baud){
	UARTx_Init(USART2, _baud);

}
void UART2_start(void){
	USART_Cmd(USART2, ENABLE);
}
void UART2_stop(void){
	USART_Cmd(USART2, DISABLE);
}
void UART2_enableRxInt(void){
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}
void UART2_disableRxInt(void){
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}
void UART2_enableTxInt(void){
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}
void UART2_disableTxInt(void){
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
}
void UART2_putChar(char c){
	USART_SendData(USART2, (uint16_t) c);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
void UART2_putString(const char *s){
	uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART2_putChar((char)s[i]);

  }
}
void UART2_putByte(uint8_t _data){
	USART_SendData(USART2, (uint16_t) _data);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
void UART2_putBytes(void* _data, unsigned int len){
	uint8_t *x = (uint8_t *)_data;
  for (int i = 0; i < len; i++){
    UART2_putByte(x[i]);

  }
}
void UART2_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
	 UARTx_getString(_data, _terminatorHandles );
}
uint16_t UART2_getRxLength(void){
	return 0;

}
void UART2_resetRxBuffer(void){};
void UART2_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART2_defaultInit(uint32_t _baud){
	UART2_init(_baud);
	UART2_enableRxInt();
	UART2_setPriority(NVIC_PriorityGroup_0,0,1);
	UART2_start();
}
void UART2_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    DMA_Channel_TypeDef  *uart2RxDmaChannel = DMA1_Channel6;
    USART_TypeDef *mUart = USART2;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC Config
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*Setup DMA*/

    DMA_DeInit(uart2RxDmaChannel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(rxBuf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(uart2RxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(uart2RxDmaChannel,ENABLE);
    /*Setup GPIO*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*Setup UART*/
    USART_DeInit(mUart);
    USART_InitStructure.USART_BaudRate = _baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(mUart, &USART_InitStructure);
    //Enable USART2 DMA Rx
    USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
    // Enable  DMA Channel

    // Enable UART

    DMA_ITConfig(uart2RxDmaChannel,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(mUart, ENABLE);
}
//UART3
void UART3_init(uint32_t _baud){
	UARTx_Init(USART3, _baud);
}
void UART3_start(void){
	USART_Cmd(USART3, ENABLE);
}
void UART3_stop(void){
	USART_Cmd(USART3, DISABLE);
}
void UART3_enableRxInt(void){
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
void UART3_disableRxInt(void){
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}
void UART3_enableTxInt(void){
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}
void UART3_disableTxInt(void){
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
}
void UART3_putChar(char c){
	USART_SendData(USART3, (uint16_t) c);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}
void UART3_putString(const char *s){
	uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART3_putChar((char)s[i]);

  }
}
void UART3_putByte(uint8_t _data){
	USART_SendData(USART3, (uint16_t) _data);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}
void UART3_putBytes(void* _data, unsigned int len){
	uint8_t *x = (uint8_t *)_data;
  for (int i = 0; i < len; i++){
    UART3_putByte(x[i]);

  }
}
void UART3_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
	 UARTx_getString(_data, _terminatorHandles );
}
uint16_t UART3_getRxLength(void){
	return 0;
}
void UART3_resetRxBuffer(void){

}
void UART3_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART3_defaultInit(uint32_t _baud){
	UART3_init(_baud);
	UART3_enableRxInt();
	UART3_setPriority(NVIC_PriorityGroup_0,0,2);
	UART3_start();
}

void UART3_RxDmaInit(uint32_t _baud, uint8_t *rxBuf, uint8_t length){
    UARTx_RxDmaInit(USART3,rxBuf,(uint16_t) length);
    UARTx_Init(USART3,9600);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    UARTx_SetRxDmaIntPriority(USART3,NVIC_PriorityGroup_0,0,0);
    UARTx_Start(USART3);
}
//UART4
void UART4_init(uint32_t _baud){
	UARTx_Init(UART4, _baud);
}
void UART4_start(void){
	USART_Cmd(UART4, ENABLE);
}
void UART4_stop(void){
	USART_Cmd(UART4, DISABLE);
}
void UART4_enableRxInt(void){
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
}
void UART4_disableRxInt(void){
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
}
void UART4_enableTxInt(void){
	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
}
void UART4_disableTxInt(void){
	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
}
void UART4_putChar(char c){
	USART_SendData(UART4, (uint16_t) c);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}
void UART4_putString(const char *s){
	uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART4_putChar((char)s[i]);

  }
}
void UART4_putByte(uint8_t _data){
	USART_SendData(UART4, _data);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
}
void UART4_putBytes(void* _data, unsigned int len){
	uint8_t *x = (uint8_t *)_data;
  for (int i = 0; i < len; i++){
    UART4_putByte(x[i]);

  }
}
void UART4_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
	 UARTx_getString(_data,_terminatorHandles );
}
uint16_t UART4_getRxLength(void){
	return 0;
}
void UART4_resetRxBuffer(void){

}
void UART4_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART4_defaultInit(uint32_t _baud){
	UART4_init(_baud);
	UART4_enableRxInt();
	UART4_setPriority(NVIC_PriorityGroup_0,0,3);
	UART4_start();
}

void UART4_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length){
    /*
    //1 Init
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//2 Reset All Register
	USART_DeInit(UART4);
	//3 RCC Config
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//4 Configure USART Tx as alternate function
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//5 Configure USART Rx as alternate function
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//6 UART Config
	USART_InitStructure.USART_BaudRate = _baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//7 Configure USART4
	USART_Init(UART4, &USART_InitStructure);
    */
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    USART_TypeDef *mUart = UART4;
    DMA_Channel_TypeDef* rxDmaChannel = DMA2_Channel3;
    NVIC_InitTypeDef NVIC_InitStructure;

    //RCC Config
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    /*Setup DMA*/

    DMA_DeInit(rxDmaChannel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&mUart->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(rxBuf);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = length;
    DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority =  DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(rxDmaChannel,&DMA_InitStructure);
    DMA_Cmd(rxDmaChannel,ENABLE);
    /*Setup GPIO*/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*Setup UART*/
	USART_DeInit(mUart);
	USART_InitStructure.USART_BaudRate = _baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(mUart, &USART_InitStructure);
    //Enable UART4 DMA Rx
    USART_DMACmd(mUart, USART_DMAReq_Rx, ENABLE);
    // Enable  DMA Channel

    // Enable UART

    DMA_ITConfig(rxDmaChannel,DMA_IT_HT | DMA_IT_TE|DMA_IT_TC,ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable and configure RCC global IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_Cmd(mUart, ENABLE);
}
//UART5
void UART5_init(uint32_t _baud){
	UARTx_Init(UART5, _baud);
}
void UART5_start(void){
	USART_Cmd(UART5, ENABLE);
}
void UART5_stop(void){
	USART_Cmd(UART5, DISABLE);
}
void UART5_enableRxInt(void){
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}
void UART5_disableRxInt(void){
	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
}
void UART5_enableTxInt(void){
	USART_ITConfig(UART5, USART_IT_TXE, ENABLE);
}
void UART5_disableTxInt(void){
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
}
void UART5_putChar(char c){
	USART_SendData(UART5, (uint8_t) c);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}
void UART5_putString(const char *s){
	uint8_t n = strlen(s);
  for (int i =0; i < n; i++){
    UART5_putChar((char)s[i]);

  }
}
void UART5_putByte(uint8_t _data){
	USART_SendData(UART5,  _data);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}
void UART5_putBytes(void* _data, unsigned int len){
	uint8_t *x = (uint8_t *)_data;
  for (int i = 0; i < len; i++){
    UART5_putByte(x[i]);

  }
}
void UART5_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles){
	 UARTx_getString(_data,_terminatorHandles );
}
uint16_t UART5_getRxLength(void){
	return 0;
}
void UART5_resetRxBuffer(void){

}
void UART5_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(_priorityGroup);
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =  _preemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void UART5_defaultInit(uint32_t _baud){
	UART5_init(_baud);
	UART5_enableRxInt();
	UART5_setPriority(NVIC_PriorityGroup_0,0,4);
	UART5_start();
}
GETCHAR_PROTOTYPE
{
    //return USART_ReceiveData(USART1);
	// return GetUSART(USART2);
	// return GetUSART(USART3);
	 return USART_ReceiveData(UART4);
    //return GetUSART(UART5);
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  //USART_SendData(USART1, (uint8_t) ch);
	//UART5_putByte(ch);
	//USART_SendData(USART2, (uint8_t) ch);
	//USART_SendData(USART3, (uint8_t) ch);
	USART_SendData(UART4, (uint8_t) ch);
	//USART_SendData(UART5, (uint8_t) ch);

  /* Loop until the end of transmission */
  //while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
	//while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){}
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET){}
	//while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET){}


  return ch;
}
#endif

/*For stm32F7*/
#ifdef USE_STM32F7_HAL

void UARTx_PutBytes(UART_HandleTypeDef *uartx, uint8_t *dataPtr, uint16_t length){

}
void UARTx_PutByte(UART_HandleTypeDef *uartx, uint8_t data){

}

void UARTx_Init(USART_TypeDef *uartx, uint16_t baud){

}
void UART4_DefaultInit(uint16_t baud){
    UARTx_Init(UART4,9600);
}



static struct {
  void (*init)(uint16_t );
}UART4Cmd = {
  UART4_DefaultInit


};

#endif













