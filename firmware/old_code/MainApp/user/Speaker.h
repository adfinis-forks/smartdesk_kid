#ifndef __SPEAKER_H
#define __SPEAKER_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "main.h"
#define SPEAKER_PORT  				GPIOB
#define SPEAKER_Pin_Mute 			GPIO_Pin_1
#define SPEAKER_Pin_Standby 	GPIO_Pin_2
#ifdef __cplusplus
extern "C" {
#endif
	void SPEAKER_Init(void);
	void SPEAKER_Mute_on(void);
	void SPEAKER_Mute_off(void);
	void SPEAKER_Standby_on(void);
	void SPEAKER_Standby_off(void);
	void SPEAKER_On(void);
	void SPEAKER_Off(void);
	
#ifdef __cplusplus
}
#endif
	

#endif
