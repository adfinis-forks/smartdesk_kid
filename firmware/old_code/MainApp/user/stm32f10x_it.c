/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x.h"  
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include "AddonBoard.h"

 __IO uint16_t CurrDataCounterEnd;
uint8_t status = 0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	/* This interrupt is generated when HSE clock fails */
	if (RCC_GetITStatus(RCC_IT_CSS) != RESET){
		/* At this stage: HSE, PLL are disabled (but no change on PLL config) and HSI
       is selected as system clock source */

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Enable HSE Ready interrupt */
    RCC_ITConfig(RCC_IT_HSERDY, ENABLE);

#ifndef SYSCLK_HSE
 #ifdef STM32F10X_CL
    /* Enable PLL and PLL2 Ready interrupts */
    RCC_ITConfig(RCC_IT_PLLRDY | RCC_IT_PLL2RDY, ENABLE);
 #else
    /* Enable PLL Ready interrupt */
    RCC_ITConfig(RCC_IT_PLLRDY, ENABLE);
 #endif	/* STM32F10X_CL */
#endif /* SYSCLK_HSE */

    /* Clear Clock Security System interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_CSS);

    /* Once HSE clock recover, the HSERDY interrupt is generated and in the RCC ISR
       routine the system clock will be reconfigured to its previous state (before
       HSE clock failure) */
  }
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void){
  
}
void EXTI0_IRQHandler(void){

		//if(EXTI_GetITStatus(EXTI_Line8) != RESET){
			//uint8_t x = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
			//GPIO_Write(GPIOD,~GPIO_ReadOutputData(GPIOD));
			//EXTI->PR = EXTI_Line8;
		//}
}
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */


void UART5_IRQHandler(void)
{
  if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    DEBUG_GetString();	
    
  }
	
  
	
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */


void DMA1_Channel3_IRQHandler(void){

   if(DMA_GetITStatus(DMA1_IT_TC3)){
      if (TimBoxCmd.GetConnectStatus()){
        TIMOTION_DmaCallback(FULL_COMPLETED);
      }

      
   }
   if (DMA_GetITStatus(DMA1_IT_HT3)){
    if (TimBoxCmd.GetConnectStatus()){
        TIMOTION_DmaCallback(HALF_COMPLETED);
      }
   }
   DMA_ClearITPendingBit(DMA1_IT_GL3);         
}
void DMA1_Channel5_IRQHandler(void){
  if(DMA_GetITStatus(DMA1_IT_TC5)){
      TIM_HANDSET_DmaCallback();
   }
  DMA_ClearITPendingBit(DMA1_IT_GL5);
}
void DMA1_Channel6_IRQHandler(void){
  if(DMA_GetITStatus(DMA1_IT_TC6)){
      RpiCmds.Callback(FULL_COMPLETED);
  }
  if(DMA_GetITStatus(DMA1_IT_HT6)){
      RpiCmds.Callback(HALF_COMPLETED);
  }
  DMA_ClearITPendingBit(DMA1_IT_GL6);  

}
void DMA1_Channel2_IRQHandler(void){
    if (DMA_GetITStatus(DMA1_IT_TC2)){
        
    }
    DMA_ClearITPendingBit(DMA1_IT_GL2);
}

void DMA2_Channel3_IRQHandler(void){
   if (DMA_GetITStatus(DMA2_IT_TC3)){
     // MBL_DmaCallback();
   
   }
   DMA_ClearITPendingBit(DMA2_IT_GL3);
}
void RCC_IRQHandler(void){
  if(RCC_GetITStatus(RCC_IT_HSERDY) != RESET)
  { 
    /* Clear HSERDY interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_HSERDY);

    /* Check if the HSE clock is still available */
    if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
    { 
#ifdef SYSCLK_HSE
      /* Select HSE as system clock source */
      RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
#else
 #ifdef STM32F10X_CL
      /* Enable PLL2 */
      RCC_PLL2Cmd(ENABLE);
 #else
      /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */ 
      RCC_PLLCmd(ENABLE);
 #endif	/* STM32F10X_CL */
#endif /* SYSCLK_HSE */      
    }
  }

#ifdef STM32F10X_CL
  if(RCC_GetITStatus(RCC_IT_PLL2RDY) != RESET)
  { 
    /* Clear PLL2RDY interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_PLL2RDY);

    /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */ 
    RCC_PLLCmd(ENABLE);
  }
#endif /* STM32F10X_CL */   

  if(RCC_GetITStatus(RCC_IT_PLLRDY) != RESET)
  { 
    /* Clear PLLRDY interrupt pending bit */
    RCC_ClearITPendingBit(RCC_IT_PLLRDY);

    /* Check if the PLL is still locked */
    if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
    { 
      /* Select PLL as system clock source */
      RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    }
  }
}
void TIM6_IRQHandler(void){
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
    AddonBoard.Run();
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  }
  
}
void TIM7_IRQHandler(void){
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET){
    if (TIM_BOX_GetDetectStatus()){
        TIM_BOX_checkHeightCallback();
    }
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  }
}
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
