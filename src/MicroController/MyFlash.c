#include "MicroController/MyFlash.h"

void MY_FLASH_Init(void){
    FLASH_Unlock();
    /* EEPROM Init */
    EE_Init();
}

void MY_FLASH_Write(uint16_t _virtualAddress, uint16_t value){
    EE_WriteVariable(_virtualAddress, value);
}
uint16_t MY_FLASH_Read(uint16_t _virtualAddress){
    uint16_t data;
    EE_ReadVariable( _virtualAddress, &data); 
    return data;
}

