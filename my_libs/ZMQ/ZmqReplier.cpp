/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include"ZMQ/ZmqReplier.hpp"
char ZmqReplier::putByte(void* data) {
    this->sendOne((void *)data,1);
}
char ZmqReplier::putByte(char data){
    this->sendOne((void *)&data,1);
}

char ZmqReplier::putByte(unsigned char data){
    this->sendOne((void *)&data,1);
}


char ZmqReplier::putBytes(void* data, int len) {
    this->sendOne((void *)data,len);
}

char ZmqReplier::putChar(char c) {
    this->sendOne((void *)&c,1);
}

char ZmqReplier::putString(char* s) {
    this->putBytes((void *)s, strlen(s));

}
char ZmqReplier::putString(std::string s){
 
    const char * ss = s.data();
    this->putBytes((void *)ss,strlen(ss));
}

char ZmqReplier::putString(const char* s){
    this->putBytes((void *)s, strlen(s));
}
//

int ZmqReplier::receiveMore() {

}

int ZmqReplier::receiveOne() {
    // Sau khi Receive thi data duoc luu tru trong rxBuf
    try{
        memset(this->rx_buf,0,sizeof(this->rx_buf));
        int nlen = zmq_recv(this->replier,(char *)this->rx_buf,size_t(this->max_rx_buf),0); 
        if ((nlen > 0)&&(nlen <= this->max_rx_buf )){
            this->rx_data_length = nlen;
            this->byte_count = nlen;
        } 
        else {
            this->rx_data_length = 0;
        }
        return this->rx_data_length;
    }
    catch (const std::exception& e){
        std::cout << " a standard exception was caught, with message '"
                  << e.what() << "'\n";
    } 
}

int ZmqReplier::sendMore() {

}

int ZmqReplier::sendNull() {
    int nlen = zmq_send(this->replier,NULL,0,0);
    assert(nlen == 0);
    return nlen;
}

int ZmqReplier::sendOne(void* data, int len) {
    uint8_t *txData = (uint8_t*)data;
    int nlen = zmq_send(this->replier,txData,(size_t)len,0);
    assert(nlen == len);
    return nlen;
}

int ZmqReplier::setup(void* ctx) {

    this->replier  = zmq_socket(ctx,ZMQ_REP);
    assert(this->replier != NULL);
    if(this->replier != NULL) {
        printf("[-I-]Created Zmq replier\n");
    }
    else printf("[-E-]Cannot create Zmq replier\n");
    
    assert(zmq_setsockopt(this->replier,ZMQ_RCVTIMEO,&this->timeOut,4)==0);
}

void ZmqReplier::start() {
    this->status = true;
}

void ZmqReplier::stop(){
    this->status = false;
}

ZmqReplier::~ZmqReplier() {
//    std::cout<<"Delete ZMPQReplier\n";
}

int ZmqReplier::bind() {
    int res =  zmq_bind(this->replier,this->endPoint.data());
    
    return res;
}

int ZmqReplier::unBind() {
    return zmq_unbind(this->replier,this->endPoint.data());
}

ZmqReplier::ZmqReplier(){ 
    this->setup(ZmqContext::getInstance()->context);
    this->start();
//    printf("Create ZmqReplier\n");
}

void* doRepThread(void* local_class) {
    
    ZmqReplier *x = (ZmqReplier*)local_class;
    while(1){
        nanosleep(&x->tim , &x->tim2);
        if (x->isReady() == READY){
            if(x->receiveOne() > 0){
                x->LFCallback(x,x->target_class);
            }
        }
        
    }
}

void ZmqReplier::init() {
    // Can phai set enpoint, maxRxLeng, TimeOut truoc
    
}

int ZmqReplier::Init(std::string endPoint){
    this->setEndPoint(endPoint);
    this->max_rx_buf = 512;
    this->setTimeOut(500);
    this->tim.tv_nsec = 100000000L; // 100ms
    this->LFCallback = ZmpReplierCallback;
    this->bufferFullCallback = ZmqRepBufferFullcallback;
    this->ErrorCallback = ZmqRepErrorCallback;
    int res =  this->setupThread(doRepThread,this);
    switch (res){
        case 0:
            printf("[-I-]Created Zmq Replier Thread\n");
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

int ZmqReplier::Init(std::string endPoint, void* target_class) {
    this->target_class = target_class;
    return this->Init(endPoint);
}

bool ZmqReplier::isReady() {
    return this->status;
}

int ZmqReplier::setupThread(void*(*fcn)(void*), void* _icom) {
    int res = 0;
    if (this->terminator_type != iCommunication::TERMINATOR_TIMEOUT) {
        res = pthread_create(&this->rep_thread, NULL, fcn, _icom);
    }

    return res;
}

__attribute__ ((weak))int ZmpReplierCallback(void *local_class, 
        void * target_class){
    
    printf("Zmq replier data in callback\n");
    return 0;
}
__attribute__ ((weak)) int ZmqRepBufferFullcallback(void *local_class,
        void * target_class){
    printf("Zmq replier buffer full callback\n");
    return 0;
}
__attribute__ ((weak)) int ZmqRepErrorCallback(void *local_class,
        void * target_class){
    printf("Zmq replier error callback\n");
    return 0;
}