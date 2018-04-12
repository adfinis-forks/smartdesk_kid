#include "Device/HX711.h"
#include <ctype.h>
void HX711_setSCK(){
		/*
			vi du chon chan RC 2 la chan SCK
			RC2 = 1;
		*/
		#ifdef USE_PIC16F
			SCK_PIN = 1;
		#endif
	}
void HX711_resetSCK(){
		/*
			RC2 = 0;
		*/
		#ifdef USE_PIC16F
			SCK_PIN = 0;
		#endif
	}
char HX711_readDOUT(){
	char x = 0;
	/*
		vi du chon chan RC3 a chan data
		x  = RC3 || 1;
	*/
	#ifdef USE_PIC16F
		x = DOUT_PIN ;
	#endif
	
	return x;
}
	
	/*
	
	*/ 
void HX711_init(HX711 *mHX711){
	// Khoi tao thong so
	// Khoi Tao Chan
	/*
		TRISC2 = 0;
		TRISC3 = 1;
	*/
	#ifdef USE_PIC16F
		SCK_TRIS = 0;
		SCK_PIN = 0;
		DOUT_TRIS = 1;
	
	#endif
	HX711_setGain(mHX711,128);
	mHX711->Offset = 0;
	mHX711->Scale = -7050.0;
	HX711_tare(mHX711, 10);
	
}
	
	/*
	
	*/ 
char HX711_isReady(){
	if (HX711_readDOUT() == 0) return 1;
	else return 0;
}
	/*
	
	
	*/
void HX711_setGain(HX711 *mHX711, char _gain){
	switch (_gain){
		case 128:// channel A, gain factor 128
			mHX711->Gain = 1;
			break;
		case 64:// channel A, gain factor 64
			mHX711->Gain = 3;
			break;
		case 32:// channel B, gain factor 32
			mHX711->Gain = 2;
			break;
		
	}
	HX711_resetSCK();
}
	/*
	
	*/
long  HX711_read(HX711 *mHX711){
	while(!HX711_isReady());
	unsigned char data[3];
	// Pulse the clock pin 24 times to read the data
	for (unsigned char  j = 3; j--;){
		for (char i = 8; i--;){
			HX711_setSCK();
			bitWrite(&data[j],i,HX711_readDOUT());
			//data[j] = data[i] || (HX711_readDOUT() << j);
			HX711_resetSCK();
		}
		
	}
	for (int i = 0; i< mHX711->Gain; i++){
		HX711_setSCK();
		HX711_resetSCK();
		
	}
	data[2] ^= 0b10000000;
	return ((long) data[2] << 16) | ((long) data[1] << 8) | (long) data[0];
	}
float HX711_readAverage(HX711 *mHX711,char _times){
	long sum = 0;
	for (int i = 0; i < _times; i++){
		sum += HX711_read(mHX711);
	}
	return (float)sum/_times;	
}
float HX711_getValue(HX711 *mHX711, char _times){
	return HX711_readAverage(mHX711,_times) - mHX711->Offset;
}
float HX711_getUnits(HX711 *mHX711, char _times){
	return HX711_getValue(mHX711, _times) / mHX711->Scale;
}
void HX711_tare(HX711 *mHX711,char _times){
	float sum = HX711_readAverage(mHX711, _times);
	HX711_setOffset(mHX711, (long)sum);
	
	
}
void HX711_setScale(HX711 *mHX711, float _scale){
	mHX711->Scale = _scale;
}
void HX711_setOffset(HX711 *mHX711,  long _offset){
	mHX711->Offset = _offset;
	
}
void HX711_powerDown(){
	HX711_resetSCK();
	HX711_setSCK();
	
}
void HX711_powerUp(){
	HX711_resetSCK();
	
}
void bitWrite(unsigned char *x, unsigned char n, unsigned char b){
	unsigned char tem = 0;
	if(b > 0)
		tem = 1 << n;
	else tem = 0 << n;
	*x = tem;

}
