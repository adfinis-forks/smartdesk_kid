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
#include "Device/DynamixelController.hpp"
DynamixelController *DynamixelController::instance = NULL;

DynamixelController::~DynamixelController() {

}

DynamixelController::DynamixelController() {
    this->nDevice = 0;
    
}




DynamixelController* DynamixelController::getInstance() {
    if(instance == NULL){
        printf("Please setup DynamixelController first");
    }
    assert(instance != NULL);
    return instance;
}

void DynamixelController::init(std::string dev_name, uint32_t baudrate) {
    
    this->mUart.Init(dev_name, baudrate);
    this->tim.tv_sec = 0;
    this->tim.tv_nsec = 100000000L; //100ms
    this->SetControlThreadFcn(dlxControllerCallback);
    this->setupThread(dlxControllerThread,this);
    
}

void DynamixelController::init(UART_InitTypedef* uart_init) {
    this->instance = this;
    this->mUart.Init(uart_init);
    this->tim.tv_sec = 0;
    this->tim.tv_nsec = 100000000L; //100ms    
    this->SetControlThreadFcn(dlxControllerCallback);
    this->setupThread(dlxControllerThread,this);
    this->mTimer.Init(10000000L,1,(void*)this); // 10ms
    
    
}

void DynamixelController::updateMotorList() {
    list<DynamixelMotor*> tem = this->motors_list;
    int i = 0;
    
    while (!tem.empty()) {
        this->motors[i++] = tem.front();
        tem.pop_front();
    }
}

void DynamixelController::start() {
    this->updateMotorList();
    

    for (int i = 0; i < this->motors_list.size(); i++) {
        (this->motors[i]->v_uart) = this->mUart;
        this->motors[i]->v_timer  = &this->mTimer;
    }
    
    this->mUart.start();
    
    for (int i = 0; i < this->motors_list.size(); i++) {
        this->motors[i]->reset();
    }
}

void DynamixelController::removeDevice(DynamixelMotor* motor) {
    if (this->motors_list.empty()) {
        printf("No device in list\n");
    }
}

void DynamixelController::getInfor() {
    int n = this->motors_list.size();
    for (int i = 0; i < n; i++) {
        printf("id = %d\n", (int) this->motors[i]->getId());
    }
}

void DynamixelController::enable() {

}

void DynamixelController::disable() {

}

void DynamixelController::addDevice(DynamixelMotor *motor) {
    list<DynamixelMotor*> tem = this->motors_list;
    bool check = true;
    while (!tem.empty()) {
        DynamixelMotor *x = tem.front();
        tem.pop_front();
        if (x->getId() == motor->getId()) {
            check = false;
        }
    }
    if (check == true) {
        this->motors_list.push_front(motor);


    }



}

void DynamixelController::setAllMotor(std::string cmd, double value) {
    if (!cmd.compare("setDegPos")){
        for (int i = 0; i < this->motors_list.size();i++){
            this->motors[i]->setDegPos(value);
        }
    }
    else if(!cmd.compare("setLedOn")){
        for (int i = 0; i < this->motors_list.size();i++){
            this->motors[i]->ledOn();
        }
    }
    else if(!cmd.compare("setLedOff")){
        for (int i = 0; i < this->motors_list.size();i++){
            this->motors[i]->ledOff();
        }
    }
}

int DynamixelController::setupThread(void*(*fcn)(void*), void* data) {
    return pthread_create(&this->controlThread, NULL, fcn, data);
}

void DynamixelController::SetControlThreadFcn(int(*fcn)(void*)) {
    this->controlThreadFcn = fcn;

}

void DynamixelController::ProcessUartCallback() {
    //printf("data:%x, length: %d \n",this->mUart.rxBuf[this->mUart.rxDataLength-1],this->mUart.rxDataLength);
    int  k = this->mUart.rx_data_length;
    
    if (k >= 6) {
        uint8_t length_fom_pck = this->mUart.rxBuf[3] + 4;     
        if(k >= length_fom_pck){
            this->updateInfor();
            this->mUart.byte_index = -1;
        }             
    }



}

bool DynamixelController::checkRxChecksum(uint8_t *data, uint8_t length) {
    uint8_t checksum_pck = data[length-1];
    uint16_t sum = 0;
    for (int i = 2; i < length-1;i++){
        sum += (uint16_t)data[i];
    }
    uint8_t k = (uint8_t) (sum);
    k = (uint8_t) ~k;
    if(k == checksum_pck) return true;
    else return false;
}

void DynamixelController::printRxPacket(uint8_t* data, uint8_t length) {
    printf("start byte1: %x \n",data[0]);
    printf("start byte2: %x \n",data[1]);
    printf("ID: %x \n",data[2]);
    printf("Length:%d \n",data[3]);
    printf("Error:");
    if(data[4] == 0) 
        printf("None\n");
    else {
        if(data[4] | DynamixelMotor::E_ANGLE_LIMIT)
            printf("angle limit error\n");
        if(data[4] | DynamixelMotor::E_CHECKSUM)
            printf("checksum error\n");
        if(data[4] | DynamixelMotor::E_INPUT_VOL)
            printf("input voltage error\n");
        if(data[4] | DynamixelMotor::E_INSTRUCTION)
            printf("instruction error\n");
        if(data[4] | DynamixelMotor::E_OVERHEATING)
            printf("over heating error\n");
        if(data[4] | DynamixelMotor::E_OVERLOAD)
            printf("over load error\n");
        if(data[4] | DynamixelMotor::E_RANGE)
            printf("range error\n");
    }
    for (int i = 0; i < data[3]-2;i++){
        printf("data %d: %x\n",i,data[5+i]);
    }
    printf("Checksum: %x\n",data[length-1]);
    
}


void DynamixelController::updateInfor() {
    uint8_t x[this->mUart.rx_data_length];
    memcpy(x,this->mUart.rxBuf,sizeof(x));
    if(this->checkRxChecksum(x,sizeof(x))){
        uint8_t id = x[2];
        for (int i = 0; i < this->motors_list.size();i++){
            if(id == this->motors[i]->getId()){
                this->motors[i]->port_busy = false;
                memcpy(this->motors[i]->rx_buffer,x,sizeof(x));
                this->motors[i]->rx_length = sizeof(x);
            }
        }
    }
    else printf("rx checksum fail\n");
}


void DynamixelController::AddToLongBuf(uint8_t* data, uint16_t length) {
    if (this->longBufInd < sizeof (this->longBuf) - length - 2) {
        memcpy(&this->longBuf[this->longBufInd], data, length);
        this->longBufInd += length;
    } else {
        this->longBufInd = 0;
    }

}

__attribute__ ((weak)) void *dlxControllerThread(void *data){
    
}
__attribute__ ((weak)) int dlxControllerCallback(void *data){
    

}
int TimerCallbackFcn(void * local_class_p, void *target_class_p){
    
    Timer * _timer = (Timer *)local_class_p;
    _timer->Stop();
    _timer->timer_triggered = true;    
    printf("Default Timer Fcn\n");
    return 0;
}
int uartLfCallkback(void *local_class,void *target_class){
//    printf("Uart Default Callback\n");
    DynamixelController *x = (DynamixelController *)target_class;
    x->ProcessUartCallback();
    return 0;

}
