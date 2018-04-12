/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cmath>

#include "Device/DynamixelMotor.hpp"
#include "Device/DynamixelController.hpp"

DynamixelMotor::DynamixelMotor() {
}

DynamixelMotor::DynamixelMotor(uint8_t id) {
    this->id = id;
}

void DynamixelMotor::dlxSend() {
    //this->uart_busy = false;
    (this->v_uart).putBytes(this->tx_buffer, this->tx_length);
    this->port_busy = true;
    //this->uart_busy = true;
}

uint8_t DynamixelMotor::getId() {
    return this->id;
}

double DynamixelMotor::getRadPos() {

}

void DynamixelMotor::ping(uint8_t _id) {    
    this->tx_buffer[DynamixelMotor::IND_ID] = _id;
}

void DynamixelMotor::ping() {
    this->tx_buffer[DynamixelMotor::IND_ID] =
            DynamixelMotor::BROADCAST_ID;
}

void DynamixelMotor::resetRxBuffer() {
    memset(this->rx_buffer, 0, sizeof (this->rx_buffer));
}

void DynamixelMotor::resetTxBuffer() {
    memset(this->tx_buffer, 0, sizeof (this->tx_buffer));
    this->tx_buffer[DynamixelMotor::IND_START_BYTE1] = 255;
    this->tx_buffer[DynamixelMotor::IND_START_BYTE2] = 255;
    this->tx_length = 0;
    

}

void DynamixelMotor::setDegPos(double pos_deg) {
    pos_deg = posCheck(pos_deg);
    uint16_t value = (uint16_t)(pos_deg*pos_resolution);
    writeRegWordAction(this->GOAL_POS_L,value);
}

double DynamixelMotor::getDegPos() {
   readRegWordAction(this->PRESENT_POS_L); 
   this->v_timer->Start();
   while(this->port_busy){
       if(this->v_timer->timer_triggered == true){
            printf("Receive timeout\n");
            break;
       }
               
   }
   this->v_timer->Stop();
   this->v_timer->Reset();
   uint16_t x = (uint16_t)this->rx_buffer[6];
   x = x << 8;
   x += (uint16_t)this->rx_buffer[5];
   
   return ((double)(x)/this->pos_resolution - this->home_pos);
}

bool DynamixelMotor::isMoving() {
    readRegByteAction(this->MOVING);
    while(this->port_busy){
        if(this->v_timer->timer_triggered == true){
            printf("Receive timeout\n");
            break;
        }
    }
    this->v_timer->Stop();
    this->v_timer->Reset();
    return (bool)this->rx_buffer[5];
}

uint8_t DynamixelMotor::getTemperature() {
    readRegByteAction(this->PRESENT_TEM);
    this->v_timer->Start();
    while(this->port_busy){
        if(this->v_timer->timer_triggered == true){
            printf("Receive timeout\n");
            return 0;
            break;
            
        }
    }
    this->v_timer->Stop();
    this->v_timer->Reset();
    return this->rx_buffer[5];
}

double DynamixelMotor::getSpeed() {
    readRegWordAction(this->PRESENT_SPEED_L);
    while(this->port_busy);
    uint16_t x = (uint16_t)this->rx_buffer[6];
   x = x << 8;
   x += (uint16_t)this->rx_buffer[5];
   
   return ((double)(x)/this->speed_resolution);
}


void DynamixelMotor::setRadPos(double pos_rad) {
    double pos_deg = posCheck(pos_rad *180/M_PI);
    uint16_t value = (uint16_t)(pos_deg*pos_resolution);
    writeRegWordAction(this->GOAL_POS_L,value);
}

uint8_t DynamixelMotor::getReturnLevel() {
    readRegByteAction(this->STATUS_RETURN_LEVEL);
    while(this->port_busy);
    return this->rx_buffer[5];
}

void DynamixelMotor::setReturnLevel(uint8_t level) {
    if(level <=2){
        writeRegByteAction(this->STATUS_RETURN_LEVEL,level);
    }
}

void DynamixelMotor::setTorque(double torque_percent) {
    torque_percent = torqueCheck(torque_percent);
    uint16_t val = (uint16_t)(torque_percent*torque_resolution);
    writeRegWordAction(this->TORQUE_LIMIT_L,val);
    
}

void DynamixelMotor::setPunch(double punch_percent) {
    punch_percent = punchCheck(punch_percent);
    uint16_t val = (uint16_t)(punch_percent*punch_resolution);
    writeRegWordAction(this->PUNCH_L,val);
}

void DynamixelMotor::enableTorque(bool cmd) {
    if(cmd == true)
        writeRegByteAction(this->TORQUE_ENABLE,1);
    else 
        writeRegByteAction(this->TORQUE_ENABLE,0);
    
}



void DynamixelMotor::setSpeed(double speed_rpm) {
    speed_rpm = speedCheck(speed_rpm);
    uint16_t val = (uint16_t)(speed_rpm*speed_resolution);
    writeRegWordAction(this->SPEED_L,val);
    
}

void DynamixelMotor::updateTxChecksum() {
    uint16_t checksum = 0;
    for (int i = 0; i < this->tx_buffer[DynamixelMotor::IND_LENGTH] + 1;
            i++) {
        /*
         * Get checksum from index:ID
         */
        checksum += (uint16_t)this->tx_buffer[i + 2];
    }
    uint8_t k = (uint8_t) (checksum);
    k = (uint8_t) ~k;
    this->tx_buffer[tx_buffer[DynamixelMotor::IND_LENGTH] + 3] = k;
    this->tx_length = tx_buffer[DynamixelMotor::IND_LENGTH] + 4;
}

DynamixelMotor::DynamixelMotor(Joint* joint){
     this->id = joint->id;
    this->sign = joint->sign;
    this->home_pos = joint->home_pos_deg;
    this->pos_range = joint->pos_range_deg;
    
    this->min_speed_limit = joint->min_speed_rpm;
    this->max_speed_limit = joint->max_speed_rpm;
    
    this->max_pos_limit = this->home_pos + this->pos_range/2;
    this->min_pos_limit = this->home_pos - this->pos_range/2;
    
    this->max_torque_limit =joint->max_torque;
    
    this->motor_type = joint->type;
    
    
    this->max_punch_reg = 1023;
    
    this->min_punch_limit = joint->min_punch;
    this->max_punch_limit = joint->max_punch;
    
    
    if(!this->motor_type.compare("AX12")){
        /*Speed defined by product*/
        this->max_speed_reg = 1023;
        this->min_speed = 1;
        this->max_speed = 114;
        /*Position defined by product*/
        this->max_pos_reg = 1023;  
        this->min_pos = 0;
        this->max_pos = 300;
        /*Torque defined by product*/
        this->max_torque_reg = 1023;
        this->min_torque = 0;
        this->max_torque = 100;
        /*Punch*/
        this->min_punch = 0;
        this->max_punch = 100;
        punch_resolution = (double)(max_punch_reg)/(max_punch - min_punch);
        
        
        
        
        
    }
    else if(!this->motor_type.compare("MX28")){
        /*Speed param defined by product*/
        this->max_speed_reg = 1023;
        this->min_speed = 1;
        this->max_speed = 114;
        /*Position params defined by product*/
        this->max_pos_reg = 4095;
        this->min_pos = 0;
        this->max_pos = 300;
        /*Torque defined by product*/
        this->max_torque_reg = 1023;
        this->min_torque = 0;
        this->max_torque = 100;
        /*Punch*/
        this->min_punch = 0;
        this->max_punch = 100;
        punch_resolution = (double)(max_punch_reg)/(max_punch - min_punch);
        
        
        
        
    }
    pos_resolution = (double)(max_pos_reg)/(max_pos - min_pos);
    speed_resolution = (double)(max_speed_reg)/(max_speed - min_speed);
    torque_resolution = (double)(max_torque_reg)/(max_torque - min_torque);
    resetRxBuffer();
    resetTxBuffer();

}

void DynamixelMotor::init(Joint* joint) {
    this->id = joint->id;
    this->sign = joint->sign;
    this->home_pos = joint->home_pos_deg;
    this->pos_range = joint->pos_range_deg;
    
    this->min_speed_limit = joint->min_speed_rpm;
    this->max_speed_limit = joint->max_speed_rpm;
    
    this->max_pos_limit = this->home_pos + this->pos_range/2;
    this->min_pos_limit = this->home_pos - this->pos_range/2;
    
    this->max_torque_limit =joint->max_torque;
    
    this->motor_type = joint->type;
    
    
    this->max_punch_reg = 1023;
    
    this->min_punch_limit = joint->min_punch;
    this->max_punch_limit = joint->max_punch;
    
    
    if(!this->motor_type.compare("AX12")){
        /*Speed defined by product*/
        this->max_speed_reg = 1023;
        this->min_speed = 1;
        this->max_speed = 114;
        /*Position defined by product*/
        this->max_pos_reg = 1023;  
        this->min_pos = 0;
        this->max_pos = 300;
        /*Torque defined by product*/
        this->max_torque_reg = 1023;
        this->min_torque = 0;
        this->max_torque = 100;
        /*Punch*/
        this->min_punch = 0;
        this->max_punch = 100;
        punch_resolution = (double)(max_punch_reg)/(max_punch - min_punch);
        
        
        
        
        
    }
    else if(!this->motor_type.compare("MX28")){
        /*Speed param defined by product*/
        this->max_speed_reg = 1023;
        this->min_speed = 1;
        this->max_speed = 114;
        /*Position params defined by product*/
        this->max_pos_reg = 4095;
        this->min_pos = 0;
        this->max_pos = 300;
        /*Torque defined by product*/
        this->max_torque_reg = 1023;
        this->min_torque = 0;
        this->max_torque = 100;
        /*Punch*/
        this->min_punch = 0;
        this->max_punch = 100;
        punch_resolution = (double)(max_punch_reg)/(max_punch - min_punch);
        
        
        
        
    }
    pos_resolution = (double)(max_pos_reg)/(max_pos - min_pos);
    speed_resolution = (double)(max_speed_reg)/(max_speed - min_speed);
    torque_resolution = (double)(max_torque_reg)/(max_torque - min_torque);
    resetRxBuffer();
    resetTxBuffer();
    
}

uint8_t DynamixelMotor::getHighByte(uint16_t word) {
    return (uint8_t) (word >> 8);
}

uint8_t DynamixelMotor::getLowByte(uint16_t word) {
    return (uint8_t) word;
}

void DynamixelMotor::printTxBuf() {
#ifdef EN_DEBUG
    printf("TX Data\n");
    printf("Start Byte1: %x\n", tx_buffer[0]);
    printf("Start Byte2: %x\n", tx_buffer[1]);
    printf("ID: %x\n", tx_buffer[2]);
    printf("Length: %x\n", tx_buffer[3]);
    printf("Instruction: %x", tx_buffer[4]);
    switch (tx_buffer[4]) {
        case DynamixelMotor::INST_PING:
            printf(" : PING\n");
            break;
        case DynamixelMotor::INST_READ:
            printf(" : READ DATA\n");
            break;
        case DynamixelMotor::INST_WRITE:
            printf(" : WRITE DATA\n");
            break;
        case DynamixelMotor::INST_REG_WRITE:
            printf(" : REG WRITE\n");
            break;
        case DynamixelMotor::INST_ACTION:
            printf(" : TRIGGER\n");
            break;
        case DynamixelMotor::INST_RESET:
            printf(" : RESET\n");
            break;
        case DynamixelMotor::INST_SYNC_WRITE:
            printf(" : SYNC WWRITE\n");
            break;
    }
    printf("Address: %x\n", tx_buffer[5]);
    int k = 6;
    for (int i = k; i < k + tx_buffer[this->IND_LENGTH] - 3; i++) {
        printf("param %d:%x\n", i - k + 1, tx_buffer[i]);
    }
    printf("Checksum: %x\n", tx_buffer[tx_length - 1]);
    printf("TX Length:%d\n", tx_length);
#endif
}

void DynamixelMotor::readRegByteAction(uint8_t address) {
    /*
     * Prepare data package for address that have 1 byte parameter
     * Storage data into control table and wait for trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_READ;
    tx_buffer[this->IND_START_ADDRESS] = address; // start address tobe read
    tx_buffer[this->IND_PARAM] = 1; // length of data to be read
    tx_buffer[this->IND_LENGTH] = 4;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();
}

void DynamixelMotor::readRegWordAction(uint8_t address) {
    /*
     * Prepare data package for address that have 2 byte2 parameter
     * Storage data into control table and wait for trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_READ;
    tx_buffer[this->IND_START_ADDRESS] = address; // start address tobe read
    tx_buffer[this->IND_PARAM] = 2; // length of data to be read
    tx_buffer[this->IND_LENGTH] = 4;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();
    
    
    
}


void DynamixelMotor::trigger() {
    /*
     * 
     */
    tx_buffer[DynamixelMotor::IND_START_BYTE1] = DynamixelMotor::START_BYTE1;
    tx_buffer[DynamixelMotor::IND_START_BYTE2] = DynamixelMotor::START_BYTE2;
    tx_buffer[DynamixelMotor::IND_ID] = DynamixelMotor::BROADCAST_ID;
    tx_buffer[DynamixelMotor::IND_INSTRUCTION] = DynamixelMotor::INST_ACTION;
    tx_buffer[DynamixelMotor::IND_LENGTH] = 2;
    updateTxChecksum();
    int n = tx_buffer[DynamixelMotor::IND_LENGTH] + 4;
    printf("Trigger data: ");
    for (int i = 0; i < n; i++) {
        printf("%x ", tx_buffer[i]);
    }
    printf("\n");

}

void DynamixelMotor::writeRegByteAction(uint8_t address, uint8_t value) {
    /*
     * Prepare data package for address that have 1 byte parameter
     * Included trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_WRITE;
    tx_buffer[this->IND_START_ADDRESS] = address;
    tx_buffer[this->IND_PARAM] = value;
    tx_buffer[this->IND_LENGTH] = 4;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();

}

void DynamixelMotor::writeRegByteNoAction(uint8_t address, uint8_t value) {
    /*
     * Prepare data package for address that have 1 byte parameter
     * Storage data into control table and wait for trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_REG_WRITE;
    tx_buffer[this->IND_START_ADDRESS] = address;
    tx_buffer[this->IND_PARAM] = value;
    tx_buffer[this->IND_LENGTH] = 4;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();


}

void DynamixelMotor::writeRegWordAction(uint8_t address, uint16_t value) {
    /*
     * Prepare data package for address that have 2 bytes parameter
     * Included trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_WRITE;
    tx_buffer[this->IND_START_ADDRESS] = address;
    tx_buffer[this->IND_PARAM] = this->getLowByte(value);
    tx_buffer[this->IND_PARAM + 1] = this->getHighByte(value);
    tx_buffer[this->IND_LENGTH] = 5;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();
}

void DynamixelMotor::writeRegWordNoAction(uint8_t address, uint16_t value) {
    /*
     * Prepare data package for address that have 2 bytes parameter
     * Storage data into control table and wait for trigger signal
     * */
    tx_buffer[this->IND_ID] = this->id;
    tx_buffer[this->IND_INSTRUCTION] = this->INST_REG_WRITE;
    tx_buffer[this->IND_START_ADDRESS] = address;
    tx_buffer[this->IND_PARAM] = this->getLowByte(value);
    tx_buffer[this->IND_PARAM + 1] = this->getHighByte(value);
    tx_buffer[this->IND_LENGTH] = 5;
    this->updateTxChecksum();
    this->dlxSend();
    this->printTxBuf();

}

double DynamixelMotor::posCheck(double pos) {
    pos = home_pos + sign*pos;
    if (pos > max_pos_limit) pos = max_pos_limit;
    if (pos < min_pos_limit) pos = min_pos_limit;
    return pos;
}

double DynamixelMotor::speedCheck(double speed) {
    if (speed > max_speed_limit) speed = max_speed_limit;
    if (speed < min_speed_limit) speed = min_speed_limit;
    return speed;
}

double DynamixelMotor::torqueCheck(double torque) {
    if (torque > max_torque_limit) torque = max_torque_limit;
    if (torque < min_torque_limit) torque = min_torque_limit;
    return torque;
}

double DynamixelMotor::punchCheck(double punch) {
    if(punch > max_punch_limit) punch = max_punch_limit;
    if(punch < min_punch_limit) punch = min_punch_limit;
    return punch;
}


void DynamixelMotor::reset() {
    this->setSpeed(20);
    this->setTorque(100);
    this->enableTorque(true);
    this->ledOn();
    this->setDefautSmoothlyMove();
    this->setPunch(0);
}

void DynamixelMotor::ledOff() {
    this->writeRegByteAction(this->LED, 0);
}

void DynamixelMotor::ledOn() {
    this->writeRegByteAction(this->LED, 1);
}

void DynamixelMotor::setDefautSmoothlyMove() {
    this->writeRegByteAction(this->CCW_SLOPE,0x40);
    this->writeRegByteAction(this->CCW_MARGIN,0x01);
    this->writeRegByteAction(this->CW_SLOPE,0x40);
    this->writeRegByteAction(this->CW_MARGIN,0x01);


}
