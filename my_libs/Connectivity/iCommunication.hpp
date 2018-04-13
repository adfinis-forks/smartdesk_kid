/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iCommunication.hpp
 * Author: thanh
 *
 * Created on March 31, 2016, 1:24 AM
 */

#ifndef ICOMMUNICATION_HPP
#define ICOMMUNICATION_HPP
#include<inttypes.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<assert.h>
#include<string>
#include<unistd.h>
#include<cstring>
#include<vector>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <stdbool.h>
#define MAX_RX_BUFFER 2048
typedef enum{
  ONGOING =0,
  FINISHED = 1
}RX_FLAG_Typedef;
typedef enum{
    MODE_TERMINATOR = 0,
    MODE_BYTECOUNT
}TerminatorMode_Typedef;

typedef enum {
    NOT_READY = 0,
    READY   
}Status_Typedef;

class iCommunication{
    

public:
//----------------------------------------------------------------------------//
    static const uint8_t TERMINATOR_LF                  = 1;
    static const uint8_t TERMINATOR_CR                  = 2;
    static const uint8_t TERMINATOR_CRLF                = 3;
    static const uint8_t TERMINATOR_CUSTOM_BYTE         = 4;
    static const uint8_t TERMINATOR_FAFA                = 5;
    static const uint8_t TERMINATOR_COUNT               = 6;
    static const uint8_t TERMINATOR_TIMEOUT             = 7;
    static const uint8_t TERMINATOR_CUSTOM_BYTES        = 8;   
    /*
     * Used in Terminator Count
     * Callback will be called when rxBuffer reach the byteCount       
     */
    int byte_count; 
    int byte_index;
    uint8_t  terminator_type;
    RX_FLAG_Typedef receiveFlag; 
    int (*bufferFullCallback)(void*, void*);
    int (*LFCallback)(void*, void*);
    int (*ErrorCallback)(void*, void*);
    int (*timeoutCallback)(void*, void*);
    void *target_class;
    __syscall_slong_t threadTime;
    Status_Typedef status;
    pthread_t communication_thread;
    uint8_t rxBuf[MAX_RX_BUFFER];
    uint8_t terminator_byte;
    uint8_t terminator_bytes[MAX_RX_BUFFER];
    uint16_t terminator_bytes_length;
    int maxRxBuffer;
    int maxTxBuffer;
    int rxDataLength;       
    struct timespec tim,tim2;   

//----------------------------------------------------------------------------//    
    int Enable(void){
        this->status = READY;
        return 0;
    }
    int Disable(void){
        this->status = NOT_READY;
        return 0;
    }    
    Status_Typedef isReady(){
        return this->status;
    }
    virtual void setLFCallback(int (*fcn)(void*, void *)){
        this->LFCallback = fcn;
    }
    virtual void setLFCallback(int (*fcn)(void*, void *), void *target_class){
        this->LFCallback = fcn;
        this->target_class = target_class;
    }
    
    virtual void setBufferFullCallback(int (*fcn)(void*, void *)){
        this->bufferFullCallback = fcn;
    } 
    
    virtual void setBufferFullCallback(int (*fcn)(void*, void *), void * target_class){
        this->bufferFullCallback = fcn;
        this->target_class = target_class;
    }
    
    
    virtual void setErrorCallback(int(*fcn)(void *, void *)){
        this->ErrorCallback = fcn;
    }
    
    virtual void setErrorCallback(int(*fcn)(void *, void *), void * target_class){
        this->ErrorCallback = fcn;
        this->target_class = target_class;
    }
    
    
    virtual void setTerminator(uint8_t  terminator){
        this->terminator_type = terminator;
        
    }
    virtual int setupThread(void*(*fcn)(void*), void* _icom){
        int res = 0;
        if(this->terminator_type != iCommunication::TERMINATOR_TIMEOUT){
            res = pthread_create(&this->communication_thread,NULL,fcn, _icom);
        }
        
        return res;
    }
    

    
    iCommunication(){
        this->tim.tv_sec = 0;
        this->tim.tv_nsec = 10000000L; // 10ms
        this->byte_count = 0;
        this->byte_index = -1;
        this->maxRxBuffer = MAX_RX_BUFFER;
        this->rxDataLength = 0;
        this->status = NOT_READY;
    }
    virtual ~iCommunication(){
        printf("Delete iCommunication\n");
    }
    virtual  int getByteAvailable(){
        return this-> byte_count;
    }
    
    //Receive data method for all devices 
    virtual int getData(uint8_t data){
        
    if (this->byte_index < this->maxRxBuffer -1){ 
        this->byte_index++;
        this->rxDataLength = this->byte_index + 1;
        this->rxBuf[this->byte_index] = data;
        
        switch (this->terminator_type){
            case TERMINATOR_LF:
                if (data == '\n'){
                    if (this->LFCallback != 0){
                        this->LFCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_CR:
                if (data == '\r'){
                    if (this->LFCallback != 0){
                        this->LFCallback(this, this->target_class);
                    }
                }
                break; 
            case TERMINATOR_CRLF:
                if ((data == '\n')&&(this->rxBuf[this->byte_index -1] == '\r')){
                    if (this->LFCallback != 0){
                        this->LFCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_CUSTOM_BYTE:
                if(data == this->terminator_byte){
                    if (this->LFCallback != 0){
                        this->LFCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_FAFA:
                if ((this->byte_index >= 1)&&(data == 0xFA)
                        &&(this->rxBuf[this->byte_index -1] == 0xFA)){
                    if (this->LFCallback != 0){                        
                        this->LFCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_COUNT:
                if (this->byte_index >= this->byte_count -1 ){
                    if (this->LFCallback != 0){                        
                        this->LFCallback(this, this->target_class);                        
                    }
                }
                break;
            case TERMINATOR_TIMEOUT:
//                if(this->timeoutCallback != 0){
//                    this->timeoutCallback(this);
//                }
                break;
            case TERMINATOR_CUSTOM_BYTES:
                if(this->rxDataLength >= this->terminator_bytes_length){
                    bool check = true;
                    uint8_t e = this->rxDataLength - this->terminator_bytes_length;
                    for (int i = 0; i < this->terminator_bytes_length - 1; i++){
                        
                        if(this->terminator_bytes[i] != this->rxBuf[i+e]){
                            check = false;
                            break;
                        }
                    }
                    
                    if ((check == true)&&(this->LFCallback != 0)){                        
                        this->LFCallback(this, this->target_class);                        
                    }
                }
                break;
        }
        
    }
    else {
        if (this->bufferFullCallback !=  0){
            this->bufferFullCallback(this, this->target_class);
        }
    }
        
    } 
//----------------------------------------------------------------------------//
    /*
     * Overload for put String method
     */
    virtual char putString(char * s) = 0;
    virtual char putString(std::string s) = 0;
    virtual char putString(const char * s)= 0;
    /*
     * Overload for putByte method.
     */
    virtual char putByte(void *data) = 0;
    virtual char putByte(char data) = 0;
    virtual char putByte(unsigned char data) = 0;
    virtual char putBytes(void *data, int len) = 0;
    /*
     * Each device will have different  start/stop/init condition
     */
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void init()=0;
};


#endif /* ICOMMUNICATION_HPP */

