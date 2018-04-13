#ifndef __ADDON_BOARD_CONTROLLER_HPP
#define __ADDON_BOARD_CONTROLLER_HPP
#include "COMMUNICATION/Uart.hpp"
#include "ZMQ/ZmqReplier.hpp"
#include "SmartDesk/Desk.hpp"
#include "Device/AutonomousCommFrame.h"

class AddonBoardController:  public Desk{
    

public:
    static const  char TURN_ON_COOL_FAN = 'a';
    static const  char TURN_OFF_COOL_FAN = 'b';
    static const  char TURN_ON_HOT_FAN = 'c';
    static const  char TURN_OFF_HOT_FAN = 'd';
    static const  char TURN_ON_LFOOT = 'e';
    static const  char TURN_OFF_LFOOT = 'f';
    static const  char TURN_ON_RFOOT = 'g';
    static const  char TURN_OFF_RFOOT = 'h';

    static const  char GET_SENSOR = 'i';
    static const  char GET_TEM = 'j';
    static const  char TURN_ON_LED = 'k';
    static const  char TURN_OFF_LED = 'l';
    static const char   STOP_ALL = 'm';
    Uart mUart;
    ZmqReplier mRep;
    AddonBoardController();
    uint16_t tem;
    uint8_t sensor;
    virtual ~AddonBoardController();
    int Run();
    int Update();
    int Init(std::string endPoint, const char * serialName, int baud);
    int Init(std::string endPoint, UART_InitTypedef * uartInitStruct);
    int Init(const char * fileName);
    int Init(const char * fileName, std::string devName);
    virtual void moveUp(  ){};
    virtual void moveDown(  ){};
    virtual void stopMove() {};
    virtual void reset(){} ;

    virtual void setHeight(uint16_t height) {};
    virtual uint16_t getHeight() {};
    virtual void saveToPos1(uint16_t pos) {};
    virtual void saveToPos2(uint16_t pos) {};
    virtual void saveToPos3(uint16_t pos) {};
    virtual void saveToPos4(uint16_t pos) {};
    
    
    virtual void moveTopos1() {};
    virtual void moveTopos2() {};
    virtual void moveTopos3() {};
    virtual void moveTopos4() {};
    
    virtual void control()  {};
    virtual void Start();;
    virtual void Stop();;
    virtual int getAction(void *data, int len,uint8_t *dataOut){
        uint8_t *rxBuf = (uint8_t*)data;
        memcpy(rxBuf,data,len);
        int txLength = 0;
        int tem = State::STATE_FREE;
        int temMode = rxBuf[ ZMQ_INSTRUCTION_IND];
        int temAddr = rxBuf[ ZMQ_ADDRESS_IND];
        uint16_t temData;
        int rxEnd = len;
        if(rxBuf[ZMQ_LENGTH_IND] == 2){ // neu nhan duoc du lieu co 2 byte
            temData = ((uint16_t)rxBuf[rxEnd - 5])*256  + (uint16_t)rxBuf[rxEnd - 4];
        } else{
            temData = rxBuf[rxEnd - 3];
        }
        if(temMode == INS_WRITE){
            this->data[temAddr] = temData;
            if (temAddr == ControlTableAddress.Up){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_ON_LFOOT);                
                this->mUart.putBytes(s,2);
                printf("-I- Up\n");
            }
            else if (temAddr == ControlTableAddress.Down){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_OFF_LFOOT);                
                this->mUart.putBytes(s,2);
                printf("-I- Down\n");
            }
            else if (temAddr == ControlTableAddress.Stop){
                char s[] = "\r\n";
                this->mUart.putByte(this->STOP_ALL);                
                this->mUart.putBytes(s,2);
                printf("-I- Stop\n");
            }
            else if (temAddr == ControlTableAddress.SaveToPos3){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_ON_RFOOT);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn On Right Foot\n");
            }
            else if (temAddr == ControlTableAddress.SetPointHeight){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_OFF_RFOOT);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn Off Right Foot\n");
            }
            else if (temAddr == ControlTableAddress.SaveToPos1){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_ON_COOL_FAN);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn On Cool Fan\n");
            }
            else if (temAddr == ControlTableAddress.MoveToPos1){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_OFF_COOL_FAN);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn Off Cool Fan\n");
            }
            else if (temAddr == ControlTableAddress.SaveToPos2){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_ON_HOT_FAN);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn On Hot Fan\n");
            }
            else if (temAddr == ControlTableAddress.MoveToPos2){
                char s[] = "\r\n";
                this->mUart.putByte(this->TURN_OFF_HOT_FAN);                
                this->mUart.putBytes(s,2);
                printf("-I- Turn Off Hot Fan\n");
            }
        } 
        else if(temMode == INS_READ){
            txLength = ZMQ_ADDRESS_IND + 4 + 2;
            uint8_t txBuf[txLength];

            txBuf[ZMQ_START_BYTE1_IND] = ZMQ_START_BYTE1;
            txBuf[ZMQ_START_BYTE2_IND] = ZMQ_START_BYTE2;
            txBuf[ZMQ_LENGTH_IND] = 2;
            txBuf[ZMQ_ADDRESS_IND] = temAddr;
            temData = this->data[temAddr];
            txBuf[ZMQ_ADDRESS_IND + 1] = (uint8_t)(temData >> 8);
            txBuf[ZMQ_ADDRESS_IND + 2] = (uint8_t)temData;

            #ifdef CONSOLE
                printf("TX Length = %d\n",txLength);
            #endif
            txBuf[txLength - 3] = this->getChecksum(txBuf,txLength);
            txBuf[txLength - 1] = ZMQ_STOP_BYTE1;
            txBuf[txLength - 2] = ZMQ_STOP_BYTE2;
            tem = State::getInstance()->STATE_GET_DATA;
            memcpy(dataOut,txBuf,txLength);
        }
        this->setState(tem);
        return txLength; 
    };
    virtual uint16_t getChecksum(void *data, int len){
        uint8_t *x = (uint8_t *)data;
        uint16_t sum = 0;
        for (int i = ZMQ_LENGTH_IND; i < len -3;i++){
            sum += x[i];
        }
        if(sum > 255) sum = sum%255;
        return sum;
    };
    static char isChecksumOk(void* data, int len) {
        uint8_t *rxBuf = (uint8_t *)data;
        int rxBegin = 0;
        int rxEnd = len-1;

        short sum = rxBuf[rxBegin + ZMQ_LENGTH_IND] +
        rxBuf[rxBegin + ZMQ_PRODUCT_ID_IND] +
        rxBuf[rxBegin + ZMQ_COMPONENT_ID_IND] +
        rxBuf[rxBegin + ZMQ_INSTRUCTION_IND] + 
        rxBuf[rxBegin + ZMQ_ADDRESS_IND];
        if(rxBuf[rxBegin + ZMQ_LENGTH_IND] > 0){
            int tem = rxBegin + ZMQ_ADDRESS_IND +1;
            for(int i = tem; i < tem + rxBuf[rxBegin + ZMQ_LENGTH_IND]; i++){
                sum += rxBuf[i];
            }
        }
        if(sum > 255){
            sum = sum%255;
        }
        if(sum == rxBuf[rxEnd - 2]) 
            return true;
        else return false;

    };
    
    /*
     User
     */
    void ProcessUartCallback();
    void ProcessZmqRepCallback();    
    
    
};

#endif