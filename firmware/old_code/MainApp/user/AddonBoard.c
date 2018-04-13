#include "AddonBoard.h"
#include "stm32f10x_tim.h"
ADDON_BOARD_Handle_Typedef BOARD_Handle;
__IO uint32_t LsiFreq = 40000;
void ADDON_BOARD_Init(void){
    // 
    MY_FLASH_Init();                
    //RCC_PCLK1Config(0);         
    // Init UART for Debug
    RpiCmds.Init();
    
    DEBUG_Init();
    // Init Box Controller
    BOX_CONTROLLER_Init();
    // Init module Mic-Button-Leds
    //BUTTON_LEDS_Init();    
    // Init System Tick 100ms
    SysTick_Config(SystemCoreClock/10);
    // Init Watchdog Timer
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_ReloadCounter();
    IWDG_Enable();
    // Enable Timer
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    TIM_TimeBaseInitTypeDef timerInit;
    /*
      frequency = 125000Hz;
      t_time = 0.008ms
      T = 500ms -> TIM_Period = 200/0.008 = 25000
    */
    timerInit.TIM_Prescaler =  ((SystemCoreClock)/125000)-1;
    printf("System Clock = %ld\n",SystemCoreClock);
    timerInit.TIM_Period = 25000-1;
    timerInit.TIM_ClockDivision =  0;
    timerInit.TIM_CounterMode =  TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6,&timerInit);
    //Init Timer6 - 500 ms
    
    // Init Timer7 - 5s
    
    timerInit.TIM_Prescaler =  ((SystemCoreClock)/1600)-1;
    timerInit.TIM_Period = 4800-1;
    timerInit.TIM_ClockDivision =  0;
    timerInit.TIM_CounterMode =  TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7,&timerInit);
    
    
    // Config Timer6 Interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // Config Timer7 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_SetCounter(TIM6,0);
    TIM_SetCounter(TIM7,0);
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    TIM_Cmd(TIM6,ENABLE);
    TIM_Cmd(TIM7,ENABLE);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    
    
}

void ADDON_BOARD_Run(void){
  // Run the Controller in specific period
  BOX_CONTROLLER_Run();    
}

void ADDON_BOARD_Update(void){
  // Process data from Handset
  TIM_HANDSET_Update();
  // Process desk Control Function 
  BOX_CONTROLLER_Update();
  // Reset Watch Dog Timer.
  IWDG_ReloadCounter();
}



