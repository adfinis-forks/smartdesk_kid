/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZmqReplier.hpp
 * Author: thanh
 *
 * Created on April 2, 2016, 1:13 AM
 */

#ifndef ZMQREPLIER_HPP
#define ZMQREPLIER_HPP
#include"iZmq.hpp"
#define MAX_RX_BUFFER 2048
void* doRepThread(void *local_class);
int ZmpReplierCallback(void *local_class, void * target_class);
int ZmqRepBufferFullcallback(void *local_class, void * target_class);
int ZmqRepErrorCallback(void *local_class, void * target_class);


class ZmqReplier:public iZmq{
public:
    uint16_t rx_data_length;
    uint8_t rx_buf[MAX_RX_BUFFER];
    uint16_t max_rx_buf;
    uint16_t max_tx_buf;
    uint16_t byte_count; 
    int byte_index;
    void * replier;
    char enable;
    bool status = false;
    struct timespec tim,tim2; 
    int (*bufferFullCallback)(void*, void*);
    int (*LFCallback)(void*, void*);
    int (*ErrorCallback)(void*, void*);
    int (*timeoutCallback)(void*, void*);
    void *target_class;
    pthread_t rep_thread;
    uint8_t  terminator_type;
    int setupThread(void*(*fcn)(void*), void* _icom);
    //char isReady();
    virtual void init();
    int Init(std::string endPoint);
    int Init(std::string endPoint, void *target_class);
    bool isReady();
//    virtual int  getBytes(char** p);

    virtual char putByte(void* data);
    
    virtual char putByte(char data);
    
    virtual char putByte(unsigned char data);

    virtual char putBytes(void* data, int len);

    virtual char putChar(char c);

    virtual char putString(char* s);

    virtual char putString(std::string s);

    virtual char putString(const char* s);

    virtual int receiveMore();

    virtual int receiveOne();

    virtual int sendMore();

    virtual int sendNull();

    virtual int sendOne(void *data, int len );
  
    virtual int setup(void* ctx);

    virtual void  start();

    virtual void stop();
    virtual ~ZmqReplier();

    virtual int bind();

    virtual int unBind();
    ZmqReplier();
    
    
    





    






    

















    
    
            

};
#endif /* ZMQREPLIER_HPP */

