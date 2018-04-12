#ifndef MAX6675_H
#define MAX6675_H
#ifdef USE_PIC16F
	#include <htc.h>
	#ifndef MAX6675_CS
	#define MAX6675_CS	RD0
	#define MAX6675_CS_TRIS TRISD0

	#define MAX6675_CLK	RC3
	#define MAX6675_CLK_TRIS TRISC3

	#define MAX6675_SO	RD1
	#define MAX6675_SO_TRIS TRISD1
	
	#endif		
#endif
void   	MAX6675_init();
void   	MAX6675_setSCK();
void   	MAX6675_resetSCK();
void   	MAX6675_setCS();
void   	MAX6675_resetCS();
int 	MAX6675_readRaw();
int 	MAX6675_readBit();
float 	MAX6675_readC();
float 	MAX6675_readF();
#endif