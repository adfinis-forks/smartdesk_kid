/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iZmq.hpp
 * Author: ThanhLe
 *
 * Created on April 1, 2016, 10:33 AM
 */

#ifndef ZMQ_HPP
#define ZMQ_HPP
#include <zmq.h>
#include <iostream>
#include <string.h>
#include "Connectivity/iCommunication.hpp"
class ZmqContext{

  ZmqContext(); 
public:
    static ZmqContext *m_instance;
    
    static ZmqContext * getInstance();
    void *context;
    int setup();
    int setNumberOfThread(int value);
    int setMaxSocket(int value);
    int setIPv6(int value);
    int shutdown();
    int terminate();
    int destroy();
    
    ~ZmqContext();
};

class iZmq {
public:
//    ZmqContext mContext;
    int timeOut;
    std::string endPoint;
    virtual int setup(void *ctx) = 0;
    virtual int sendMore() = 0;
    virtual int sendOne(void *data, int len) = 0;
    virtual int sendNull() = 0;
    virtual int receiveOne() = 0;
    virtual int receiveMore() = 0;
    virtual void init()=0;
protected:    
    virtual int bind() = 0;
    virtual int unBind() = 0;
public:
    virtual ~iZmq();
    iZmq();
//    iZmq(ZmqContext zmqContext);

    virtual char putByte(void* data)= 0;
    
    virtual char putByte(char data) = 0;
    
    virtual char putByte(unsigned char data) = 0;

    virtual char putBytes(void* data, int len)= 0;

    virtual char putChar(char c)= 0;

    virtual char putString(char* s)= 0;
    
    virtual char putString(const char* s)= 0;

    virtual char putString(std::string s)= 0;
    virtual void start()= 0;
    virtual void stop()= 0;
    /*Khai Bao Setter Getter*/


    void setEndPoint(std::string endPoint);
    std::string getEndPoint() const;
    
    void setTimeOut(int timeOut);

    int getTimeOut() const;
    


};

#endif /* ZMQ_HPP */

