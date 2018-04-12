/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ZMQ/iZmq.hpp"
// Implement ZmqContext Class;

//iZmq::iZmq(ZmqContext zmqContext) {
//
//}

ZmqContext * ZmqContext::m_instance = NULL;
ZmqContext* ZmqContext::getInstance(){
    if (m_instance == NULL)
	{
            m_instance = new ZmqContext();
	}
 
	return m_instance;
}

ZmqContext::ZmqContext() {
//    std::cout<<"Create Zmq Context\n";
    this->setup();
}

int ZmqContext::setup() {
    this->context = zmq_ctx_new();
    if (this -> context != NULL){
        std::cout<<"[-I-]Created Zmq Context\n";
        return 0;
    }
        
    else{
        std::cout<<"[-E-] Cannot Create Zmq Context\n";
        return -1;
    }
    assert(this->context != NULL);
}

int ZmqContext::destroy() {
    int ctx_destroy = zmq_ctx_destroy(this->context);   
    assert(ctx_destroy == 0);
    return ctx_destroy;
}

int ZmqContext::setIPv6(int value) {

}

int ZmqContext::setMaxSocket(int value) {

}

int ZmqContext::setNumberOfThread(int value) {

}


int ZmqContext::shutdown() {
    int ctx_shutdown = zmq_ctx_shutdown(this->context);
    return ctx_shutdown;
    assert(ctx_shutdown == 0);
}

int ZmqContext::terminate() {
    int ctx_term = zmq_ctx_term(this->context);
    assert(ctx_term == 0);
    return ctx_term;
}

ZmqContext::~ZmqContext() {
//    std::cout<<"[-I-]Deletet ZMQ Context\n";
}
//Requester implement:

iZmq::iZmq() {
//    std::cout << "Create iZmq\n";
}

iZmq::~iZmq() {
//    std::cout << "Delete iZmq\n";
}

std::string iZmq::getEndPoint() const {
    return endPoint;
}

int iZmq::getTimeOut() const {
    return timeOut;
}

void iZmq::setEndPoint(std::string endPoint) {
    this->endPoint = endPoint;
}

void iZmq::setTimeOut(int timeOut) {
    this->timeOut = timeOut;
}


