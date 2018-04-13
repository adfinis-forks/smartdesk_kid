#ifndef HX_711_H
#define HX_711_H
#ifdef USE_PIC16F
	#include <htc.h>
	#ifndef SCK_PIN
		#define SCK_PIN RD6
		#define DOUT_PIN RD5
		#define SCK_TRIS TRISD6
		#define DOUT_TRIS TRISD5
	#endif
#endif
	//Interface Function 
	typedef struct hx{
		char Gain;
		long Offset;
		float Scale;
		
		
	}HX711;
	void HX711_setSCK();
	void HX711_resetSCK();
	char HX711_readDOUT();
	void HX711_init(HX711 *mHX711);
	char HX711_isReady();
	void HX711_setGain(HX711 *mHX711, char _gain);
	long  HX711_read(HX711 *mHX711);
	float HX711_readAverage(HX711 *mHX711,char _times);
	float HX711_getValue(HX711 *mHX711, char _times);
	float HX711_getUnits(HX711 *mHX711, char times);
	void HX711_tare(HX711 *mHX711,char _times);
	void HX711_setScale(HX711 *mHX711, float _scale);
	void HX711_setOffset(HX711 *mHX711, long _offset);
	void HX711_powerDown();
	void HX711_powerUp();
	void bitWrite(unsigned char *x, unsigned char n, unsigned char b);
#endif