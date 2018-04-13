/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"Connectivity/Uart.hpp"

char Uart::putByte(void* data) {
    this->writeApi((void *)data,1);    
}

void Uart::init() {
}

void Uart::setPacketTimeout(uint16_t packet_length) {
    this->packet_start_time = this->getCurrentTime();
    this->packet_timeout = (tx_time_per_byte * (double) packet_length) + 
            ((double)LATENCY_TIMER * 2.0) + 2.0;

}

void Uart::setPacketTimeoutMs(uint16_t timeout_ms) {
     this->packet_start_time = this->getCurrentTime();
     this->packet_timeout = timeout_ms;
}

bool Uart::isPacketTimeout() {
    if (getTimeSinceStart() > packet_timeout) {
        packet_timeout = 0;
        return true;
    }
    return false;
}

double Uart::getTimeSinceStart() {
    double time;
    time = getCurrentTime() - this->packet_start_time;
    if (time < 0.0)
        this->packet_start_time = getCurrentTime();

    return time;

}

double Uart::getCurrentTime() {
    /*
     * Get the current time in ms
     */
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    return ((double) tv.tv_sec * 1000.0 + (double) tv.tv_nsec * 0.001 * 0.001);
}

uint16_t Uart::getBytesAvailable() {
    int bytes_available = -1;
    ioctl(this->fd, FIONREAD, &bytes_available);
    return bytes_available;
}

char Uart::putByte(char data){
    this->writeApi((void *)&data,1);
}

char Uart::putByte(unsigned char data){
    this->writeApi((void *)&data,1);
}

char Uart::putBytes(void* data, int len) {
    printf("Put bytes\n");
    this->writeApi(data,len);
}

char Uart::putString(char* s) {
    try{
        return write(this->fd, s,strlen(s));
    }
    catch (int e){
        return e;  
    }  
}

int Uart::getCFlagBaud(uint32_t baudrate) {
    switch (baudrate) {
        /*
         * #define  B50	0000001
#define  B75	0000002
#define  B110	0000003
#define  B134	0000004
#define  B150	0000005
#define  B200	0000006
#define  B300	0000007
#define  B600	0000010
#define  B1200	0000011
#define  B1800	0000012
#define  B2400	0000013
#define  B4800	0000014
         */
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}

char Uart::putString(std::string s){
    write(this->fd,s.data(),s.length());
    
}

char Uart::putString(const char* s){
    char *x = (char*)s;
    return   this->writeApi(x,strlen(x));
}

Uart::~Uart() {
    this->closeApi();
}
//----------------------------------------------------------//

int Uart::writeApi(void* data, uint8_t len) {
    
    try{
        this->clearApi();
        return write(this->fd, data,len);
    }
    catch (int e){
        return e;
    
    }
}

uint16_t Uart::readApi(uint8_t * data) {
    assert(this->fd >= 0);
    return read(this ->fd,data,1);
}

uint16_t Uart::readApiTimeout(uint8_t* data, uint16_t min_length) {
    uint16_t rx_length = 0;
    uint16_t result = 0;
    uint8_t c[50];
    uint8_t index = 0;
    uint16_t n;
    this->setPacketTimeout(min_length);
    while(1){
        n = this->getBytesAvailable();
        if(n > 0){
            
            this->readApi(c,n);
            memcpy(&data[index],c,n);
            rx_length += n;
            index = rx_length;
        }
        if(rx_length >= min_length){
            result = rx_length;
            break;
        }
        else{
            if(this->isPacketTimeout() == true){
                if(rx_length == 0){
                    result = 0;
                    break;
                }
                else {
                    result = 1;
                    break;
                }

            }
        }
    }
    
    return result;
}

uint16_t Uart::readApiTimeoutMs(uint8_t* data, uint16_t time_out_ms){
    /*
     * time_out_ms : time out each byte
     */
    uint16_t rx_length = 0;
    uint16_t result = 0;
    uint8_t c[50];
    uint8_t index = 0;
    uint16_t n;
    this->setPacketTimeout(time_out_ms);
    while(1){
        n = this->getBytesAvailable();
        if(n > 0){
            this->setPacketTimeout(time_out_ms);
            this->readApi(c,n);
            memcpy(&data[index],c,n);
            rx_length += n;
            index = rx_length;
        }
        else{
            if(this->isPacketTimeout() == true){
                if(rx_length == 0){
                    result = 0;
                    break;
                }
                else {
                    result = rx_length;
                    break;
                }

            }
        }
    }
    
    return result;
    
}

uint16_t Uart::readApi(uint8_t* data, uint16_t length) {
    int n = read(this->fd, data,length);
    if(n > 0)
    return n;
    else return 0;
}

int Uart::setMinimumBytesToRead(uint8_t n_bytes) {
    
    struct termios  config;
    // get the current port config
    if(this->fd < 0){
        printf("Please open device first\n");
    }
    assert(this->fd >= 0);
    int tem = tcgetattr (this->fd, &config);
    
    if (tem < 0){
        printf ("-E- cannot get current config \r\n");
        return 0;
    }
    assert(tem >= 0);
    
    config.c_cc[VMIN]  = n_bytes; // minimum character to read
    /*
     * 	Flush input and output buffers and make the change
     */
    if (tcsetattr (fd, TCSAFLUSH, &config) < 0){
        return 0;
    }
    /*
     * Check if  write success or not
     */
    if(config.c_cc[VMIN] == n_bytes) return 1;
    
    else return 0;
}

uint8_t Uart::getMinimumBytesToRead() {
    struct termios  config;
    // get the current port config
    if (tcgetattr (this->fd, &config) < 0){
        printf ("-E- cannot get current config \r\n");
        return 0;
    }
    return config.c_cc[VMIN];
}

int Uart::printCurrentConfig(){
    // Input option
    struct termios  config;
    // get the current port config
    if (tcgetattr (this->fd, &config) < 0){
        printf ("-E- cannot get current config \r\n");
        return -1;
    }
    unsigned int input_flag             = config.c_iflag;
    unsigned int output_flag            = config.c_oflag;
    unsigned int control_flag           = config.c_cflag;
    unsigned int local_flag             = config.c_lflag;
    unsigned char line_flag             = config.c_line;
    cc_t *character_flag;
    character_flag  = config.c_cc;
    speed_t input_speed = config.c_ispeed;
    speed_t output_speed = config.c_ospeed;
    
    printf("1. Current Input Control Option\n");
    
    if (input_flag & IGNBRK ) printf("  -Ignore break condition = true\n");
    if (input_flag & BRKINT ) 
        printf("  -Send a SIGINT when a break condition is detected\n");
    if (input_flag & IGNPAR ) printf("  -Ignore parity errors\n");
    if (input_flag &PARMRK ) printf("  -Mark parity errors\n");
    if (input_flag & INPCK ) printf("  -Enable parity check\n");
    if (input_flag & ISTRIP ) printf("  -Strip parity bits\n");
    if (input_flag & INLCR ) printf("  -Map NL to CR\n");
    if (input_flag & IGNCR ) printf("  -Ignore CR\n");
    if (input_flag & ICRNL ) printf("  -Map CR to NL\n");
    if (input_flag & IUCLC ) printf("  -Map uppercase to lowercase\n");
    if (input_flag & IXON ) 
        printf("  -Enable software flow control (outgoing)\n");
    if (input_flag & IXANY ) 
        printf("  -Allow any character to start flow again\n");
    if (input_flag & IXOFF ) 
        printf("  -Enable software flow control (incoming)\n");
    if (input_flag & IMAXBEL ) printf("  -Echo BEL on input line too long\n");
    if (input_flag & IMAXBEL ) printf("  -Echo BEL on input line too long\n");
    printf("2. Current Output Control Option\n");
    if (output_flag & OPOST == 0 )  printf("  -raw output");
    if (output_flag & OLCUC )       printf("  -Map lowercase to uppercase");
    if (output_flag & ONLCR )       printf("  -Map NL to CR-NL");
    if (output_flag & OCRNL )       printf("  -Map CR to NL");
    if (output_flag & ONLRET )       printf("  -NL performs CR function");
    if (output_flag & OFILL )       printf("  -Use fill characters for delay");
    if (output_flag & OFDEL )       printf("  -Fill character is DEL");
    #if defined __USE_MISC || defined __USE_XOPEN
        if (output_flag & NLDLY )       
            printf("  -Mask for delay time needed between lines");
        if (output_flag & NL0 )       
            printf("  -No delay for NLs");
        if (output_flag & NL1 )       
            printf("  -Delay further output after newline for "
                    "100 milliseconds");
        if (output_flag & CRDLY )       
            printf("  -Mask for delay time needed to return "
                    "carriage to left column");
        #endif
    printf("3. Control Option\n");
    
    if ((control_flag & CSIZE) == CS5 )         
        printf("   -5 bit data\n");
    if ((control_flag & CSIZE) == CS6 ) 
        printf("   -6 bit data\n");
    if ((control_flag & CSIZE) == CS7 ) 
        printf("   -7 bit data\n");
    if ((control_flag & CSIZE) == CS8 )
        printf("   -8 bit data\n");
    if (control_flag & CSTOPB ) 
        printf("   -2 stop bit\n");
    if (control_flag & CREAD ) 
        printf("   -Enable receiver\n");
    if (control_flag & PARENB ) 
        printf("   -Enable parity bit\n");
    if (control_flag & HUPCL ) 
        printf("   -Hangup (drop DTR) on last close\n");
    if (control_flag & CLOCAL ) 
        printf("   -Local line - do not change owner of port\n");
    if (control_flag & CRTSCTS ) 
        printf("   -Enable hardware flow control\n");
    printf("4. Local Option\n");

    if(local_flag & ISIG)
        printf("    -Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals\n");
    if(local_flag & ICANON)
        printf("    -Enable canonical input (else raw)\n");
    else
        printf("    -RAW input\n");
    if(local_flag & XCASE)
        printf("    -Map uppercase lowercase (obsolete)\n");
    if(local_flag & ECHO)
        printf("    -Enable echoing of input characters\n");
    if(local_flag & ECHOE)
        printf("    -Echo erase character as BS-SP-BS\n");
    
    if(local_flag & ECHOK)
        printf("    -Echo NL after kill character\n");
    if(local_flag & ECHONL)
        printf("    -Echo NL\n");
    if(local_flag & NOFLSH)
        printf("    -Disable flushing of input buffers after interrupt "
                "or quit characters\n");
    if(local_flag & IEXTEN)
        printf("Enable extended functions\n");
    
    if(local_flag & ECHOCTL)
        printf("    -Echo control characters as ^char and delete as ~?\n");
    if(local_flag & ECHOPRT)
        printf("    -Echo erased character as character erased\n");
    if(local_flag & ECHOKE)
        printf("    -BS-SP-BS entire line on line kill\n");
    if(local_flag & FLUSHO)
        printf("    -Output being flushed\n");
    if(local_flag & PENDIN)
        printf("    -Retype pending input at next read or input char\n");
    if(local_flag & TOSTOP)
        printf("    -Send SIGTTOU for background output\n");
    printf("5. Character Option\n");
    if(character_flag[VMIN] > 0)
        printf("    -minimum character to read : %d\n",character_flag[VMIN]);
    else 
        printf("    -time to wait for every character read: %d\n",character_flag[VTIME]);
}

int Uart::config(int fd, int baud) {
    struct termios  config;
    // check the devices is a tty device?
    if (!isatty (fd))
       {
            return -1;
       }
    // get the current port config
    if (tcgetattr (fd, &config) < 0)
       {
            printf ("-E- cannot get current config \r\n");
            return -1;
       }
    //printCurrentConfig();
    /*
     * 
     *  c_cflag	Control options
        c_lflag	Line options
        c_iflag	Input options
        c_oflag	Output options
        c_cc	Control characters
        c_ispeed	Input baud (new interface)
        c_ospeed	Output baud (new interface)
     */
    
    /*SETUP INPUT OPTION:
     * IGNBRK	Ignore break condition
     * BRKINT   Send a SIGINT when a break condition is detected
     * ICRNL    Map CR to NL
     * INLCR    Map NL to CR
     * PARMRK   Mark parity errors
     * INPCK	Enable parity check
     * ISTRIP	Strip parity bits
     * IXON	Enable software flow control (outgoing)
     * CSTOPB	2 stop bits
     */
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR |PARMRK | INPCK | 
            ISTRIP | IXON);
    
    /*SETUP OUTPUT OPTION*/
    config.c_oflag = 0;
    
    /* SETUP LINE OPTION
     
     */
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    
    /* SETUP CONTROL OPTION 
     */
    config.c_cflag &= ~PARENB;  // Disable parity bit
    config.c_cflag &= ~CSIZE;   // Disable bit mask for data bit
    config.c_cflag &= ~CSTOPB;  // Disable 2 Stop bit
    config.c_cflag &= ~CRTSCTS; // Disable Hardware Flow Control
    config.c_cflag |= CS8;      // Select 8 data bits
    
    /*SETUP CONTROL CHARACTERS*/
    config.c_cc[VMIN]  = 1; // minimum character to read
    /*
     * reads will block (wait) indefinitely unless the NDELAY option is set on the port with open or fcntl.
     */
    config.c_cc[VTIME] = 0; 
    
    
    // Set baud rate
    if (cfsetispeed (&config, baud) < 0 || cfsetospeed (&config, baud) < 0){
	return -1;
    } else {
    }
    if (tcsetattr (fd, TCSAFLUSH, &config) < 0){
        return -1;
    }
  
   // 
    printCurrentConfig();
    return 0;
}

int Uart::closeApi() {
    return  close(this->fd); 
}

int Uart::clearApi() {
    assert(this->fd >=0);
    return tcflush(this->fd, TCIOFLUSH);
}

int Uart::openApi(const char *device,int baud_flag) {
    /*
     Open deveice with options:
     *  O_RDWR : open for read and write permission
     * O_NOCTTY: flag tells UNIX that this program doesn't
     *  want to be the "controlling terminal" for that port. If you don't 
     * specify this then any input (such as keyboard abort signals and so forth)
     *  will affect your process. Programs like getty(1M/8) use this feature
     * when starting the login process, but normally a user program does not 
     * want this behavior.
     * O_NDELAY: enableNonblockingRead
     */
        int res = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
        assert(res >= 0);
        this->device_name = device;
        int res1 = -1;
        if(res >= 0){
            //printf("Open Device %s successful\n",device);
            this->fd = res;
            res1 = this->config(res, baud_flag);
            if(res1 > -1){
                this->fd = res;
                
                //printf("Config device %s successful\n",device_name.data());
                return 0;
            }
            
            else {
                //printf("Cannot config device %s\n",device);
                this -> fd = -1;
                assert(res1 >=1);
                return -1;
            }
            assert(res1 > -1);
        } 
        else{
            //printf("Cannot Open Device %s\n",device);
        }
}

int Uart::openApi(std::string deviceName, int baud){
    /*
     * Open Device and Config
     */
    this->openApi(deviceName.data(),baud);
    


}

int Uart::openApi(){
    assert(this->device_name.size() > 0);
    assert(this->baudrate_ > 0);
    assert(this->baudrate_flag > 0);
    this->openApi(this->device_name,this->baudrate_flag);
}

Uart::Uart() {
    this->max_rx_length = MAX_RX_BUFFER;
    this->fd = -1;
    this->device_name = "";
    this->baudrate_flag = 0;
    this->baudrate_ = 0;
    this->target_class = NULL;
    this->bufferFullCallback = NULL;
    this->lfCallback = NULL;
    this->errorCallback = NULL;
    this->timeoutCallback = NULL;
    this->byte_index = 0;
    
}

int Uart::Init(const char *devName, uint32_t baudRate){
    
    this->setBaudRate(baudRate);
    this->max_rx_length = MAX_RX_BUFFER;
    this->byte_index = 0;
    this->bufferFullCallback = uartBufferFullCallback;
    this->lfCallback = uartLfCallkback;
    this->errorCallback = uartErrorCallback;
    this->device_name = devName;
    this->openApi();
    int res =  this->setupThread(do_uart_thread,this);
    switch (res){
        case 0:
            printf("[-I-]Created Uart\n");
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
    return res;
}

int Uart::Init(UART_InitTypedef* uartInit){
    /*
     * Convert user baudrate to system baudrate
     */
    this->setBaudRate(uartInit->baudRate); 
    /*
     * Get Init data from user
     */
    this->bufferFullCallback = uartBufferFullCallback;
    this->lfCallback = uartLfCallkback;
    this->errorCallback = uartErrorCallback;
    this->device_name = uartInit->deviceName;
    this->terminator_type = uartInit->terminator_type;
    this->target_class = uartInit->target_class;
    /*
     * Setup delay time for Uart reading thread
     */
    tim.tv_sec = 0;
//    tim.tv_nsec = this->tx_time_per_byte*1000000*2;
    tim.tv_nsec = 100*1000;
    
    this->openApi(this->device_name,this->baudrate_flag);
    /*
     * set LF callback occur when last received 1 byte
     * same as user defined byte
     */
    if (uartInit->terminator_type == Uart::TERMINATOR_CUSTOM_BYTE){
        this->terminator_byte = uartInit->custom_byte;        
    }
    /*
     * Set LF callback occur when received a number of bytes reach
     * user define
     * 
     */
    else if(uartInit->terminator_type == Uart::TERMINATOR_COUNT){
        this->byte_count = uartInit->byte_count;
        this->setMinimumBytesToRead(this->byte_count);
    }
    
    /*
     * set LF callback occur when last received bytes
     * same as user defined bytes
     */
    else if(uartInit->terminator_type == Uart::TERMINATOR_CUSTOM_BYTES){
        for (int i =0; i < uartInit->custom_bytes_length; i++){
            this->terminator_bytes[i] = uartInit->custom_bytes[i];
        }
        this->terminator_bytes_length = uartInit->custom_bytes_length;   
    }   
    /*
     * set LF callback occur when receive time for a number of bytes is out
     */
    
    else if(uartInit->terminator_type == Uart::TERMINATOR_TIMEOUT){
        this->setPacketTimeout(uartInit->byte_count);
    
    }
    else if (uartInit->terminator_type == Uart::TERMINATOR_POLL)
    {
        this->byte_count = uartInit->byte_count;
    }
    
    /*
     * Create a thread to poll the uart port
     * param: do_uart_thread - the function for thread to poll
     * param: this - pointer to UART class, used by do_uart_thread
     */
    int res =  this->setupThread(do_uart_thread,this);
    switch (res){
        case 0:
            printf("[-I-]Created Uart\n");
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
    return res;   
}

int Uart::Init(std::string devName, uint32_t baud){
    this->Init(devName.data(),baud);
}

void* do_uart_thread(void* local_class) 
{ 
    Uart *obj = (Uart*)local_class;  
    while(1){
        obj->processUartThread();       
    }
}

void Uart::processUartThread() 
{
    /*
     * Check if uart is enabled
     */
    nanosleep(&this->tim,&this->tim2);
    if(this->isReady())
    {
        /*
         * Get available data on port
         */
        int n = this->getBytesAvailable();
        /*
         * Read data from Port
         */
        if (n >= this->byte_count)
        {
            uint8_t tem_buf[n];
            int len = this->readApi(tem_buf,n);
            /*
             * Check read operation
             * Add Data to rx buffer
             */
            if (n == len)
            {
                this->addDataToBuffer(tem_buf,n); 
                
            }
            
            /*
             * Process data to check callback condition
             */
            this->checkCallBack();
            
        }
        
    
    }
}

uint32_t Uart::getBaudRate()  {
    return baudrate_;
}

void Uart::setBaudRate(uint32_t baudRate) {
    this->baudrate_ = baudRate;
    this->baudrate_flag = this->getCFlagBaud(baudRate);
    /*
     * time for 1 byte transmitted in ms
     */
    this->tx_time_per_byte = (1000.0 / (double)this->baudrate_) * 10.0;
}

void Uart::start() {
    
    this->status = true;
}

void Uart::stop() {
    if (this->fd > -1){
        int res = this->closeApi();
        if (res == -1){
            printf("cannot close device %s \n",this->device_name.data());
        }
        else {
            this->status = false;
            printf("Close device %s successful\n",this->device_name.data());
            
        }
    }
    else {
        printf("Device %s is not opened\n",this->device_name.data());
    }
    
    
}

Uart::Uart(std::string devName, uint32_t baudRate) {
    this->Init(devName, baudRate);
}

void Uart::enableNonblockingRead() {
    /*         
     * Timeouts are ignored in canonical input mode or when 
     * the NDELAY option is set on the file via open or fcntl.
     */
    fcntl(this->fd, F_SETFL, FNDELAY);
}

void Uart::enableBlockingRead() {
    fcntl(this->fd, F_SETFL, 0);
}

int Uart::setupThread(void*(*fcn)(void*), void* _icom) {
    int res = 0;
    if (this->terminator_type != Uart::TERMINATOR_TIMEOUT) {
        res = pthread_create(&this->uart_thread, NULL, fcn, _icom);
    }

    return res;
}


bool Uart::isReady() {
    return status;
}

int Uart::getData(uint8_t data) {

    if (this->byte_index < this->max_rx_length - 1) 
    {
        this->rxBuf[this->byte_index] = data;
        this->byte_index++;
        this->rx_data_length = this->byte_index;     
        switch (this->terminator_type) 
        {
            case TERMINATOR_LF:
                if (data == '\n') 
                {
                    if (this->lfCallback != NULL) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_CR:
                if (data == '\r') 
                {
                    if (this->lfCallback != NULL) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_CRLF:
                if ((data == '\n')&&(this->rxBuf[this->rx_data_length - 1] == '\r')) 
                {
                    if (this->lfCallback != NULL) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_CUSTOM_BYTE:
                if (data == this->terminator_byte) 
                {
                    if (this->lfCallback != NULL) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_FAFA:
                if ((rx_data_length >= 2)&&(rxBuf[rx_data_length - 1] == 0xFA)
                        &&(rxBuf[byte_index - 2] == 0xFA)) 
                {
                    if (this->lfCallback != NULL) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_COUNT:
                if (rx_data_length >= this->byte_count) 
                {
                    if (this->lfCallback != 0) {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
            case TERMINATOR_TIMEOUT:
                //                if(this->timeoutCallback != 0){
                //                    this->timeoutCallback(this);
                //                }
                break;
            case TERMINATOR_CUSTOM_BYTES:
                if (rx_data_length >= terminator_bytes_length) {
                    bool check = true;
                    uint8_t e = this->rx_data_length - this->terminator_bytes_length;
                    for (int i = 0; i < this->terminator_bytes_length - 1; i++) 
                    {
                        if (this->terminator_bytes[i] != this->rxBuf[i + e]) 
                        {
                            check = false;
                            break;
                        }
                    }
                    if ((check == true)&&(this->lfCallback != NULL)) 
                    {
                        this->lfCallback(this, this->target_class);
                    }
                }
                break;
        }

    } 
    else 
    {
        if (this->bufferFullCallback != 0) {
            this->bufferFullCallback(this, this->target_class);
        }
    }

}

void Uart::addDataToBuffer(uint8_t* data, uint16_t length) 
{
    if (this->byte_index < this->max_rx_length - length- 2) 
    {
        /*
         * Increase index according to the length
         */
        memcpy(&rxBuf[byte_index],data,length);
        this->byte_index += length;    
        this->rx_data_length = this->byte_index;               
        

    } 
    else 
    {
        if (this->bufferFullCallback != 0) 
        {
            this->bufferFullCallback(this, this->target_class);
        }
    }
}

void Uart::checkCallBack() {
    switch (this->terminator_type)
    {
        case TERMINATOR_LF:
            for (int i = 0; i < rx_data_length; i++)
            {
                if ((rxBuf[i] == '\n') && (lfCallback != NULL))
                {
                    i = rx_data_length;
                    lfCallback(this,this->target_class);
                }


            }
            break;
        case TERMINATOR_CR:
            for (int i = 0; i < rx_data_length; i++)
            {
                if ((rxBuf[i] == '\r') && (lfCallback != NULL))
                {
                    i = rx_data_length;
                    lfCallback(this,this->target_class);
                }


            }              
            break;
        case TERMINATOR_CRLF:
            for (int i = 0; i < rx_data_length - 1; i++)
            {
                if ((rxBuf[i] == '\r') && (lfCallback != NULL)&&
                        (rxBuf[i+1] == '\n'))
                {
                    i = rx_data_length;
                    lfCallback(this,this->target_class);
                }


            }
            break;
        case TERMINATOR_CUSTOM_BYTE:
            for (int i = 0; i < rx_data_length; i++)
            {
                if ((rxBuf[i] == terminator_byte) && (lfCallback != NULL))
                {
                    i = rx_data_length;
                    lfCallback(this,this->target_class);
                }


            } 
            break;
        case TERMINATOR_FAFA:
            for (int i = 0; i < rx_data_length - 1; i++)
            {
                if ((rxBuf[i] == 0xFA) && (lfCallback != NULL)&&
                        (rxBuf[i+1] == 0xFA))
                {
                    i = rx_data_length;
                    lfCallback(this,this->target_class);
                }


            }
            break;
        case TERMINATOR_COUNT:
            if (rx_data_length >= this->byte_count) {
                if (this->lfCallback != NULL) {
                    this->lfCallback(this, this->target_class);
                }
            }
            break;
        case TERMINATOR_TIMEOUT:
            break;
        case TERMINATOR_CUSTOM_BYTES:
            if (this->rx_data_length >= this->terminator_bytes_length) {
                bool check = true;
                uint8_t e = this->rx_data_length - this->terminator_bytes_length;
                for (int i = 0; i < this->terminator_bytes_length - 1; i++) {

                    if (this->terminator_bytes[i] != this->rxBuf[i + e]) {
                        check = false;
                        break;
                    }
                }

                if ((check == true)&&(this->lfCallback != NULL)) {
                    this->lfCallback(this, this->target_class);
                }
            }
            break;
        case Uart::TERMINATOR_POLL:
            if (this->lfCallback != NULL)
            {
                this->lfCallback(this,this->target_class);
            }
            break;
        }
}



__attribute__((weak))  int uartLfCallkback(void *local_class, void *target_class){
    printf("Uart Default Callback\n");
    Uart *x = (Uart *)local_class;
    printf("Bytes Available: %d\n",x->rx_data_length);
    printf("Bytes Count: %d\n",x->byte_count);
    printf("Bytes Index: %d\n",x->byte_index);
    x->byte_index = 0;
    return 0;

}

__attribute__((weak))  int uartBufferFullCallback(void *local_class,void *target_class){
    printf("Buffer if full\n");
    Uart *x = (Uart *)local_class;
    x->byte_index = 0;
    x->rx_data_length = 0;
    return 0;

}

__attribute__((weak))  int uartErrorCallback(void *local_class,void *target_class){
    printf("Uart Error Callback\n");
    return 0;

}

__attribute__((weak)) int uartTimeoutCallback(void *local_class,void* target_class){
    printf("Time out Callback\n");
}
