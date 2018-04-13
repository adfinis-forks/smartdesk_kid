/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DeskRequester.hpp
 * Author: thanh
 *
 * Created on May 30, 2017, 1:11 AM
 */

#ifndef DESKREQUESTER_HPP
#define DESKREQUESTER_HPP
#include "ZMQ/ZmqRequester.hpp"
#include"SmartDesk/Desk.hpp"
#define  DESK  "ipc:///tmp/desk_control:9999"
class DeskRequester{
public:
    unsigned char tx[20],rx[20];
    unsigned char len = 0;
    unsigned char b,c,addr;
    int a;
    unsigned short h,sum;
    char s[20],str[20];
    ZmqRequester mReq;
    void Init();
    virtual ~DeskRequester();;
    DeskRequester();;    
    void Run();
};


#endif /* DESKREQUESTER_HPP */

