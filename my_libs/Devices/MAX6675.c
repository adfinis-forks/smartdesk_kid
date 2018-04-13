#include "Device/MAX6675.h"
#define MAX6675_SO 1
#define MAX6675_CS 1
#ifdef USE_PIC16F
#include<htc.h>
#ifndef _XTAL_FREQ 
#define _XTAL_FREQ 20000000
#define MAX6675_SO 1
#endif
#endif


void   	MAX6675_init(){
#ifdef USE_PIC16F
	MAX6675_CS_TRIS = 0;
	MAX6675_CLK_TRIS = 0;
	MAX6675_SO_TRIS = 1;
	
	MAX6675_CS = 1;
	MAX6675_CLK  =  0 ; 
#endif
	
}
void   	MAX6675_setSCK(){
#ifdef USE_PIC16F
		MAX6675_CLK = 1;
#endif
	
	
}
void   	MAX6675_resetSCK(){
#ifdef USE_PIC16F
		MAX6675_CLK = 0;
#endif
	
}

void   	MAX6675_setCS(){
	#ifdef USE_PIC16F
		MAX6675_CS = 1;
	#endif
	
	
}
void   	MAX6675_resetCS(){
#ifdef USE_PIC16F
		MAX6675_CS = 0;
#endif
	
}
int MAX6675_readBit(){
	if(MAX6675_CS == 1) return 1;
	else return 0;	
}
int MAX6675_readRaw(){
	int i;
	unsigned char d = 0;
	for(i = 7; i >= 0 ; i--){
		MAX6675_resetSCK();
		__delay_ms(1);
		if(MAX6675_readBit()){
			d |= (1 << i);
		}
	
		MAX6675_setSCK();
		__delay_ms(1);
	}
	return d;
}

float MAX6675_readC(){
	unsigned int v;
	MAX6675_resetCS();
	__delay_ms(1);
	v = (unsigned int)MAX6675_readRaw();
	v <<= 8;
	v |= (unsigned int)MAX6675_readRaw();
	MAX6675_setCS();
	
	if(v & 0x04){
		return 0;
	}
	v >>= 3;
	return (float)v*0.25;
	
	
	
	
}
float MAX6675_readF(){
	return (float)MAX6675_readC()*9.0/5.0 + 32;

}
