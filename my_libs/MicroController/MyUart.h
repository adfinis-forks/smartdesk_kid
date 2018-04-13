#ifndef __MY_UART
#define __MY_UART
/*
for stm8

*/
#include <stdio.h>
#include "string.h"
#include "ctype.h"
#include <stdint.h>
#ifdef STM32
#include "stm32f10x_usart.h"
#endif
#ifdef USE_STM32F7_HAL
#include "stm32f7xx.h"
#include "stm32f7xx_hal_usart.h"
#include "stm32f7xx_hal_def.h"
#endif
#define MAX_TX_BUFFER 3
#define MAX_RX_BUFFER 4
typedef enum{
  ONGOING =0,
  FINISHED = 1

}RX_FLAG_Typedef;
typedef enum {
  TERMINATOR_LF = 0, 	/*0x0A*/
  TERMINATOR_CR,     	/*0x0D      */
  TERMINATOR_CRLF,      /*0x0D + 0x0A*/
  TERMINATOR_USER_DEFINE,
  TERMINATOR_COUNT


}TERMINATOR_Typedef;
typedef struct x {

  TERMINATOR_Typedef terminator;
  int byteCount;
  int byteIndex;
  RX_FLAG_Typedef receiveFlag;
  unsigned char *rxBufPtr;
  int maxRxLength;
  unsigned char lastByte;
  void (*bufferFullCallback)(void);
  void (*LFCallback)(void);
  int beginInd[2];
  int endInd;
  USART_TypeDef *uart_name;
  uint32_t baudRate;


}USART_HandlesTerminatorTypedef;
typedef struct  {
   TERMINATOR_Typedef terminator;
   USART_TypeDef *uart_name;
   uint32_t baud;
} UART_HandlesInit_Typedef;


#ifdef USE_STM8
#include "stm8s.h"
#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
#include "stm8s_uart1.h"
#endif /* STM8S208 || STM8S207 || STM8S103 ||STM8S903 || STM8AF52Ax || STM8AF62Ax */
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
#include "stm8s_uart2.h"
#endif /* STM8S105 || STM8AF626x */
#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) ||\
defined (STM8AF62Ax)
#include "stm8s_uart3.h"
#endif /* STM8S208 || STM8S207 || STM8AF52Ax || STM8AF62Ax */

  void UART1_init(uint32_t _baud);
  void UART1_putChar(char c);
  void UART1_putString(const char *s);
  void UART1_putByte(uint8_t _data);
  void UART1_putBytes(void* _data, unsigned int len);
  RX_FLAG_Typedef UART1_getString(char c,TERMINATOR_Typedef _terminator);
  uint16_t UART1_getRxLength(void);
  void UART1_resetRxBuffer(void);
  void UART1_GetData(USART_HandlesTerminatorTypedef *_terminatorHandles );
  uint8_t  UART1_GetByte(void);
#endif


#ifdef USE_STM32F1
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include <stdio.h>
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

//General/
void UARTx_Init(USART_TypeDef *mUart,uint32_t _baud);
void UARTx_RxDmaInit(USART_TypeDef *mUart,uint8_t *rxBuf, uint16_t length);
void UARTx_TxDmaInit(USART_TypeDef *mUart, uint8_t * data, uint16_t length);
void UARTx_RxDmaDefaultInit(USART_TypeDef *mUart,uint32_t _baud, uint8_t *rxBuf, uint16_t length);
void UARTx_TxDmaDefaultInit(USART_TypeDef *mUart,uint32_t _baud, uint8_t * data, uint16_t length);
void UARTx_Start(USART_TypeDef *mUart);
void UARTx_Stop(USART_TypeDef *mUart);
void UARTx_EnableRxInt(USART_TypeDef *mUart);
void UARTx_DisableRxInt(USART_TypeDef *mUart);
void UARTx_EnableTxInt(USART_TypeDef *mUart);
void UARTx_disableTxInt(USART_TypeDef *mUart);
void UARTx_PutByte(USART_TypeDef *mUart, uint8_t _data);
void UARTx_PutBytes(USART_TypeDef *mUart, uint8_t *_data, uint16_t length);
void UARTx_PutBytesDMA(USART_TypeDef *mUart, uint8_t *_data, uint16_t length);
void UARTx_SetTxRxIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UARTx_SetRxDmaIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UARTx_SetTxDmaIntPriority(USART_TypeDef *mUart,uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);

  //UART1
void UART1_init(uint32_t _baud);
void UART1_start(void);
void UART1_stop(void);
void UART1_enableRxInt(void);
void UART1_disableRxInt(void);
void UART1_enableTxInt(void);
void UART1_disableTxInt(void);
void UART1_putChar(char c);
void UART1_putString(const char *s);
void UART1_putByte(uint8_t _data);
void UART1_putBytes(void* _data, unsigned int len);
void UART1_getString(uint8_t _data, USART_HandlesTerminatorTypedef * _terminatorHandles);
uint16_t UART1_getRxLength(void);
void UART1_resetRxBuffer(void);


void UART1_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UART1_defaultInit(uint32_t _baud);
void UART1_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length);
void UART1_TxDmaInit(uint32_t _baud, uint8_t length);

  //UART2
void UART2_init(uint32_t _baud);
void UART2_start(void);
void UART2_stop(void);
void UART2_enableRxInt(void);
void UART2_disableRxInt(void);
void UART2_enableTxInt(void);
void UART2_disableTxInt(void);
void UART2_putChar(char c);
void UART2_putString(const char *s);
void UART2_putByte(uint8_t _data);
void UART2_putBytes(void* _data, unsigned int len);
void UART2_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles);
uint16_t UART2_getRxLength(void);
void UART2_resetRxBuffer(void);
void UART2_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UART2_defaultInit(uint32_t _baud);
void UART2_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length);
  //UART3
void UART3_init(uint32_t _baud);
void UART3_start(void);
void UART3_stop(void);
void UART3_enableRxInt(void);
void UART3_disableRxInt(void);
void UART3_enableTxInt(void);
void UART3_disableTxInt(void);
void UART3_putChar(char c);
void UART3_putString(const char *s);
void UART3_putByte(uint8_t _data);
void UART3_putBytes(void* _data, unsigned int len);
void UART3_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles);
uint16_t UART3_getRxLength(void);
void UART3_resetRxBuffer(void);
void UART3_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UART3_defaultInit(uint32_t _baud);
void UART3_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length);
  //UART4
void UART4_init(uint32_t _baud);
void UART4_start(void);
void UART4_stop(void);
void UART4_enableRxInt(void);
void UART4_disableRxInt(void);
void UART4_enableTxInt(void);
void UART4_disableTxInt(void);
void UART4_putChar(char c);
void UART4_putString(const char *s);
void UART4_putByte(uint8_t _data);
void UART4_putBytes(void* _data, unsigned int len);
void UART4_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles);
uint16_t UART4_getRxLength(void);
void UART4_resetRxBuffer(void);
void UART4_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UART4_defaultInit(uint32_t _baud);
void UART4_RxDmaInit(uint32_t _baud,uint8_t *rxBuf, uint8_t length);
  //UART5
void UART5_init(uint32_t _baud);
void UART5_start(void);
void UART5_stop(void);
void UART5_enableRxInt(void);
void UART5_disableRxInt(void);
void UART5_enableTxInt(void);
void UART5_disableTxInt(void);
void UART5_putChar(char c);
void UART5_putString(const char *s);
void UART5_putByte(uint8_t _data);
void UART5_putBytes(void* _data, unsigned int len);
void UART5_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles);
uint16_t UART5_getRxLength(void);
void UART5_resetRxBuffer(void);
void UART5_setPriority(uint32_t _priorityGroup, uint8_t _preemptionPriority, uint8_t subPriority);
void UART5_defaultInit(uint32_t _baud);
void UARTx_getString(uint8_t _data, USART_HandlesTerminatorTypedef *_terminatorHandles);
void UARTx_ClearRxBuffer(USART_HandlesTerminatorTypedef *_terminatorHandles);
uint16_t UARTx_GetRxLength(USART_HandlesTerminatorTypedef *_terminatorHandles);

#endif

#ifdef USE_STM32F7_HAL
#include "stm32f7xx.h"
#include "stm32f7xx_hal_usart.h"
  void UARTx_PutBytes(UART_HandleTypeDef *uartx, uint8_t *dataPtr, uint16_t length);
  void UARTx_PutByte(UART_HandleTypeDef *uartx, uint8_t data);
  void UARTx_Init(USART_TypeDef *uartx, uint16_t baud);
  void UART4_DefaultInit(uint16_t baud);










#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif


#endif

/*
#include "stm32f10x_usart.h"
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

void uartInit(USART_TypeDef* USARTx, unsigned int baud);
short getString(USART_TypeDef* USARTx ,char *str,short *count);
void ClearString(char *s, char n);
void SendUSART(USART_TypeDef* USARTx,uint16_t ch);
void sendDatas(USART_TypeDef* USARTx, char* datas,  int length);
#endif


      */


#endif

