/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Desk.hpp
 * Author: thanh
 *
 * Created on March 31, 2016, 12:22 AM
 */

#ifndef DESK_HPP
#define DESK_HPP

#include "LinearDevice.hpp"
#include "COMMUNICATION/iCommunication.hpp"
#include "ZMQ/ZmqReplier.hpp"
#include "Device/AutonomousCommFrame.h"
#include <string.h>
// Data Package Define
#define ZMQ_START_BYTE1_IND      0
#define ZMQ_START_BYTE2_IND      1
#define ZMQ_LENGTH_IND           2
#define ZMQ_PRODUCT_ID_IND       3
#define ZMQ_COMPONENT_ID_IND     4
#define ZMQ_INSTRUCTION_IND      5
#define ZMQ_ADDRESS_IND          6

// Product ID Define
#define PERSONAL_ROBOT      0x01
#define CLOCK               0x02
#define SMART_DESK          0x03
#define IOS                 0x04

// Component ID Define
#define PAN_MOTOR           0x01
#define TILT_MOTOR          0x02
#define UP_DOWN_MOTOR       0x03

// Instruction Define
#define INS_READ            0x01
#define INS_WRITE           0x02
#define INS_RETURN          0x03
//
#define     ZMQ_START_BYTE1         0xFF
#define     ZMQ_START_BYTE2         0xFF
#define     ZMQ_STOP_BYTE1          0xFA
#define     ZMQ_STOP_BYTE2          0xFA
#define     ZMQ_TX_LENGTH           60
#define     ZMQ_RX_LENGTH           60


#define     MAX_RX_LENGTH            512
#define     MAX_TX_LENGTH            20
#define NOT_UPDATE          0
#define UPDATED             1
#define TRUE                1
#define FALSE               0
#define ADDRESS             0
#define VALUES              1
#define ON                  2
#define OFF                 1

#define DEFAULT_HEIGHT      1000
#define EEPROM_SECOND_USE   100
#define DEFAULT_VERSION         2

//
#define UID                      0x00010001
class State{
private:
    State();;  
public:
    static State *m_instance;
    static State * getInstance();;
    static const uint16_t STATE_STOP = 0;
    static const uint16_t STATE_UP = 1;
    static const uint16_t STATE_DOWN = 2 ;
    static const uint16_t STATE_FREE = 3;
    static const uint16_t STATE_ERROR = 4;
    static const uint16_t STATE_SET_DATA = 5;
    static const uint16_t STATE_GET_DATA = 6 ;
    static const uint16_t STATE_RETURN = 7;
    static const uint16_t STATE_SINGLE_MOTOR_RESET = 8;
    static const uint16_t STATE_BUSY = 9;
    static const uint16_t STATE_SAVE2POS1 = 10;
    static const uint16_t STATE_MOVE2POS1 = 11;
    static const uint16_t STATE_SAVE2POS2 = 12;
    static const uint16_t STATE_MOVE2POS2 = 13;
    static const uint16_t STATE_SAVE2POS3 = 14;
    static const uint16_t STATE_MOVE2POS3 = 15;
    static const uint16_t STATE_SAVE2POS4 = 16;
    static const uint16_t STATE_MOVE2POS4 = 17;
    
    
    
    static const uint16_t STATE_GET_MINMAX = 18;
    static const uint16_t STATE_DUAL_MOTOR_RESET = 19;
    static const uint16_t STATE_ALL = 255;
};
struct mDeskPacket{
    mDeskPacket();;
    uint16_t IND_START_BYTE1;
    uint16_t IND_START_BYTE2;
    uint16_t IND_FUNCTION ;
    uint16_t IND_DATA_LENGTH;
};
struct mCtrBoxFcn{
    mCtrBoxFcn();
    uint16_t   CURENT_HEIGHT;
    uint16_t   ERROR_CODE;
    uint16_t   RESET_MODE;
    uint16_t   MAX_MIN_HEIGHT;
    uint16_t RX_START_BYTE1;
    uint16_t RX_START_BYTE2;
    uint16_t RX_STOP_BYTE;
} ;
class Mode{
private:
    Mode(){};
public:
    static Mode * m_instance;
    static Mode * getInstance(){
        if(m_instance == NULL) m_instance = new Mode();
        return m_instance;
    };
    static const uint16_t MODE_AUTO = 1 ;
    static const uint16_t MODE_MANUAL = 0;
};
class Desk : public LinearDevice{
public:
    virtual ~Desk();
    Desk();
    //int data[(const int)ControlTableAddress.LengthOfAddr]; 
    uint16_t *data = new uint16_t[ControlTableAddress.LengthOfAddr];
    void initDataTable();
    //Interface function
    ZmqReplier mRep;
    uint16_t preState;
    uint16_t getPreState(){
        return preState;
    }

    void setPreState(uint16_t preState) {
        this->preState = preState;
    }
    // Getter and setter
    uint16_t getCurrentHeight() ;

    void setCurrentHeight(uint16_t currentHeight);

    virtual uint16_t getMode() ;

    virtual void setMode(uint16_t mode);

    uint16_t getPos1Height() ;

    void setPos1Height(uint16_t pos1Height);

    uint16_t getPos2Height() ;

    void setPos2Height(uint16_t pos2Height);

    uint16_t getPos3Height() ;

    void setPos3Height(uint16_t pos3Height);

    uint16_t getPos4Height() ;

    void setPos4Height(uint16_t pos4Height);

    uint16_t getSetpointHeight() ;

    uint16_t setSetpointHeight(uint16_t setpointHeight);

    virtual  uint16_t getState() ;

    virtual  void setState(uint16_t state);
    void SetMinHeight(uint16_t minHeight){
        this->data[ControlTableAddress.MinHeight] = minHeight;
    }
    void SetMaxHeight(uint16_t maxHeight){
        this->data[ControlTableAddress.MaxHeight] = maxHeight;
    }
    int GetMinHeight(){
        return this->data[ControlTableAddress.MinHeight];
    }
    int GetMaxHeight(){
        return this->data[ControlTableAddress.MaxHeight];
    }
    //Khai bao cac ham ke thua tu class cha
    virtual void moveUp() = 0;
    virtual void moveDown() = 0;
    virtual void stopMove() = 0;
    virtual void singleMotorReset() = 0;
    virtual void setHeight(uint16_t height) = 0;
    virtual uint16_t  getHeight() = 0;
    virtual void saveToPos1(uint16_t pos) = 0;
    virtual void moveTopos1() = 0;
 
    virtual void saveToPos2(uint16_t pos) = 0;
    virtual void moveTopos2() = 0;
    
    virtual void saveToPos3(uint16_t pos) = 0;
    virtual void moveTopos3() = 0;
    
    virtual void saveToPos4(uint16_t pos) = 0;
    virtual void moveTopos4() = 0;

            
};


#endif /* DESK_HPP */

