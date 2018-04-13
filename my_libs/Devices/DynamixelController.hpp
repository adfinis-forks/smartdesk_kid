/*
 * The MIT License
 *
 * Copyright 2017 thanh.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   DynamixelMotor.hpp
 * Author: thanh
 *
 * Created on May 2, 2017, 8:50 PM
 */

#ifndef DYNAMIXELCONTROLLER_HPP
#define DYNAMIXELCONTROLLER_HPP
#include "COMMUNICATION/Uart.hpp"
#include "Device/DynamixelMotor.hpp"
#include <stddef.h>
#include "TIMER/Timer.hpp"
#include <list>
#include <array>
#include <stack>
#include <assert.h>
using namespace std;
void *dlxControllerThread(void *data);
int dlxControllerCallback(void *data);
class DynamixelController{
public:
    
    static DynamixelController *instance;
    static DynamixelController * getInstance();
    /*Setup the callback function called in thread*/
    void SetControlThreadFcn(int(*fcn)(void*) );
    /*Setup the thread function */
    int setupThread(void*(*fcn)(void*),void *data);
    list<DynamixelMotor*> motors_list;
    DynamixelMotor *motors[10];
    DynamixelController();
    void init(std::string dev_name, uint32_t baudrate);
    
    void init(UART_InitTypedef *uart_init);
    
    void updateMotorList();
    void disable();
    void enable();
    void start();
    virtual ~DynamixelController() ;


    void addDevice(DynamixelMotor *motor);
    void removeDevice(DynamixelMotor *motor);
    void getInfor();
    void setAllMotor(std::string cmd, double value);
    struct timespec tim,tim2;
    void AddToLongBuf(uint8_t* data, uint16_t length);
    void ProcessUartCallback();
    void updateInfor();
    bool checkRxChecksum(uint8_t *data, uint8_t length);
    void printRxPacket(uint8_t *data, uint8_t length);
    Timer mTimer;
    
protected:
private:    
    Uart mUart;
    pthread_t controlThread;
    int (*controlThreadFcn)(void *);
    uint8_t longBuf[1024];
    uint16_t longBufInd;
    bool uart_flag = false;
    bool RxDataFlag = false;
    uint8_t nDevice;
    
    
    
};


#endif /* DYNAMIXELMOTOR_HPP */

