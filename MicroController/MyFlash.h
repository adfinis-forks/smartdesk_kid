#ifndef __MY_FLASH_H
#define __MY_FLASH_H
#include "stm32f10x_flash.h"
#include "eeprom.h"

void MY_FLASH_Init(void);
void MY_FLASH_Write(uint16_t _virtualAddress, uint16_t value);
uint16_t MY_FLASH_Read(uint16_t _virtualAddress);

#endif

