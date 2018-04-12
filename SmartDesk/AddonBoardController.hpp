#ifndef __ADDON_BOARD_CONTROLLER_HPP
#define __ADDON_BOARD_CONTROLLER_HPP
#include "COMMUNICATION/Uart.hpp"
#include "ZMQ/ZmqReplier.hpp"
#include "SmartDesk/Desk.hpp"
#include "Device/AutonomousCommFrame.h"

class AddonBoardController:  public Desk{
private:
    int getDataFromUartBuf(void);
    AutonomousCommFrame_Typedef rx_obj;

public:
    Uart mUart;
    ZmqReplier mRep;
    AddonBoardController();
    virtual ~AddonBoardController();
    int Run();
    int Update();
    int Init(std::string endPoint, const char * serialName, int baud);
    int Init(std::string endPoint, UART_InitTypedef * uartInitStruct);
    int Init(const char * fileName);
    int Init(const char * fileName, std::string dev_name);
    virtual void moveUp(  );;
    virtual void moveDown(  );;
    virtual void stopMove();;
    virtual void singleMotorReset(); ;

    virtual void setHeight(uint16_t height);;
    virtual uint16_t getHeight();;
    virtual void saveToPos1(uint16_t pos);;
    virtual void saveToPos2(uint16_t pos);;
    virtual void saveToPos3(uint16_t pos);;
    virtual void saveToPos4(uint16_t pos);;
    
    
    virtual void moveTopos1();;
    virtual void moveTopos2();;
    virtual void moveTopos3();;
    virtual void moveTopos4();;
    
    virtual void control();;
    virtual void Start();;
    virtual void Stop();;
    virtual int getAction(void *data, int len,uint8_t *dataOut);;
    virtual uint16_t getChecksum(void *data, int len);;
    static char isChecksumOk(void* data, int len);;
    
    /*
     User
     */
    void ProcessUartCallback();
    void ProcessZmqRepCallback();    
    
    
};

#endif