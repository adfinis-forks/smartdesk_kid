/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"ZMQ/ZmqRequester.hpp"

void ZmqRequester::Delete()
{
  this->unBind();
  zmq_close(this->requester);
}

void ZmqRequester::init() {
}

int ZmqRequester::setup(void* ctx) {
    this->requester = zmq_socket(ctx,ZMQ_REQ);
    if(this->requester != NULL) printf("Create Requester OK\n");
    else printf("Create Requester Fail\n");
    assert(this->requester != NULL);
    return zmq_setsockopt(this->requester,ZMQ_RCVTIMEO,&this->timeOut,4);

}

void ZmqRequester::start() {    
    status = true;
}

void ZmqRequester::stop() {
//    int res = this->unBind();
//    if(res == 0) zmq_close(this->requester);    
    this->status = false;
}

int ZmqRequester::sendMore() {
    

}

int ZmqRequester::sendOne(void* data, int len) {
    const void *txData = (const void*)data;
    int nlen = zmq_send(this->requester,txData,(size_t)len,0);
    assert(nlen = len);
    return nlen;

}

int ZmqRequester::sendNull() {
    int nlen = zmq_send(this->requester,NULL,0,0);
    assert(nlen == 0);
    return nlen;
}

int ZmqRequester::receiveOne() {
        try{
        int nlen = zmq_recv(this->requester,(char *)this->rx_buf,size_t(this->max_rf_buf),0);
//        assert(nlen >= 0);  
        if ((nlen > 0)&&(nlen <= this->max_rf_buf )){   
            this->rx_buf_length = nlen;
        } 
        else {
            
            this->rx_buf_length = 0;
        }
        return this->rx_buf_length;
    }
    catch (const std::exception& e){
        std::cout << " a standard exception was caught, with message '"
                  << e.what() << "'\n";
    } 

}

int ZmqRequester::receiveMore() {

}

int ZmqRequester::bind() {
    return zmq_connect(this->requester,this->endPoint.data());
}

int ZmqRequester::unBind() {
    return zmq_disconnect(this->requester,this->endPoint.data());
}

char ZmqRequester::putByte(void* data) {
    this->sendOne(data,1);
}

char ZmqRequester::putByte(char data){
    this->sendOne(&data,1);
}

char ZmqRequester::putByte(unsigned char data){
    this->sendOne(&data,1);
}

char ZmqRequester::putBytes(void* data, int len) {
    this->sendOne(data,len);
}

char ZmqRequester::putChar(char c) {
    this->sendOne(&c,1);
}

char ZmqRequester::putString(const char* s) {
    this->putBytes((void*)s,strlen(s));
}

char ZmqRequester::putString(std::string s){
    this->putBytes((void*)s.data(),strlen(s.data()));
}

char ZmqRequester::putString(char* s){
    this->putBytes((void*)s,strlen(s));
}

void* doReqThread(void* local_class) {
    ZmqRequester *x = (ZmqRequester*)local_class;
    while(1){
        if(x->isReady() > 0){
            nanosleep(&x->tim,&x->tim2);
            if(x->isReady() == READY){
                //Check data
                if(x->receiveOne() > 0){
                    x->LFCallback(x,x->target_class );
                }              
            }    
        }
    }
}
ZmqRequester::ZmqRequester() {
    printf("Khoi tao ZmqRequester\n");
    this->setup(ZmqContext::getInstance()->context);
//    this->Disable();
    this->status = false;
}

ZmqRequester::~ZmqRequester() {

}

int ZmqRequester::setupThread(void*(*fcn)(void*), void* _icom) {
    int res = 0;
    if (this->terminator_type != iCommunication::TERMINATOR_TIMEOUT) {
        res = pthread_create(&this->req_thread, NULL, fcn, _icom);
    }

    return res;
}

int ZmqRequester::Init(std::string endPoint){
        this->setEndPoint(endPoint);
        this->max_rf_buf = 512;
        this->setTimeOut(500);
        this->tim.tv_nsec = 10000000L; //10 ms
        this->LFCallback = ZmqReqCallback;
        this->bufferFullCallback = ZmqReqBufferFullcallback;
        this->ErrorCallback = ZmqReqErrorCallback;
        this->target_class = this;
        int res = this->setupThread(doReqThread,this);
        switch (res){
            case 0:
                printf("-I- Created Zmq Requester Thread\n");
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
}

int ZmqRequester::Init(std::string endPoint, void* target_class) {
    this->target_class = target_class;
    this->Init(endPoint);
}



__attribute((weak)) int ZmqReqCallback(void *local_class, void *target_class){
    ZmqRequester *obj = (ZmqRequester *)local_class;
    
    int x = obj->rx_buf[obj->rx_buf_length -5]*256 + obj->rx_buf[obj->rx_buf_length - 4];
    printf("Received data From Replier, data= %d,len = %d\n",x,obj->rx_buf_length);
    if (obj->rx_buf_length > 0){
        for (int i = 0; i < obj->rx_buf_length; i++)
            printf("%x",obj->rx_buf[i]);
    }
    
    return 0;
    
    
    
    
}

__attribute((weak)) int ZmqReqBufferFullcallback(void *local_class, void *target_class){
    printf("Zmq Requester buffer full callback\n");
    return 0;
}
__attribute((weak)) int ZmqReqErrorCallback(void *local_class, void *target_class){
    printf("Zmq Requester error callback\n");
    return 0;
}
