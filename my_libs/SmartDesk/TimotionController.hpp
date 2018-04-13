/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimotionController.hpp
 * Author: thanh
 *
 * Created on March 31, 2016, 12:39 AM
 */

#ifndef TIMOTIONCONTROLLER_HPP
#define TIMOTIONCONTROLLER_HPP

#include "Desk.hpp"
#include "ZMQ/ZmqReplier.hpp"
#include "Device/TimboxCommFrame.h"
#include "COMMUNICATION/Uart.hpp"
#include "TIMER/Timer.hpp"
#include "LCD/MyLcd.hpp"
#include <stdio.h>
#define TIMOTION_MAX_HEIGHT 1300
#define TIMOTION_MIN_HEIGHT 550
#define MAX_DELAY_COUNT 20 
#define ERROR_THRESHOLD 1 
typedef enum {
  CONTROLLER_FREE = 0,
  CONTROLLER_BUSY

}WorkStatus_Typedef;
// For Timotion Controller
void* TimotionControllerThread(void*data);

int   TimotionControlCallback(void* local_class, void * taget_class);


// For CheckHeightTimer
//void *TimotionCheckHeightThread(void *data );

int  TimotionCheckHeightCallback(void *local_class, void * target_class);

// For CheckConnectTimer
//void *TimotionCheckConnectThread(void *data);

int  TimotionCheckConnectCallback(void *local_class, void * target_class);

//// 
//void* UpdateHeightThread(void *data);
int UpdateHeightTimerCallbackFcn(void *local_class, void * target_class);




class TiMotionController:public Desk{
private:
    uint8_t upCmd[5]    = {0xD8, 0xD8, 0x66, 0x02, 0x02} ;   //  {0xD8, 0xD8, 0x66, 0x02, 0x02};
    uint8_t downCmd[5]  = {0xD8, 0xD8, 0x66, 0x01, 0x01} ; //  {0xD8, 0xD8, 0x66, 0x01, 0x01};
    uint8_t stopCmd[5]  = {0xD8, 0xD8, 0x66, 0x00, 0x00} ; //  {0xD8, 0xD8, 0x66, 0x00, 0x00};
    uint8_t pos1Cmd[5]  = {0xD8, 0xD8, 0x66, 0x04, 0x04}; //  {0xD8, 0xD8, 0x66, 0x04, 0x04};
    uint8_t pos2Cmd[5]  = {0xD8, 0xD8, 0x66, 0x08, 0x08}; //  {0xD8, 0xD8, 0x66, 0x08, 0x08};
    uint8_t pos3Cmd[5]  = {0xD8, 0xD8, 0x66, 0x10, 0x10}; //  {0xD8, 0xD8, 0x66, 0x10, 0x10};
    uint8_t pos4Cmd[5]  = {0xD8, 0xD8, 0x66, 0x20, 0x20}; //  {0xD8, 0xD8, 0x66, 0x20, 0x20};
    uint8_t saveCmd[5]  = {0xD8, 0xD8, 0x66, 0x40, 0x40};//   {0xD8, 0xD8, 0x66, 0x40, 0x40};
    uint8_t resetCmd[5] = {0xD8, 0xD8, 0x66, 0x03, 0x03}; //  {0xD8, 0xD8, 0x66, 0x03, 0x03};
    int posError;
    uint8_t longBuf[1024];
    uint16_t longBufInd;
    TimboxFrameStatus frameStatus_cur; // store status data from RX packet
    TimboxFrameStatus frameStatus_pre; // store status data from RX packet
    int updateControllerStatus(WorkStatus_Typedef _status);
    TiMotionController();
    pthread_mutex_t mutex1 ;
    struct timespec tstart={0,0}, tend={0,0};
#define LOG_LENGTH 4
    double logData[3][LOG_LENGTH];
    int log_index = 0;
public:
    static TiMotionController * tim_instance;
    static TiMotionController * getInstance();
    MyLcd *mLcd = MyLcd::getInstance();
    WorkStatus_Typedef controlState;
    uint8_t connectStatus = 0;;
    uint8_t disconnectStatus;
    uint16_t movePos1Count,movePos2Count,movePos3Count,movePos4Count;
    uint16_t subState;
    int saveCount;
    void *target_class;
    void PrintConnectStatus();
    void PrintDisconnectStatus();
    int oldHeight;
    uint8_t RxDataFlag;
    Timer *checkHeightTimer; 
    Timer *checkConnectTimer;
    TimotionRxStruct_Typedef rxFrameObj;
    TimotionTxStruct_Typedef txFrameObj;
    int uart_flag;
    
    virtual ~TiMotionController();
    Uart mUart;
    ZmqReplier mRep;    
    int check;
    int ID;
    int delayTimeCount;
    int firtLog = 1;
    int error = 0;
    int down_error = 0;
    uint16_t hold_height;
    int offset = 0;
    
    /*
     get the information from RX packet 
     */
    
    TimboxFrameStatus getBoxStatus();
    void SetBoxStatus(TimboxFrameStatus _status);
    virtual void moveUp()  ;
    virtual void moveDown() ;
    virtual void stopMove() ;
    virtual void singleMotorReset() ;
    void dualMotorReset();
    virtual void setHeight(uint16_t height) ;
    virtual uint16_t getHeight() ;
    virtual void saveToPos1(uint16_t pos) ;
    void saveToPos1();;
    void saveToPos2();;
    void saveToPos3();;
    void saveToPos4();;
    virtual void moveTopos1() ;
    virtual void saveToPos2(uint16_t pos) ;
    virtual void moveTopos2() ;
    virtual void saveToPos3(uint16_t pos) ;
    virtual void moveTopos3() ;
    virtual void saveToPos4(uint16_t pos) ;
    virtual void moveTopos4() ;    
    virtual void initData();
    virtual void Start();
    virtual void Stop();
    virtual void Update();
    void updateLogData(uint16_t height,uint16_t setpoint);
    /*
     * User
     */
    int AddToLongBuf(uint8_t *data, uint16_t length); 
    int GetHeightFromBuffer();
    int Init(std::string endPoint, std::string serialName, int baud);
    int Init(std::string endPoint, UART_InitTypedef * uartInitStruct); 
    int Init(const char * fileName);
    int Init(const char * fileName, std::string s);
    static char isChecksumOk(void *data, int len);
    int getAction(void *data, int len,uint8_t *dataOut);
    virtual uint16_t getChecksum(void *data, int len);    
    int getDataFromUart(uint8_t *data, int begin);;
    int getPacketBegin(uint8_t *data, int len);;
    int isValidPacket(uint8_t *data, int begin, int end);
    void updateSetpointHeight()
    int Run();
    
    void ProcessUartCallback();
    void ProcessZmqRepCallback();
};
#endif /* TIMOTIONCONTROLLER_HPP */

