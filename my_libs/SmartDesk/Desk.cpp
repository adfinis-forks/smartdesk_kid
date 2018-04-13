/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SmartDesk/Desk.hpp"
//Getter and setter area
uint16_t Desk::getCurrentHeight()  {
    return this->data[ControlTableAddress.Height];
}

uint16_t Desk::getMode()  {
    return this->data[ControlTableAddress.OperateMode];
}

uint16_t Desk::getPos1Height()  {
    return this->data[ControlTableAddress.Pos1Height];
}

uint16_t Desk::getPos2Height()  {
    return this->data[ControlTableAddress.Pos2Height];
}

uint16_t Desk::getPos3Height()  {
    return this->data[ControlTableAddress.Pos3Height];
}

uint16_t Desk::getPos4Height()  {
    return this->data[ControlTableAddress.Pos4Height];
}

uint16_t Desk::getSetpointHeight()  {
    return this->data[ControlTableAddress.SetPointHeight];
}

uint16_t Desk::getState()  {
    return this->data[ControlTableAddress.ControllerState];
}

void Desk::setCurrentHeight(uint16_t currentHeight) {
    this->data[ControlTableAddress.Height] = currentHeight;
}

void Desk::setMode(uint16_t mode) {
    this->data[ControlTableAddress.OperateMode] = mode;
}

void Desk::setPos1Height(uint16_t pos1Height) { 
    this->data[ControlTableAddress.Pos1Height] = pos1Height;  
}

void Desk::setPos2Height(uint16_t pos2Height) {
    this->data[ControlTableAddress.Pos2Height] = pos2Height;
    
}

void Desk::setPos3Height(uint16_t pos3Height) {
    this->data[ControlTableAddress.Pos3Height] = pos3Height;
}

void Desk::setPos4Height(uint16_t pos4Height) {
    this->data[ControlTableAddress.Pos4Height] = pos4Height;
}

uint16_t Desk::setSetpointHeight(uint16_t setpointHeight) {
    if ((setpointHeight >= this->GetMinHeight())&&(setpointHeight <= this->GetMaxHeight())){ 
        return 1;
    }
    else {
        return 0;
    }   
}

void Desk::setState(uint16_t state) {
    this->setPreState(this->getState());
    this->data[ControlTableAddress.ControllerState] = state;
}
//User define



void Desk::initDataTable() {
    this->data[ControlTableAddress.Version] = 0x02;
    this->data[ControlTableAddress.Height] = 650;
    this->data[ControlTableAddress.Up] = false;
    this->data[ControlTableAddress.Down] = false;
    this->data[ControlTableAddress.Stop] = true;
    this->data[ControlTableAddress.SaveToPos1] = false;
    this->data[ControlTableAddress.MoveToPos1] = false;
    this->data[ControlTableAddress.SaveToPos2] = false;
    this->data[ControlTableAddress.MoveToPos2] = false;
    this->data[ControlTableAddress.SaveToPos3] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.MoveToPos3] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.SaveToPos4] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.MoveToPos4] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.User9Height] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.User10Height] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.MinHeight] = 650;
    this->data[ControlTableAddress.MaxHeight] = 1300;
    this->data[ControlTableAddress.SetPointHeight] = DEFAULT_HEIGHT;
    this->data[ControlTableAddress.OperateMode] = Mode::MODE_MANUAL;
    this->data[ControlTableAddress.UserID] = (uint16_t)UID;
    this->data[ControlTableAddress.Pos1Height] = 650;
    this->data[ControlTableAddress.Pos2Height] = 850;
    this->data[ControlTableAddress.Pos3Height] = 1050;
    this->data[ControlTableAddress.Pos4Height] = 1250;
    
}

Desk::~Desk() {
//    std::cout<<"Delete Desk\n";
    free(this->data);
}

Desk::Desk() {
//    this->data = (int *)malloc(ControlTableAddress.LengthOfAddr*sizeof(int));
    this->initDataTable();
}
mCtrBoxFcn::mCtrBoxFcn() {
    this->CURENT_HEIGHT = 0x01;
    this->ERROR_CODE   = 0x02;
    this->RESET_MODE   = 0x04;
    this->MAX_MIN_HEIGHT  = 0x07;
    this->RX_START_BYTE1   =   0xF2;
    this->RX_START_BYTE2   =   0xF2;
    this->RX_STOP_BYTE   =   0x7E;
}

mDeskPacket::mDeskPacket() {
    this->IND_START_BYTE1 =  0;
    this->IND_START_BYTE2 =  1;
    this->IND_FUNCTION =     2;
    this->IND_DATA_LENGTH =  3;
}

State * State::m_instance = NULL;

State* State::getInstance() {
    if (m_instance == NULL)
        m_instance = new State();
    return m_instance;
}

State::State() {
}






