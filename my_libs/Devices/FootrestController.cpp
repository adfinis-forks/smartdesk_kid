#include "Device/FootrestController.hpp"

AddonBoardController::AddonBoardController() {

}
AddonBoardController::~AddonBoardController(){

}

int AddonBoardController::Init(std::string endPoint, const char* serialName, int baud) {

}

int AddonBoardController::Init(std::string endPoint, UART_InitTypedef* uartInitStruct) {
    assert(this->mUart.Init(uartInitStruct) == 0);
    assert(this->mRep.Init(endPoint, this) == 0);    
}

int AddonBoardController::Init(const char* fileName) {
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_CRLF;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos;
    assert(fd > 0);
    if (fd > 0){
        printf("[I]Read data.dat successfull\n"); 
       char buff[255];
        int data;
        fscanf(fd,"MinHeight:%s\n",buff);
        minHeight = buff;
        fscanf(fd,"MaxHeight:%s\n",buff);
        maxHeight = buff;
        fscanf(fd,"SitPos:%s\n",buff);
        sitPos = buff;
        fscanf(fd,"StandPos:%s\n",buff);
        standPos = buff;
        fscanf(fd,"SerialDevice:%s\n",buff);
        dName = buff;
        fscanf(fd,"ZmqIPC:%s\n",buff);
        epoint = buff;      
        fscanf(fd,"Controller:%s\n",buff);
        
        uartInit.deviceName = dName;
        fscanf(fd,"Unit:%s\n",buff);
        this->Init(epoint,&uartInit);
    }    
}

int AddonBoardController::Init(const char* fileName, std::string devName) {
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_CRLF;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos;
    assert(fd > 0);
    if (fd > 0){
        printf("[I]Read data.dat successfull\n"); 
       char buff[255];
        int data;
        fscanf(fd,"MinHeight:%s\n",buff);
        minHeight = buff;
        fscanf(fd,"MaxHeight:%s\n",buff);
        maxHeight = buff;
        fscanf(fd,"SitPos:%s\n",buff);
        sitPos = buff;
        fscanf(fd,"StandPos:%s\n",buff);
        standPos = buff;
        fscanf(fd,"SerialDevice:%s\n",buff);
        dName = devName;
        fscanf(fd,"ZmqIPC:%s\n",buff);
        epoint = buff;      
        fscanf(fd,"Controller:%s\n",buff);
        
        uartInit.deviceName = dName;
        fscanf(fd,"Unit:%s\n",buff);
        this->Init(epoint,&uartInit);
    }
}

void AddonBoardController::Start() {
    this->mRep.bind();
    this->mUart.start();
    this->mRep.start();
    this->Enable();
}

void AddonBoardController::Stop() {
    this->mUart.stop();
    this->mRep.stop();
    this->Disable();
}

void AddonBoardController::ProcessUartCallback() {
    switch(this->mUart.rxBuf[this->mUart.rx_data_length - 3]){
        case this->GET_SENSOR:            
            this->sensor = atoi((const char*)this->mUart.rxBuf);
            this->data[ControlTableAddress.MaxHeight] = this->sensor;
            break;
        case this->GET_TEM:
            this->tem = atoi((const char *)this->mUart.rxBuf);
            this->data[ControlTableAddress.MinHeight] = this->tem;
            break;                
    }
    this->mUart.byte_index = -1;
}

void AddonBoardController::ProcessZmqRepCallback() {
#ifdef ZMQ_REP_DEBUG
    printf("Received data from requester: ");
    for (int i = 0; i < this->mRep.rxDataLength; i++) {
        printf("%x", (uint8_t)this->mRep.rxBuf[i]);
    }
    printf("\n");
#endif
    if (this->mRep.rx_data_length > 10) {
        if (this->isChecksumOk(this->mRep.rx_buf, this->mRep.rx_data_length)) {
            uint8_t txBuf[12];
            int len = this->getAction(this->mRep.rx_buf, this->mRep.rx_data_length, txBuf);
            if (len > 0) {
#ifdef ZMQ_REP_DEBUG
                printf("Sent data to Requester: ");
                for (int i = 0; i < len; i++) {
                    printf("%x", txBuf[i]);
                }
                printf("\n");
#endif
                this->mRep.sendOne(txBuf, len);
            } else {
                this->mRep.sendNull();
            } // len <= 0
        } else {
            this->mRep.sendNull();
        } // checksum fail
    } else {
        this->mRep.sendNull();
    } // this->mRep.rxDataLength <= 10    

}

int uartLfCallkback(void *local_class,void* target_class){
    
    AddonBoardController *x = (AddonBoardController *)target_class;
    x->ProcessUartCallback();
    //mDesk.ProcessUartCallback();   
}








