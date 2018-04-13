/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Uart.hpp
 * Author: thanh
 *
 * Created on March 31, 2016, 1:39 AM
 */

#ifndef UART_HPP
#define UART_HPP
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include<time.h>
#include<inttypes.h>
#include <sys/ioctl.h>
#define MAX_RX_BUFFER 2048
typedef struct {
    uint32_t baudRate;
    uint8_t  terminator_type;
    std::string  deviceName;
    uint16_t  byte_count;
    
    uint8_t custom_byte;
    uint8_t *custom_bytes;
    uint8_t custom_bytes_length;
    void * target_class;
}UART_InitTypedef;
/*
 
 */
void* do_uart_thread(void *local_data);
int uartLfCallkback(void *local_data, void *target_class);
int uartBufferFullCallback(void *local_data, void *target_class);
int uartErrorCallback(void *local_data, void *target_class);
int uartTimeoutCallback(void *local_data, void *target_class);
class Uart{
private:  
    // Variables
    int fd;
    uint32_t baudrate_;
    int baudrate_flag;
    uint32_t timeout_ns;
    pthread_t uart_thread;
    bool status = false;
    
    // Functions    
    int config(int fd, int baud);     
    int getCFlagBaud(uint32_t baudrate );
    
    void checkCallBack(void);

public:
    // Constants
    static const uint8_t TERMINATOR_LF                  = 1;
    static const uint8_t TERMINATOR_CR                  = 2;
    static const uint8_t TERMINATOR_CRLF                = 3;
    static const uint8_t TERMINATOR_CUSTOM_BYTE         = 4;
    static const uint8_t TERMINATOR_FAFA                = 5;
    static const uint8_t TERMINATOR_COUNT               = 6;
    static const uint8_t TERMINATOR_TIMEOUT             = 7;
    static const uint8_t TERMINATOR_CUSTOM_BYTES        = 8;
    static const uint8_t TERMINATOR_POLL                = 9;  
    static const uint8_t  LATENCY_TIMER                  = 4 ; 
    //Variables
    void            *target_class;
    uint8_t         terminator_type;
    std::string     device_name;
    struct timespec tim,tim2;
    uint8_t         rx_byte;
    uint16_t        max_rx_length;
    uint16_t        byte_index = 0;
    uint8_t         rxBuf[MAX_RX_BUFFER];
    uint8_t         terminator_byte;
    uint8_t         terminator_bytes[MAX_RX_BUFFER];
    uint16_t        terminator_bytes_length;
    uint16_t        byte_count;
    uint16_t        rx_data_length;
    double          packet_start_time;
    double          packet_timeout;  
    double          tx_time_per_byte;
    int (*bufferFullCallback)(void*, void *);
    int (*lfCallback)(void*, void *);
    int (*errorCallback)(void*, void *);
    int (*timeoutCallback)(void*, void *);
    // Functions
    
    int writeApi(void* data, uint8_t len);
    int openApi(const char *device,int baud_flag);
    int openApi(std::string deviceName, int baud_flag); 
    int openApi();
    uint16_t readApi(uint8_t *data);
    uint16_t readApi(uint8_t *data, uint16_t length);
    uint16_t readApiTimeout(uint8_t *data,uint16_t length);
    uint16_t readApiTimeoutMs(uint8_t *data,uint16_t time_out_ms);
    int closeApi();
    int clearApi();
    void processUartThread(void);
    int setMinimumBytesToRead(uint8_t n_bytes);
    uint8_t getMinimumBytesToRead();
    void enableNonblockingRead();   
    void enableBlockingRead();
    int getData(uint8_t data);   
    bool isReady();
    int setupThread(void*(*fcn)(void*), void* _icom);
    double getCurrentTime();    
    void setPacketTimeout(uint16_t packet_length);   
    void setPacketTimeoutMs(uint16_t timeout_ms);   
    double getTimeSinceStart();   
    bool isPacketTimeout();     
    uint32_t getBaudRate();  
    void setBaudRate(uint32_t baudRate);   
    virtual ~Uart() ; 
    Uart();   
    Uart(std::string devName, uint32_t baudRate);   
    int Init(const char *devName, uint32_t baudRate);  
    int Init(std::string devName, uint32_t baudRate);
    int Init(UART_InitTypedef * uartInit);
    uint16_t getBytesAvailable();
    int printCurrentConfig();
    void addDataToBuffer(uint8_t *data, uint16_t length);
    //Interface functions inherited from iSerial
    virtual char putString(char * s);   
    virtual char putString(std::string s);  
    virtual char putString(const char * s);
    virtual char putByte(void *data);  
    virtual char putByte(char data);  
    virtual char putByte(unsigned char data);  
    virtual char putBytes(void *data, int len); 
    virtual void start(); 
    virtual void stop(); 
    virtual void init();   
};


#endif /* UART_HPP */

