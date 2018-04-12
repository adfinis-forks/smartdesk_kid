/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SmartDesk/TimotionController.hpp"
#include "Device/TimboxCommFrame.h"
TiMotionController *TiMotionController::tim_instance = NULL;

TiMotionController* TiMotionController::getInstance() {
    if (tim_instance == NULL)
    {
        tim_instance = new TiMotionController();
        return tim_instance;
    }
    else
    {
        printf("Cannnot Open, Timotion Controller is running\n");
        exit(EXIT_FAILURE);
    }
    
}

TiMotionController::TiMotionController() {
    this->initData();
    this->delayTimeCount = 0;
    this->posError = 0;
    this->check = 0;
    this->longBufInd = 0;
    this->uart_flag = 0;
    this->oldHeight = 0;
    this->checkHeightTimer = new Timer();
    this->checkConnectTimer = new Timer();
    this->RxDataFlag = 0;
    this->saveCount = 0;
    this->subState = 0;
    this->movePos1Count = 0;
    this->movePos2Count = 0;
    this->movePos3Count = 0;
    this->movePos4Count = 0;
    this->controlState = CONTROLLER_FREE;
    this->mutex1  = PTHREAD_MUTEX_INITIALIZER;
}

void TiMotionController::initData() {
    //Init Data
    upCmd[0] = 0xD8;
    upCmd[1] = 0xD8;
    upCmd[2] = 0x66;
    upCmd[3] = 0x02;
    upCmd[4] = 0x02;
    downCmd[0] = 0xD8;
    downCmd[1] = 0xD8;
    downCmd[2] = 0x66;
    downCmd[3] = 0x01;
    downCmd[4] = 0x01;
    
    stopCmd[0] = 0xD8;
    stopCmd[1] = 0xD8;
    stopCmd[2] = 0x66;
    stopCmd[3] = 0x00;
    stopCmd[4] = 0x00;
    
    pos1Cmd[0] = 0xD8;
    pos1Cmd[1] = 0xD8;
    pos1Cmd[2] = 0x66;
    pos1Cmd[3] = 0x04;
    pos1Cmd[4] = 0x04;
    
    pos2Cmd[0] = 0xD8;
    pos2Cmd[1] = 0xD8;
    pos2Cmd[2] = 0x66;
    pos2Cmd[3] = 0x08;
    pos2Cmd[4] = 0x08;
    
    pos3Cmd[0] = 0xD8;
    pos3Cmd[1] = 0xD8;
    pos3Cmd[2] = 0x66;
    pos3Cmd[3] = 0x10;
    pos3Cmd[4] = 0x10;

    pos4Cmd[0] = 0xD8;
    pos4Cmd[1] = 0xD8;
    pos4Cmd[2] = 0x66;
    pos4Cmd[3] = 0x20;
    pos4Cmd[4] = 0x20;
    
    saveCmd[0] = 0xD8;
    saveCmd[1] = 0xD8;
    saveCmd[2] = 0x66;
    saveCmd[3] = 0x40;
    saveCmd[4] = 0x40;  
    
    resetCmd[0] = (uint8_t)0xD8; 
    resetCmd[1] = (uint8_t)0xD8;
    resetCmd[2] = (uint8_t)0x66;
    resetCmd[3] = (uint8_t)0x03;
    resetCmd[4] = (uint8_t)0x03;
}

TiMotionController::~TiMotionController() {
    printf("Delete Timotion desk controller\n");
}

uint16_t TiMotionController::getHeight() {

}

void TiMotionController::moveDown() {

}

void TiMotionController::moveTopos1() {
    /*
     * State 0: Send 2 stop command before send Sit command.
     * State 1: Send Sit Command continuously.
     * State 2: 
     */
    switch (this->subState){
        case 0:  
            if(this->saveCount < 2){
                // Only print status one time
                if (this->getPreState() != this->getState()){
                    printf("[I]-Moving to Sit Pos\n");
                    this->setState(State::STATE_MOVE2POS1);    
                }                   
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
                this->saveCount++;
            }
            else {
                
                this->saveCount = 0;
                this->subState = 1;
            }
            break;
        case 1: 
            /*
             Command sit/Stand Co the gui lien tuc, neu chen giua 2 lenh nay 1
             * lenh stop thi command sau do se stop
             
             */
            if (this->movePos1Count< 10){
                this->movePos1Count++;
            }
            if(this->movePos1Count == 4){
                this->setPos1Height(this->getCurrentHeight());
                printf("[I]Sit Height = %d\n",this->getCurrentHeight());
                
            }
            
            this->mUart.putBytes(&pos1Cmd,sizeof(pos1Cmd));
            break;
    }
}

void TiMotionController::moveTopos2() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0:
            if(this->saveCount < 2){
                // 
                this->saveCount++;
                // Only print status one time
                if (this->getPreState() != this->getState()){
                    printf("Moving to Stand Pos\n");
                    this->setState(State::STATE_MOVE2POS2);    
                }   
                // Send stop command 2 times before send Sit command
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                
                this->saveCount = 0;
                this->subState = 1;
            }
            break;
        case 1:
            if (this->movePos2Count< 100){
                this->movePos2Count++;
            }
            if(this->movePos2Count == 4){
                this->setPos2Height(this->getCurrentHeight());
                printf("[I]-Stand Height = %d\n",this->getCurrentHeight());
            }
            this->mUart.putBytes(&pos2Cmd,sizeof(pos2Cmd));
            break;
    }
}

void TiMotionController::moveTopos3() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0:
            if(this->saveCount < 2){
                // 
                this->saveCount++;
                // Only print status one time
                if (this->getPreState() != this->getState()){
                    printf("[I]-Moving to Pos3\n");
                    this->setState(State::STATE_MOVE2POS3);    
                }   
                // Send stop command 2 times before send Sit command
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                
                this->saveCount = 0;
                this->subState = 1;
            }
            break;
        case 1:
            if (this->movePos3Count< 10){
                this->movePos3Count++;
                
            }
            if(this->movePos3Count == 4){
                this->setPos3Height(this->getCurrentHeight());
                printf("[I]-Pos3 Height = %d\n",this->getCurrentHeight());
            }
            this->mUart.putBytes(&pos3Cmd,sizeof(pos3Cmd));
            break;
    }
}

void TiMotionController::moveTopos4() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0:
            if(this->saveCount < 2){
                // 
                this->saveCount++;
                // Only print status one time
                if (this->getPreState() != this->getState()){
                    printf("[I]-Moving to Pos4\n");
                    this->setState(State::STATE_MOVE2POS4);    
                }   
                // Send stop command 2 times before send Sit command
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                
                this->saveCount = 0;
                this->subState = 1;
            }
            break;
        case 1:
            if (this->movePos4Count< 10){
                this->movePos4Count++;
            }
            if(this->movePos4Count == 4){
                this->setPos4Height(this->getCurrentHeight());
                printf("[I]-Pos4 Height = %d\n",this->getCurrentHeight());
            }
            this->mUart.putBytes(&pos4Cmd,sizeof(pos4Cmd));
            break;
    }
}

void TiMotionController::moveUp() {
    //this->data[ControlTableAddress.BoxStatus]
    if(this->getPreState() != State::STATE_UP){
        this->setState(State::STATE_UP);
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        printf("[I]-Up\n");
    }
    else {
        this->mUart.putBytes(&upCmd,sizeof(upCmd));
    }
    
    
    
    if(this->getCurrentHeight() >= this->GetMaxHeight()){
        this->setState(State::STATE_STOP);
        printf("[-I-]- Reach max height\n");
        
    }
        
}

void TiMotionController::dualMotorReset() {
        /*
         *  State 0: send 3 stop command to clean the controller
         *  State 1: send Reset Command until  get the height is 2550
         *  State 2: continuously send the Reset Command until get the valid height
         * 
         */
        switch (this->subState){        
        case 0: // Gui 3 Lenh Stop
            if (this->saveCount < 3){
                this->saveCount++;
                this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
            }
            else{
                this->saveCount = 0;
                this->subState = 1;
                printf("[I]-Entering Reset Mode\n");
            }
            break;
        case 1:// Gui Lenh Reset Cho Den khi kich hoat mode            
            if(this->getCurrentHeight() != 2550){
                this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));

            }
            else{
                printf("[I]-Reset Mode activated\n");
                this->saveCount = 0;
                this->subState = 2;                                                                                               
            }
            break;
        case 2: // Gui Lenh Di Chuyen Xuong
            if(this->getCurrentHeight() == 2550){
                this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));
            }
            else{
                this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
                printf("[I]-Reset Finished\n");
                this->subState = 0;
                this->saveCount = 0;
                this->setState(State::STATE_STOP);
            }
            break;                           
    }
}

void TiMotionController::singleMotorReset(){
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){        
        case 0: // Gui 3 Lenh Stop
            if (this->saveCount < 3){
                this->saveCount++;
                this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
            }
            else{
                this->saveCount = 0;
                this->subState++;
                printf("[I]-Entering Reset Mode\n");
            }
            break;
        case 1:// Gui Lenh Reset Cho Den khi kich hoat mode            
            if(this->getCurrentHeight() != 2550){
                this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));

            }
            else{
                this->saveCount++;
                if(this->saveCount == 1){
                    printf("[I]-Reset Mode activated\n");
                    
                }
                if(this->saveCount < 4){
                    this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
                    //this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));
                }
                else{
                    this->saveCount = 0;
                    this->subState++;
                }                                                                                                
            }
            break;
        case 2: // Gui 3 Lenh Stop 
            if (this->saveCount < 3){
                this->saveCount++;
                this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
                //this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));
            }
            else{
                this->saveCount = 0;
                this->subState++;
                printf("[I]-Moving the desk down...\n");
            }
            break;
        case 3: // Gui Lenh Di Chuyen Xuong
            if(this->getCurrentHeight() == 2550){
                this->mUart.putBytes(&this->downCmd,sizeof(this->downCmd));
                //this->mUart.putBytes(&this->resetCmd,sizeof(this->resetCmd));
            }
            else{
                this->mUart.putBytes(&this->stopCmd,sizeof(this->stopCmd));
                printf("[I]-Reset Finished\n");
                this->subState = 0;
                this->saveCount = 0;
                this->setState(State::STATE_STOP);
            }
            break;                           
    } 
}

void TiMotionController::saveToPos1(uint16_t pos) {
    this->delayTimeCount++;
    if(this->delayTimeCount < MAX_DELAY_COUNT){
        this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
        printf("[I]-Save To Pos1: %d\n",MAX_DELAY_COUNT - this->delayTimeCount);
        if(this->delayTimeCount == MAX_DELAY_COUNT-1){
            sleep(1);
            
        }
    }
    else{
        
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&pos1Cmd,sizeof(pos1Cmd));
        usleep(100000);
        this->mUart.putBytes(&pos1Cmd,sizeof(pos1Cmd));
        usleep(100000);
        this->setState(State::STATE_STOP);
        this->setPos1Height(this->getCurrentHeight());
        printf("[I]-Save finished\n");
    }
}

void TiMotionController::saveToPos2(uint16_t pos) {
    this->delayTimeCount++;
    if(this->delayTimeCount < MAX_DELAY_COUNT){
        this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
        printf("[I]-Save To Pos1: %d\n",MAX_DELAY_COUNT - this->delayTimeCount);
        if(this->delayTimeCount == MAX_DELAY_COUNT-1){
            sleep(1);
            
        }
    }
    else{
        
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&pos2Cmd,sizeof(pos2Cmd));
        usleep(100000);
        this->mUart.putBytes(&pos2Cmd,sizeof(pos2Cmd));
        usleep(100000);
        this->setState(State::STATE_STOP);
        this->setPos2Height(this->getCurrentHeight());
        printf("[I]-Save finished\n");
    }
}

void TiMotionController::saveToPos3(uint16_t pos) {
    this->delayTimeCount++;
    if(this->delayTimeCount < MAX_DELAY_COUNT){
        this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
        printf("[I]-Save To Pos1: %d\n",MAX_DELAY_COUNT - this->delayTimeCount);
        if(this->delayTimeCount == MAX_DELAY_COUNT-1){
            sleep(1);
            
        }
    }
    else{
        
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&pos3Cmd,sizeof(pos3Cmd));
        usleep(100000);
        this->mUart.putBytes(&pos3Cmd,sizeof(pos3Cmd));
        usleep(100000);
        this->setState(State::STATE_STOP);
        this->setPos3Height(this->getCurrentHeight());
        printf("[I]-Save finished\n");
    }
}

void TiMotionController::saveToPos4(uint16_t pos) {
    this->delayTimeCount++;
    if(this->delayTimeCount < MAX_DELAY_COUNT){
        this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
        printf("[I]-Save To Pos1: %d\n",MAX_DELAY_COUNT - this->delayTimeCount);
        if(this->delayTimeCount == MAX_DELAY_COUNT-1){
            sleep(1);
            
        }
    }
    else{
        
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
        usleep(100000);
        this->mUart.putBytes(&pos4Cmd,sizeof(pos4Cmd));
        usleep(100000);
        this->mUart.putBytes(&pos4Cmd,sizeof(pos4Cmd));
        usleep(100000);
        this->setState(State::STATE_STOP);
        this->setPos4Height(this->getCurrentHeight());
        printf("[I]-Save finished\n");
    }
}

void TiMotionController::setHeight(uint16_t height) {
    

}

void TiMotionController::stopMove() {
    this->controlState = CONTROLLER_BUSY;
    uint16_t temState = this->getPreState();
    /*
     * If preState is Move To pos1
     */
    if (temState == State::STATE_MOVE2POS1){
        printf("Pre State: Move To Pos1\n");
    }
    else if (temState == State::STATE_MOVE2POS2){
        printf("Pre State: Move To Pos2\n");
    }
    
    
    
    
    if (this->getPreState() != this->getState()){
        // Gui Lenh Stop Dau tien
        this->mUart.putBytes(&stopCmd,sizeof(upCmd));
        // Log out
        printf("[I]-Stop\n");
        // Update State to skip log
        this->setState(State::STATE_STOP);
        
    }
    else{
        if (this->saveCount < 3){
            this->mUart.putBytes(&stopCmd,sizeof(upCmd));
            this->saveCount++;
            
        }
        else{
            this->saveCount = 0;
            this->setState(State::STATE_FREE);
        }
        
        
    }
    
}



char TiMotionController::isChecksumOk(void* data, int len) {
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
    
}

int TiMotionController::getAction(void* data, int len,uint8_t *dataOut) {
    // Tao bien tem de luu lai thong tin State tu data gui toi
    // khoi tao tem = Freestate
    uint8_t *rxBuf = (uint8_t*)data;
    memcpy(rxBuf,data,len);
    uint16_t txLength = 0;
    uint16_t temData;
    uint16_t rxEnd = len;
//    uint16_t temState = this->getState();
    uint16_t instruction = (uint16_t)rxBuf[ ZMQ_INSTRUCTION_IND];
    uint16_t address = (uint16_t)rxBuf[ ZMQ_ADDRESS_IND];
    uint16_t product = (uint16_t)rxBuf[ZMQ_PRODUCT_ID_IND];
    uint16_t component = (uint16_t)rxBuf[ZMQ_COMPONENT_ID_IND];
    if(rxBuf[ZMQ_LENGTH_IND] == 2)
    { // neu nhan duoc du lieu co 2 byte

        temData = ((uint16_t)rxBuf[rxEnd - 5])*256  + (uint16_t)rxBuf[rxEnd - 4];
    } 
    else
    {
        temData = rxBuf[rxEnd - 3];
    }
    
    if(product == ProductId.SmartDesk)
    {
        if (component == ComponentId.UpDownMotor)
        {

            if(instruction == Instructions.Write)
            {
                this->saveCount = 0;
                this->subState = 0;
        #ifdef ZMQ_REP_DEBUG
                std::cout<<"-I-Received Instruction Write Data\n";
                printf("address:%d\n",address);
        #endif

                this->data[address] = temData;
                if (address == ControlTableAddress.Up)
                {
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_UP);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.Down)
                {
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_DOWN);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.Stop)
                {
                    this->setMode(Mode::MODE_MANUAL); 
                    this->setState(State::STATE_STOP);
                    this->checkHeightTimer->Reset();
                    this->firtLog = 1;
                }
                else if (address == ControlTableAddress.SetPointHeight)
                {
                    this->checkHeightTimer->Reset();
                    temData = temData/10;
                    temData = temData*10;
                    int tem = abs(temData- this->getCurrentHeight());
                    if ( tem <= 10){
                        this->offset = 0;
            
                    }
                    else if(tem <= 20){
                        this->offset = 10;
                    }
                    else{
                        this->offset = this->error;
                    }
                    if(this->setSetpointHeight(temData)){
                        this->data[address] = temData;
                        this->setMode(Mode::MODE_AUTO);
                        printf("[I]-set heigh = %d\n ",temData);
                        this->checkHeightTimer->Reset();
                    }
                    else{
                        printf("[E]- height is out of range\n");
                    }    
                }
                else if (address == ControlTableAddress.SaveToPos1){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_SAVE2POS1);
                    this->delayTimeCount = 0;
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.SaveToPos2){
                    this->setMode(Mode::MODE_MANUAL);
                    this->delayTimeCount = 0;
                    this->setState(State::STATE_SAVE2POS2);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.SaveToPos3){
                    this->setMode(Mode::MODE_MANUAL);
                    this->delayTimeCount = 0;
                    this->setState(State::STATE_SAVE2POS3);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.SaveToPos4){
                    this->setMode(Mode::MODE_MANUAL);
                    this->delayTimeCount = 0;
                    this->setState(State::STATE_SAVE2POS4);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.MoveToPos1){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_MOVE2POS1);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.MoveToPos2){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_MOVE2POS2);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.MoveToPos3){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_MOVE2POS3);
                    this->checkHeightTimer->Reset();
                }
                else if (address == ControlTableAddress.MoveToPos4){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_MOVE2POS4);
                    this->checkHeightTimer->Reset();    
                }      
                else if (address == ControlTableAddress.SingleMotorReset){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_SINGLE_MOTOR_RESET);
                    this->checkHeightTimer->Reset();

                }
                else if (address == ControlTableAddress.DualMotorReset){
                    this->setMode(Mode::MODE_MANUAL);
                    this->setState(State::STATE_DUAL_MOTOR_RESET);
                    this->checkHeightTimer->Reset();
                }

            } 
            else if(instruction == Instructions.Read)
            {                           
                txLength = ZMQ_ADDRESS_IND + 4 + 2;
                uint8_t txBuf[txLength];
                txBuf[ZMQ_START_BYTE1_IND] = ZMQ_START_BYTE1;
                txBuf[ZMQ_START_BYTE2_IND] = ZMQ_START_BYTE2;
                txBuf[ZMQ_LENGTH_IND] = 2;
                txBuf[ZMQ_ADDRESS_IND] = address;
                pthread_mutex_lock(&this->mutex1);
                temData = this->data[address];
                pthread_mutex_unlock(&this->mutex1);
                txBuf[ZMQ_ADDRESS_IND + 1] = (uint8_t)(temData >> 8);
                txBuf[ZMQ_ADDRESS_IND + 2] = (uint8_t)temData;
                txBuf[txLength - 3] = this->getChecksum(txBuf,txLength);
                txBuf[txLength - 1] = ZMQ_STOP_BYTE1;
                txBuf[txLength - 2] = ZMQ_STOP_BYTE2;
                memcpy(dataOut,txBuf,txLength);
            }
            
        }
        else if (component == ComponentId.RpiBoard)
        {
            if (instruction == Instructions.Write)
            {
               if (address == RpiControlTable.Backlight)
               {
                   if (temData == LCD_ON)
                   {
                       this->mLcd->backlightOn();
                   }
                   else if (temData == LCD_OFF)
                   {
                       this->mLcd->backlightOff();
                   }
               }
               else if (address == RpiControlTable.Brightness)
               {
                   this->mLcd->setBrightnessLevel((uint8_t)temData);
                   printf("Set brightness = %d\n",temData);
               
               }
               else if (address = RpiControlTable.Touch2On)
               {
                    
               }
               else if (address == RpiControlTable.Touch2Off)
               {
                   
               }
               else if (address == RpiControlTable.Touch2Toggle)
               {
                   
               }
            }
            else if (instruction == Instructions.Read)
            {
                if (address == RpiControlTable.Backlight)
                {
                    
                }
                else if (address == RpiControlTable.Brightness)
                {
                
                }
            
            }
            
        }
    
    }
    return txLength;
}

uint16_t TiMotionController::getChecksum(void* data, int len) {
    uint8_t *x = (uint8_t *)data;
    uint16_t sum = 0;
    for (int i = ZMQ_LENGTH_IND; i < len -3;i++){
        sum += x[i];
    }
    if(sum > 255) sum = sum%255;
    return sum;
    
    
}

void TiMotionController::Update() 
{
    // Update Data Controller   
    
    if(this->getMode() == Mode::MODE_AUTO){
        //updateLogData(this->getCurrentHeight(),this->getSetpointHeight());
        this->posError = this->getSetpointHeight()-this->getCurrentHeight();  
        //printf("e:%d\n",this->posError);
        if(this->posError < -this->offset){
//        if(this->posError < 0){
            this->check = 0;
            if((this->getState() != State::STATE_DOWN)&&((this->getMode() == Mode::MODE_AUTO))){       
                this->setState(State::STATE_DOWN);        
            }
        }
        else if(this->posError > this->offset){
//        else if(this->posError > 0){
            this->check = 0;
            if((this->getState() != State::STATE_UP)&&(this->getMode() == Mode::MODE_AUTO)){
                this->setState(State::STATE_UP);
            }
        }
        else if(this->check == 0){
            this->check = 1;
            this->mUart.putBytes(&stopCmd,sizeof(stopCmd)); 
            this->hold_height = this->getCurrentHeight();
            this->setState(State::STATE_STOP);
            this->setMode(Mode::MODE_MANUAL);  
            printf("e:%d\n",this->posError);
            printf("Stoped in Update\n");
        }
    }
}


int TiMotionController::Init(std::string endPoint, std::string serialName, int baud){
    assert(this->mUart.Init(serialName,baud) == 0);
    assert(this->mRep.Init(endPoint,this) == 0);
    this->controlThreadFcn = TimotionControlCallback;
    this->tim.tv_sec = 0;
    this->tim.tv_nsec = 150000000L; //150ms
    int res = this->setupThread(TimotionControllerThread, this);
    switch (res){
        case 0:
            printf("[-I-]Created Timotion Controller\n");
            break;
        case EAGAIN:
            printf("[-E-]Insufficient resources to create another thread\n");
            break;
        case EINVAL:
            printf("[-E-]Invalid settings in attr\n");
            break;
        case EPERM:
            printf("[-E-]No permission to set the scheduling policy and parameters specified in attr\n");
            break;
    }
    this->checkHeightTimer->Init(500000000L, 8,TimotionCheckHeightCallback,this);//5s
    this->checkConnectTimer->Init(500000000L,10,TimotionCheckConnectCallback,this); //5s
    
}

int TiMotionController::Init(std::string endPoint, UART_InitTypedef * uartInitStruct){
    assert(this->mUart.Init(uartInitStruct) == 0);
    assert(this->mRep.Init(endPoint,this) == 0);    
    this->SetMinHeight(TIMOTION_MIN_HEIGHT);
    this->SetMaxHeight(TIMOTION_MAX_HEIGHT);
    this->SetControlThreadFcn(TimotionControlCallback);
    this->tim.tv_sec = 0;
    this->tim.tv_nsec = 150*1000*1000;
    int res = this->setupThread(TimotionControllerThread, this);
    switch (res){
        case 0:
            printf("[-I-]Created Timotion Controller\n");
            break;
        case EAGAIN:
            printf("[-E-]Insufficient resources to create another thread\n");
            break;
        case EINVAL:
            printf("[-E-]Invalid settings in attr\n");
            break;
        case EPERM:
            printf("[-E-]No permission to set the scheduling policy and parameters specified in attr\n");
            break;
    }
    this->checkHeightTimer->Init(500000000L, 10,TimotionCheckHeightCallback,this);//5s
    this->checkConnectTimer->Init(500000000L,10,TimotionCheckConnectCallback,this); //5s
    
}

int TiMotionController::Init(const char* fileName)
{ 
    /*
     * MinHeight:560 
    MaxHeight:1300 
    SitPos:700
    StandPos:1000
    SerialDevice:/dev/ttyUSB0
    ZmqIPC:ipc:///tmp/desk_control:9999
    Controller:Timotion
    Unit:met
     */
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_POLL ;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos;
    assert(fd >0);
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
        this->SetMinHeight(atoi(minHeight.data()));
        this->SetMaxHeight(atoi(maxHeight.data()));
        this->data[ControlTableAddress.Pos1Height] = atoi(sitPos.data());
        this->data[ControlTableAddress.Pos2Height] = atoi(standPos.data());
    }
 
}

int TiMotionController::Init(const char* fileName, std::string s) 
{
    /*
     * MinHeight:560 
    MaxHeight:1300 
    SitPos:700
    StandPos:1000
    SerialDevice:/dev/ttyUSB0
    ZmqIPC:ipc:///tmp/desk_control:9999
    Controller:Timotion
    Unit:met
     */
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_COUNT ;
    uartInit.byte_count = TimboxFrameHeader.rxFrameLength;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos,up_e,down_e;
    assert(fd >0);
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
        dName = s;
        fscanf(fd,"ZmqIPC:%s\n",buff);
        epoint = buff;      
        fscanf(fd,"Controller:%s\n",buff);      
        uartInit.deviceName = dName;
        fscanf(fd,"Unit:%s\n",buff);
        fscanf(fd,"Error:%s\n",buff);
        up_e = buff;
        
        
        
        this->Init(epoint,&uartInit);
        this->SetMinHeight(atoi(minHeight.data()));
        this->SetMaxHeight(atoi(maxHeight.data()));
        this->data[ControlTableAddress.Pos1Height] = atoi(sitPos.data());
        this->data[ControlTableAddress.Pos2Height] = atoi(standPos.data());
        this->error = atoi(up_e.data());
        printf("Error:%d\n",this->error);
        
        this->down_error = atoi(down_e.data());
    }
}



int TiMotionController::AddToLongBuf(uint8_t* data, uint16_t length) 
{
    if (this->longBufInd < sizeof (this->longBuf) - length - 2) 
    {
        memcpy(&this->longBuf[this->longBufInd], data, length);
        this->longBufInd += length;
    } 
    else 
    {
//        printf("[E]-Device was unplugged\n");
        this->longBufInd = 0;
    }

}

void TiMotionController::ProcessUartCallback() 
{
    this->RxDataFlag = 1;
    uint16_t res = GetHeightFromBuffer()*10;
    if ((res > 0 )&&(res != this->getCurrentHeight()))
    {
        //pthread_mutex_lock(&this->mutex1);
        this->setCurrentHeight(res);   
        //printf("Current height:%d\n",res);
        //pthread_mutex_unlock(&this->mutex1);
        /*
        if(this->getState() == State::STATE_STOP)
        {
            uint16_t te = this->getPreState();
            if(( te == State::STATE_DOWN)||(te == State::STATE_UP)){
                int x = abs(this->hold_height -res);
                if ((x != 0)&&(x != this->error)){                
                    this->error = x;
                    char s[40];
                    sprintf(s,"./change.sh Error %d",this->error);
                    system(s);
                }
            }
            
        }
         */
        
        
    } 
    
    
}
void TiMotionController::updateLogData(uint16_t height,uint16_t setpoint){
    if(firtLog){
        firtLog = 0;
        clock_gettime(CLOCK_MONOTONIC, &tstart);
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double tem = (double)tend.tv_sec + 1.0e-9*tend.tv_nsec;
    double tem_start = (double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec;
    
    
    
    for (int i = 3; i > 0; i--){
        logData[0][i] = logData[0][i-1];
        logData[1][i] = logData[1][i-1];
        logData[2][i] = logData[1][i-1];
    }
    logData[0][0] = (double)height; // [x] [ ] []
    logData[1][0] = tem-tem_start;  // [ ] [x] []  
    // Tinh Van Toc
    for (int i = 0; i < 3; i++){
        if((logData[1][i] != 0) &&(logData[1][i +1] != 0)){
            
            double dt = logData[1][i] - logData[1][i+1];
            double dy = logData[0][i] - logData[0][i+1];
            double v = dy/dt;
            logData[2][i] = v;
        }
    }
    if(log_index < 4){
        log_index++;
    }
    else{
        double dd = (double)setpoint;
       double y1 = logData[0][0];
       double y2 = logData[0][1];
       double t1 = logData[1][0];
       double t2 = logData[1][1];
       double dy1 = logData[2][0];
       double dy2 = logData[2][1]; 
//#define WAY1
#ifdef WAY1
       double del_t = -(y1 -y2 -t1*dy1 + t2*dy2)/(dy1 - dy2);
       double b = -(dy2*(y1 -t1*dy1) - dy1*(y2 - t2*dy2))/(dy1 - dy2);
       double p_y = dy1*(t1 -del_t) +b;
       double t_new = (dd - b +dy1*del_t)/dy1;
       
#else
       double del_t = (dy1*t1-y1)/dy1;
       double t_new  = (dd + dy1*del_t)/dy1;
       double p_y = dy1*(t1 -del_t);
#endif   
       //double d_t = (p_y -b)/dy1;
       //printf("h:%d,time:%3.3f,dy:%3.3f,dt:%f,b:%f,py:%f\n",height,logData[1][0],logData[2][0],del_t,b,p_y);
       if(t1 >= t_new){
           this->stopMove();
           this->setState(State::STATE_STOP);
           this->firtLog = 1;
       }
       printf("y_old,y_new,t_old,t_new:%f,%f,%f,%f\n",(double)height,dd,t1,t_new);
       
       //printf("t1:%f, t2:%f\n,dy1:%f,dy2:%f,py%f\n",logData[1][0],logData[1][1],logData[2][0],logData[2][1],p_y);
       //printf("Timeneed:%f\n",d_t);
       //printf("DeltaT:%f\n",del_t);
    }
    
}
int TiMotionController::GetHeightFromBuffer() 
{
    /*
     Return res()the real value from Control Box, then x10(convert to mm) and save to data table
     */
    uint16_t res = 0;
    uint16_t length = this->mUart.rx_data_length; // =6
    uint16_t index = length - 1;
    uint16_t tem = TimboxFrameHeader.rxFrameLength -1; // =5
    while(index >= tem )
    {
        if(_TIM_isValidRxData(&mUart.rxBuf[index -tem],length))
        {
            _TIM_Rxarray2struct(&mUart.rxBuf[index -tem], length, &rxFrameObj);
            res = _TIM_getDataFromRxStruct(&rxFrameObj);
            SetBoxStatus((TimboxFrameStatus) _TIM_GetParamFromRxStruct(&rxFrameObj));
            index = 0;
            mUart.rx_data_length = 0;
            mUart.byte_index = 0;
            break;
            
        }
        else
        {
            index--;
        }
        
    }

    return res;
}

void TiMotionController::PrintConnectStatus() 
{
    if (this->connectStatus == 0) 
    {
        this->connectStatus = 1;
        this->disconnectStatus = 0;
        printf("[I]-Control Box Connected\n");
    }
}

void TiMotionController::PrintDisconnectStatus() 
{
    if (this->disconnectStatus == 0) 
    {
        this->disconnectStatus = 1;
        this->connectStatus = 0;
        printf("[E]-Control Box Not Connected\n");
    }
}



void TiMotionController::ProcessZmqRepCallback() 
{
    
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
            //Get information from ZMQ Requester
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

int TiMotionController::Run() 
{
    int currentState = this->getState();
    switch (currentState) {
        case State::STATE_UP:
            this->updateControllerStatus(CONTROLLER_BUSY);
            if(this->getPreState() != State::STATE_UP){
                this->setState(State::STATE_UP);
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
                printf("[I]-Up\n");
            }
            else {
                this->mUart.putBytes(&upCmd,sizeof(upCmd));
            }



            if(this->getCurrentHeight() >= this->GetMaxHeight()){
                this->setState(State::STATE_STOP);
                printf("[-I-]- Reach max height\n");

            }
            break;
        case State::STATE_DOWN:
            this->updateControllerStatus(CONTROLLER_BUSY);
                //this->data[ControlTableAddress.BoxStatus]
            if(this->getPreState() != State::STATE_DOWN){
                this->setState(State::STATE_DOWN);
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
                printf("[I]-Down\n");
            }
            else{
                this->mUart.putBytes(&downCmd,sizeof(upCmd));
            }
            if(this->getCurrentHeight() <= this->GetMinHeight()){
                this->setState(State::STATE_STOP);
            }
            break;
        case State::STATE_STOP:
            if(this->getSetpointHeight() != getCurrentHeight()){
                this->setSetpointHeight(this->getCurrentHeight());
            }
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->hold_height = this->getCurrentHeight();
            if((this->preState == State::STATE_MOVE2POS1)||(this->preState == State::STATE_MOVE2POS2)){
                switch(this->subState){
                    case 0:
                        this->mUart.putBytes(this->stopCmd,sizeof(upCmd));
                        
                        printf("[-I-] Stop\n");
                        this->subState++;
                    case 1:
                        this->mUart.putBytes(this->upCmd,sizeof(upCmd));
                        printf("[-I-] Stop\n");
                        this->subState++;
                        break;
                    case 2:                    
                        this->mUart.putBytes(this->stopCmd,sizeof(upCmd));
                        this->subState++;
                        break;
                    case 3:
                        this->mUart.putBytes(this->stopCmd,sizeof(upCmd));
                        this->subState = 0;
                        this->setState(State::STATE_FREE);
                        break;
                }
            
            }
            else {
                if(this->saveCount++ < 3){
                    this->mUart.putBytes(this->stopCmd,sizeof(upCmd));
                    if(this->saveCount == 1){
                        printf("[-I-] Stop\n");
                    }
                }
                else{
                    this->saveCount = 0;
                    this->setState(State::STATE_FREE);
                }
            
            }
            break;
        case State::STATE_FREE:
            this->saveCount = 0;
            this->subState = 0;
            this->movePos1Count = 0;
            this->movePos2Count = 0;
            this->movePos3Count = 0;
            this->movePos4Count = 0;
            this->mUart.putBytes(&stopCmd,sizeof(upCmd));
            this->updateControllerStatus(CONTROLLER_FREE);
            
            break;
        case State::STATE_SINGLE_MOTOR_RESET:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->singleMotorReset();                                   
            break;
        case State::STATE_DUAL_MOTOR_RESET:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->dualMotorReset();
            break;
        case State::STATE_SAVE2POS1:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->saveToPos1();
            break;
        case State::STATE_SAVE2POS2:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->saveToPos2();
            break;
        case State::STATE_SAVE2POS3:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->saveToPos3();
            break;
        case State::STATE_SAVE2POS4:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->saveToPos4();
            break;
        case State::STATE_MOVE2POS1:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->moveTopos1();   
            
            break;
        case State::STATE_MOVE2POS2:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->moveTopos2();
            break;
        case State::STATE_MOVE2POS3:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->moveTopos3();
            break;
        case State::STATE_MOVE2POS4:
            this->updateControllerStatus(CONTROLLER_BUSY);
            this->moveTopos4();
            break;
        case State::STATE_GET_DATA:
            printf("GetData State\n");
            break;


    }
}

void TiMotionController::SetBoxStatus(TimboxFrameStatus _status) 
{
//    if (_status != this->frameStatus_cur) {
//        this->frameStatus_pre = this->frameStatus_cur;
//        this->frameStatus_cur = _status;
//    }
    this->frameStatus_cur = _status;
}

void TiMotionController::Start() 
{
    this->mRep.bind();
    this->mUart.start();
    this->mRep.start();
    this->checkHeightTimer->Start();
    this->checkConnectTimer->Start();
    this->Enable();

}

void TiMotionController::Stop() 
{
    this->mUart.stop();
    this->mRep.stop();
    this->checkHeightTimer->Stop();
    this->checkConnectTimer->Stop();
    this->Disable();
}

TimboxFrameStatus TiMotionController::getBoxStatus() {
    return this->frameStatus_cur;
}

int TiMotionController::getDataFromUart(uint8_t* data, int begin) 
{
    return (int) data[begin + TimboxFrameHeader.rxFrameLength - 1];
}

int TiMotionController::getPacketBegin(uint8_t* data, int len) {
    int x = -1;
    if (len > 0) {
        for (int i = 0; i < len - 1; i++) {
            if ((data[i] == TimboxFrameHeader.RxStartByte1)
                    &&(data[i + 1] == TimboxFrameHeader.RxStartByte2)) 
            {
                x = i;
                printf("data:");
                for (int k = 0; k < x + TimboxFrameHeader.rxFrameLength; k++) 
                {
                    printf("%x ", data[k]);
                }
                printf("\n");
                i = len;
            }
        }
    }
    return x;
}

int TiMotionController::isValidPacket(uint8_t* data, int begin, int end) {
    if ((data[begin] == TimboxFrameHeader.RxStartByte1)
            &&(data[begin + 1] == TimboxFrameHeader.RxStartByte2)
            &&(data[end - 1] == data[end - 2]))
        return 1;
    else return 0;
}

void TiMotionController::saveToPos1() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0: // Gui 3 lenh Stop
            
            if (this->saveCount < 3){
                if(this->saveCount == 1){
                    printf("[I]-Entering saving mode\n");
                }
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 1;
            }
        case 1: // Gui lenh Save cho den khi kich hoat  
            
            if (this->getBoxStatus() != TimboxStatus.SAVE_TRIGGED){
                this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
            }
            else{
                this->saveCount++;
                if(this->saveCount >= 3){
                    this->saveCount = 0;
                    this->subState = 2;
                    printf("[I]-Save Mode Activated\n");
                }
            }

            break;
        case 2: // Gui 3 lenh Stop
            if (this->saveCount < 3) {
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 3;
            }
            break;
        case 3: // Gui Lenh Luu Vao Pos1
            if(this->saveCount < 6){
                this->saveCount ++;
                this->mUart.putBytes(&pos1Cmd,sizeof(pos1Cmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 4;
                this->setPos1Height(this->getCurrentHeight());
                printf("[I]-Save Sit Pos Finished\n");
            }
            break;
        case 4: // Gui 3 Lenh Stop
            if (this->saveCount < 3){
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 0;
                this->setState(State::STATE_STOP);
            }
            break;
    }
}

void TiMotionController::saveToPos2() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0:
            
            if (this->saveCount < 3){
                if(this->saveCount == 1){
                    printf("[I]-Entering saving mode\n");
                }
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 1;
            }
        case 1:
            
            if (this->getBoxStatus() != TimboxStatus.SAVE_TRIGGED){
                this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
            }
            else{
                this->saveCount++;
                if(this->saveCount >= 3){
                    this->saveCount = 0;
                    this->subState = 2;
                    printf("[I]-Save Mode Activated\n");
                }
            }

            break;
        case 2:
            if (this->saveCount < 4) {
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 3;
            }
            break;
        case 3:
            if(this->saveCount < 4){
                this->saveCount ++;
                this->mUart.putBytes(&pos2Cmd,sizeof(pos2Cmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 4;
                this->setPos1Height(this->getCurrentHeight());
                printf("[I]-Saved to Stand Pos\n");
            }
            break;
        case 4:
            if (this->saveCount < 4){
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 0;
                this->setState(State::STATE_STOP);
            }
            break;
    }
}

void TiMotionController::saveToPos3() {
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState){
        case 0:
            
            if (this->saveCount < 4){
                if(this->saveCount == 1){
                    printf("[I]-Enabling saving mode\n");
                }
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 1;
            }
        case 1:
            
            if (this->getBoxStatus() != TimboxStatus.SAVE_TRIGGED){
                this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
            }
            else{
                this->saveCount++;
                if(this->saveCount >= 3){
                    this->saveCount = 0;
                    this->subState = 2;
                    printf("[I]-Save Enabled\n");
                }
            }

            break;
        case 2:
            if (this->saveCount < 4) {
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 3;
            }
            break;
        case 3:
            if(this->saveCount < 4){
                this->saveCount ++;
                this->mUart.putBytes(&pos3Cmd,sizeof(pos3Cmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 4;
                this->setPos1Height(this->getCurrentHeight());
                printf("[I]-Saved to Pos3\n");
            }
            break;
        case 4:
            if (this->saveCount < 4){
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 0;
                this->setState(State::STATE_STOP);
            }
            break;
    }
}

void TiMotionController::saveToPos4() 
{
    //this->data[ControlTableAddress.BoxStatus]
    switch (this->subState)
    {
        case 0:
            
            if (this->saveCount < 4){
                if(this->saveCount == 1){
                    printf("[I]-Enabling saving mode\n");
                }
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 1;
            }
        case 1:
            
            if (this->getBoxStatus() != TimboxStatus.SAVE_TRIGGED){
                this->mUart.putBytes(&saveCmd,sizeof(saveCmd));
            }
            else{
                this->saveCount++;
                if(this->saveCount >= 3){
                    this->saveCount = 0;
                    this->subState = 2;
                    printf("[I]-Save Enabled\n");
                }
            }

            break;
        case 2:
            if (this->saveCount < 4) {
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 3;
            }
            break;
        case 3:
            if(this->saveCount < 4){
                this->saveCount ++;
                this->mUart.putBytes(&pos4Cmd,sizeof(pos4Cmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 4;
                this->setPos1Height(this->getCurrentHeight());
                printf("[I]-Saved to Pos4\n");
            }
            break;
        case 4:
            if (this->saveCount < 4){
                this->saveCount++;
                this->mUart.putBytes(&stopCmd,sizeof(stopCmd));
            }
            else {
                this->saveCount = 0;
                this->subState = 0;
                this->setState(State::STATE_STOP);
            }
            break;
    }
}

int TiMotionController::updateControllerStatus(WorkStatus_Typedef _status) 
{
    this->data[ControlTableAddress.BoxStatus] = _status;
    return this->data[ControlTableAddress.BoxStatus];
}




/*
 attribute weak function
 */
// For Timotion Controller

__attribute__ ((weak)) void* TimotionControllerThread(void* data) 
{
    TiMotionController *x = (TiMotionController*)data;
    while(1)
    {
        nanosleep(&x->tim,&x->tim2);
        if(x->isReady() > 0)
        {
            if(x->controlThreadFcn != NULL)
            {
                (*x->controlThreadFcn)(x, x->target_class);
            }
        }
    
    }
}

__attribute__ ((weak))int TimotionControlCallback(void* local_class, void *target_class)
{
    TiMotionController *obj = (TiMotionController *)(local_class);
    obj->Run();
    

}

__attribute__ ((weak)) int  TimotionCheckHeightCallback(void *local_class, void * target_class)
{
    TiMotionController *obj = (TiMotionController *)(target_class);
    if (obj->oldHeight == obj->getCurrentHeight())
    {
        if ((obj->getState() == State::STATE_UP)
            ||(obj->getState() == State::STATE_DOWN)
            ||(obj->getState() == State::STATE_MOVE2POS1)
            ||(obj->getState() == State::STATE_MOVE2POS2)
            ||(obj->getState() == State::STATE_MOVE2POS3)
            ||(obj->getState() == State::STATE_MOVE2POS4)    )
        {
            obj->setState(State::STATE_STOP);
            obj->setMode(Mode::MODE_MANUAL);
            printf("[E]-No moment detected in 5 seconds\n");
            
        }
    }
    else 
    {
        obj->oldHeight = obj->getCurrentHeight();
        //obj->nomoveCount = 0;
    }
    if(obj->getCurrentHeight() == 2550)
    {
        obj->setMode(Mode::MODE_MANUAL);
        //obj->setState(State::STATE_RESET);
        
    }
    return 0;

}

__attribute__ ((weak)) int  TimotionCheckConnectCallback(void *local_class, void * target_class){
    TiMotionController *obj = (TiMotionController *)(target_class);
    if(obj->RxDataFlag){
        obj->RxDataFlag = 0;
        obj->PrintConnectStatus();
    }
    else{
        obj->PrintDisconnectStatus();
    }

}

/*Overwrite Callback Function*/
int uartLfCallkback(void *local_class, void *target_class)
{
    TiMotionController *obj = (TiMotionController *)(target_class);
    obj->ProcessUartCallback();
    return 0;

}

int ZmpReplierCallback(void *local_class, void * target_class)
{
    TiMotionController *obj = (TiMotionController *)(target_class);
    obj->ProcessZmqRepCallback();
    return 0;
}
/*
 End of File
 */