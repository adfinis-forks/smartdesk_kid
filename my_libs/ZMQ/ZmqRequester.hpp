/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZmqRequester.hpp
 * Author: ThanhLe
 * email: qthanh_le@hotmail.com
 *
 * Created on April 4, 2016, 3:01 AM
 */

#ifndef ZMQREQUESTER_HPP
#define ZMQREQUESTER_HPP

#include "iZmq.hpp"
#include "ZmqReplier.hpp"
void * doReqThread(void *local_class);
int ZmqReqCallback(void *local_class, void *target_class);
int ZmqReqBufferFullcallback(void *local_class, void *target_class);
int ZmqReqErrorCallback(void *local_class, void *target_class);
class ZmqRequester:public iZmq{
    
public:
    friend void *doReqThread(void *data);
    void * requester;
    uint8_t rx_buf[MAX_RX_BUFFER];
    uint16_t rx_buf_length = 0;
    bool status = false;
    uint16_t max_rf_buf;
    struct timespec tim,tim2; 
    int (*bufferFullCallback)(void*, void*);
    int (*LFCallback)(void*, void*);
    int (*ErrorCallback)(void*, void*);
    int (*timeoutCallback)(void*, void*);
    int setupThread(void*(*fcn)(void*), void* _icom);
    uint8_t terminator_type;
    void *target_class = NULL;
    bool isReady(){
        return status;
    }
    void Delete();
//Interface funtion
    virtual void init();
    virtual int Init(std::string endPoint);
    virtual int Init(std::string endPoint, void *target_class);
    virtual int setup(void* ctx);

    virtual void start();

    virtual void stop();

    virtual int sendMore();

    virtual int sendOne(void* data, int len);

    virtual int sendNull();

    virtual int receiveOne();

    virtual int receiveMore();

    virtual int bind();

    virtual int unBind();

//    virtual int getBytes(char** p);

    virtual char putByte(void* data);
    
    virtual char putByte(char data);
    
    virtual char putByte(unsigned char data);

    virtual char putBytes(void* data, int len);

    virtual char putChar(char c);

    virtual char putString(const char* s);

    virtual char putString(std::string s);

    virtual char putString(char* s);
    ZmqRequester();
    ~ZmqRequester();

private:
    pthread_t req_thread;













    
    



    
    






















    
    

    


};


#endif /* ZMQREQUESTER_HPP */

