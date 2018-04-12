#include"MyEpprom.h"
#include<Arduino.h>
#include <EEPROM.h>

 Mega2560Calibrator::Mega2560Calibrator() {
     
    
}

void Mega2560Calibrator::init() {
     char sID[] = "www.mlabviet.com";
     for (unsigned int i = 0; i <sizeof(sID)-1; i++) {
        EEPROM.write(i, sID[i] );
     }
    
}

