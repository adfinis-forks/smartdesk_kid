/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MyDht11.hpp
 * Author: thanh
 *
 * Created on April 18, 2017, 12:51 AM
 */

#ifndef MYDHT11_HPP
#define MYDHT11_HPP
#include <string.h>
#include <pthread.h>
#include<wiringPi.h>
#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdarg>
#include <inttypes.h>
#include<assert.h>
void *doThread(void *data);
int CallbackFcn(void *data);
class MyDht11
{
public:
    MyDht11(unsigned int pin = 7) {
        this->pin = pin;
        wiringPiSetup();
        onOffStatus = DHT_DISABLE;
    }
    void setUpDefaultCallback(){
        this->setCallbackFcn(CallbackFcn);
        this->setThreadFcn(doThread,this);
    }
    virtual ~MyDht11() {

    }
    float getTemperater(void);
    float getHumidity(void);
    int (*callbackFcn)(void *);
    void *(*threadFcn)(void*);
    int setCallbackFcn(int (*fcn)(void *)){
        this->callbackFcn = fcn;
    }  
    int setThreadFcn(void*(*fcn)(void *),void *data){
        pthread_create(&this->dht_thread, NULL, fcn, data);
    }
    static const unsigned char DHT_ENABLE = 1;
    static const unsigned char DHT_DISABLE = 0;
    void Start(){
        this->onOffStatus = DHT_ENABLE;
    }
    void Stop(){
        this->onOffStatus = DHT_DISABLE;
    }
    unsigned char isReady(){
        return this->onOffStatus;
    }
    void readDht(){
        uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode( this->pin, OUTPUT );
	digitalWrite( this->pin, LOW );
	delay( 18 );
	digitalWrite( this->pin, HIGH );
	delayMicroseconds( 40 );
	pinMode( this->pin, INPUT );
 
	for ( i = 0; i < 85; i++ )
	{
		counter = 0;
		while ( digitalRead( this->pin ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( this->pin );
 
		if ( counter == 255 )
			break;
 
		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		f = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
	}else  {
		printf( "Data not good, skip\n" );
	}
        
    }
private:
    unsigned int pin;
    int dht11_dat[5] = { 0, 0, 0, 0, 0 };
    float hum;
    float tem;
    unsigned char onOffStatus;
    pthread_t dht_thread;
    
    
   

};

#endif /* MYDHT11_HPP */

