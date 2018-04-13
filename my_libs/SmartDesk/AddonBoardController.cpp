#include "SmartDesk/AddonBoardController.hpp"

AddonBoardController::AddonBoardController() 
{

}
AddonBoardController::~AddonBoardController()
{

}

int AddonBoardController::Init(std::string endPoint, const char* serialName, int baud) 
{

}

int AddonBoardController::Init(std::string endPoint, UART_InitTypedef* uartInitStruct) 
{
    assert(this->mUart.Init(uartInitStruct) == 0);
    assert(this->mRep.Init(endPoint,this) == 0);    
}

int AddonBoardController::Init(const char* fileName) 
{
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_POLL;
    uartInit.byte_count = 12;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos;
    
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

int AddonBoardController::Init(const char* fileName, std::string dev_name) 
{
    UART_InitTypedef uartInit;
    uartInit.baudRate = 9600;    
    uartInit.terminator_type = Uart::TERMINATOR_POLL;
    uartInit.byte_count = 12;
    uartInit.target_class = this;
    FILE *fd = fopen(fileName,"r");
    std::string epoint;
    std::string dName;
    std::string minHeight, maxHeight,sitPos,standPos;
    
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
        dName = dev_name;
        fscanf(fd,"ZmqIPC:%s\n",buff);
        epoint = buff;      
        fscanf(fd,"Controller:%s\n",buff);
        
        uartInit.deviceName = dName;
        fscanf(fd,"Unit:%s\n",buff);
        this->Init(epoint,&uartInit);
    }
}

void AddonBoardController::Start() 
{
    this->mRep.bind();
    this->mUart.start();
    this->mRep.start();
    this->Enable();
}

void AddonBoardController::Stop() 
{
    this->mUart.stop();
    this->mRep.stop();
    this->Disable();
}

int AddonBoardController::Run() {

}

int AddonBoardController::Update() {

}

void AddonBoardController::control() {
}

int AddonBoardController::getDataFromUartBuf() {
    uint16_t res = 0;
    uint16_t length = mUart.rx_data_length;
    uint16_t index = length - 1;
    uint16_t tem = AutonomousFrame.FrameLength - 1;
    while(index >= tem)
    {
       if(AutonomousCommFrame_IsValidPacket(&mUart.rxBuf[index - tem], length)) 
       {
            AutonomousCommFrame_Array2Struct(&mUart.rxBuf[index - tem], length,&rx_obj);
            mUart.rx_data_length = 0;
            mUart.byte_index = 0;
            res = 1;
            index = 0;
            break;
       }
       else 
       {
           index--;
       }
       
    
    }
    return res;
}

void AddonBoardController::moveDown() {
}

void AddonBoardController::stopMove() {
}

void AddonBoardController::singleMotorReset() {
}

void AddonBoardController::setHeight(uint16_t height) {
}

void AddonBoardController::saveToPos4(uint16_t pos) {
}

void AddonBoardController::saveToPos3(uint16_t pos) {
}

void AddonBoardController::saveToPos2(uint16_t pos) {
}

void AddonBoardController::saveToPos1(uint16_t pos) {
}

void AddonBoardController::moveUp() {
}

void AddonBoardController::moveTopos4() {
}

void AddonBoardController::moveTopos3() {
}

void AddonBoardController::moveTopos2() {
}

void AddonBoardController::moveTopos1() {
}


char AddonBoardController::isChecksumOk(void* data, int len) {
    uint8_t *rxBuf = (uint8_t *) data;
    int rxBegin = 0;
    int rxEnd = len - 1;

    short sum = rxBuf[rxBegin + ZMQ_LENGTH_IND] +
            rxBuf[rxBegin + ZMQ_PRODUCT_ID_IND] +
            rxBuf[rxBegin + ZMQ_COMPONENT_ID_IND] +
            rxBuf[rxBegin + ZMQ_INSTRUCTION_IND] +
            rxBuf[rxBegin + ZMQ_ADDRESS_IND];
    if (rxBuf[rxBegin + ZMQ_LENGTH_IND] > 0) {
        int tem = rxBegin + ZMQ_ADDRESS_IND + 1;
        for (int i = tem; i < tem + rxBuf[rxBegin + ZMQ_LENGTH_IND]; i++) {
            sum += rxBuf[i];
        }
    }
    if (sum > 255) {
        sum = sum % 255;
    }
    if (sum == rxBuf[rxEnd - 2])
        return true;
    else return false;

}

uint16_t AddonBoardController::getHeight() {
}

uint16_t AddonBoardController::getChecksum(void* data, int len) {
    uint8_t *x = (uint8_t *) data;
    uint16_t sum = 0;
    for (int i = ZMQ_LENGTH_IND; i < len - 3; i++) {
        sum += x[i];
    }
    if (sum > 255) sum = sum % 255;
    return sum;
}

int AddonBoardController::getAction(void* data, int len, uint8_t* dataOut) {
    uint8_t *rxBuf = (uint8_t*) data;
    memcpy(rxBuf, data, len);
    int txLength = 0;
    uint16_t temData;
    int tem = State::STATE_FREE;
    int instruction = rxBuf[ ZMQ_INSTRUCTION_IND];
    int address = rxBuf[ ZMQ_ADDRESS_IND];
    int component = rxBuf[ZMQ_COMPONENT_ID_IND];
    int product = rxBuf[ZMQ_PRODUCT_ID_IND];
    if (product == ProductId.SmartDesk) 
    {
        if (component == ComponentId.UpDownMotor) 
        {

            int rxEnd = len;
            if (rxBuf[ZMQ_LENGTH_IND] == 2) 
            { // neu nhan duoc du lieu co 2 byte
                temData = ((uint16_t) rxBuf[rxEnd - 5])*256 + (uint16_t) rxBuf[rxEnd - 4];
            } 
            else {
                temData = rxBuf[rxEnd - 3];
            }
            if (instruction == INS_WRITE) 
            {
                this->data[address] = temData;
                this->mUart.putBytes(this->mRep.rx_buf, this->mRep.rx_data_length);
                if (address == ControlTableAddress.Up) 
                {

                    printf("[- I -] Move Up\n");
                } 
                else if (address == ControlTableAddress.Down) 
                {
                    printf("[- I -] Move Down\n");
                } 
                else if (address == ControlTableAddress.Stop) 
                {
                    printf("[- I -] Stop\n");
                }
                else if (address == ControlTableAddress.SaveToPos1)
                {
                    printf("[- I -] Save Sit Pos\n");
                }
                else if (address == ControlTableAddress.SaveToPos2)
                {
                    printf("[- I -] Save Stand Pos\n");
                }
                else if (address == ControlTableAddress.MoveToPos1)
                {
                    printf("[- I -] Sit\n");
                }
                else if(address == ControlTableAddress.MoveToPos2)
                {
                    printf("[- I -] Stand\n");
                }

            }
            else if (instruction == INS_READ) 
            {
                txLength = ZMQ_ADDRESS_IND + 4 + 2;
                uint8_t txBuf[txLength];
                txBuf[ZMQ_START_BYTE1_IND] = ZMQ_START_BYTE1;
                txBuf[ZMQ_START_BYTE2_IND] = ZMQ_START_BYTE2;
                txBuf[ZMQ_LENGTH_IND] = 2;
                txBuf[ZMQ_ADDRESS_IND] = address;
                temData = this->data[address];
                txBuf[ZMQ_ADDRESS_IND + 1] = (uint8_t) (temData >> 8);
                txBuf[ZMQ_ADDRESS_IND + 2] = (uint8_t) temData;

#ifdef CONSOLE
                printf("TX Length = %d\n", txLength);
#endif
                txBuf[txLength - 3] = this->getChecksum(txBuf, txLength);
                txBuf[txLength - 1] = ZMQ_STOP_BYTE1;
                txBuf[txLength - 2] = ZMQ_STOP_BYTE2;
                tem = State::STATE_GET_DATA;
                memcpy(dataOut, txBuf, txLength);
            }
            this->setState(tem);
        } 
        else if (component == ComponentId.RpiBoard) 
        {
            printf("[- I -]RPI command not support in AI box\n");
            txLength = 0;
        }

    }


    return txLength;
}

void AddonBoardController::ProcessUartCallback() 
{
   
    if (getDataFromUartBuf())
    {
        uint8_t ins = rx_obj.Instruction;
        uint8_t addr = rx_obj.Address;
        uint16_t data = AutonomousCommFrame_GetParamsFromStruct(&rx_obj);
        if (ins == Instructions.Update)
        {
           
            if (addr == ControlTableAddress.Height)
            { 
                this->setCurrentHeight(data);
            }
            else if (addr == ControlTableAddress.Mic)
            {
                this->data[ControlTableAddress.Mic] = 1;
            
            }
            else if (addr == ControlTableAddress.BoxStatus)
            {
                if (data > 0){
                    printf("[- I -]AddonBoard is busy now\n");
                }
                else {
                    printf("[- I -]AddonBoard is free now\n");
                }
                this->data[addr] = data;
            }
            
        }
        else if (ins == Instructions.Read)
        {
        
        }
        else if (ins == Instructions.Return)
        {
        
        }
        else if (ins == Instructions.Write)
        {
            printf("RPI command not supported in Console Version\n");
            
        
        }        
    }
    this->mUart.byte_index = 0;
}

void AddonBoardController::ProcessZmqRepCallback() 
{
    if (this->mRep.rx_data_length > 10) 
    {
        if (this->isChecksumOk(mRep.rx_buf, mRep.rx_data_length)) 
        {
            uint8_t txBuf[12];
            int len = this->getAction(mRep.rx_buf, mRep.rx_data_length, txBuf);
            if (len > 0) 
            {
                this->mRep.sendOne(txBuf, len);
            } 
            else 
            {
                this->mRep.sendNull();
            } // len <= 0
        } 
        else 
        {
            this->mRep.sendNull();
        } // checksum fail
    } 
    else 
    {
        this->mRep.sendNull();
    } // this->mRep.rxDataLength <= 10    

}


int ZmpReplierCallback(void *local_class, void * target_class)
{
    AddonBoardController *obj = (AddonBoardController *)(target_class);
    obj->ProcessZmqRepCallback();
    return 0;
}

int uartLfCallkback(void *local_class, void *target_class)
{
//    printf("Uart Default Callback\n");
    AddonBoardController *obj = (AddonBoardController *)(target_class);
    obj->ProcessUartCallback();
    return 0;

}