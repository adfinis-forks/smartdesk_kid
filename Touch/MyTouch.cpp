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

#include "Touch/MyTouch.hpp"

__attribute__((weak)) void* doTouchThread(void* local_class_p) {
    MyTouch *x = (MyTouch*)local_class_p;
    while(1){
        nanosleep(&x->tim,&x->tim2);
        if(x->isReady()){
            x->readTouchData();   
        }
    };
    return 0;
}

__attribute__((weak)) int touchContactFcn(void* local_class_p, void* target_class_p)
{
    MyTouch *l_obj = (MyTouch *)local_class_p;
    printf("Touch Contacted\n");
    l_obj->touchClean();
    
}

__attribute__((weak)) int touchReleaseFcn(void* local_class_p, void* target_class_p)
{
    MyTouch *l_obj = (MyTouch *)local_class_p;
    printf("Touch Release\n");
    l_obj->touchClean();
    
}

int timerCallback(void *local_class_p, void *target_class_p){
    MyTouch *obj = (MyTouch *)target_class_p;
    
}

void MyTouch::Init(std::string dev_name) {
    dev_name = dev_name;
    touch_fd = open(dev_name.data(), O_RDONLY);
    if (touch_fd < 0) {
        printf("Cannot Open Device %s\n", dev_name.data());
        status = false;
    } else {
        ioctl(touch_fd, EVIOCGNAME(sizeof (name)), name);
        printf("Reading from:\n");
        printf("device name = %s\n", name);
        status = true;
    }

}

void MyTouch::Init() {
    Init(EVENT_DEVICE);    
    tim.tv_sec = 0;
    tim.tv_nsec = 1000000L; // 1ms
}

void MyTouch::Init(void* target_class) {
    this->Init();
    this->setTargetClass(target_class);
}

void MyTouch::Init(void* target_class, std::string dev_name) {
    this->Init(dev_name);
    this->setTargetClass(target_class);
}


MyTouch::MyTouch() {
    sprintf(name, "%s", "Unknown");
    status = false;
    this->class_data = NULL;
    touch_status = TOUCH_UNKNOWN;
    this->setTouchContactFcn(touchContactFcn);
    this->setTouchReleaseFcn(touchReleaseFcn);
    setupThreadFcn(doTouchThread, this);

}

bool MyTouch::isReady() {
    return status;
}

TOUCH_Typedef MyTouch::getTouchStatus() {
    return touch_status;
}

void MyTouch::touchClean() {
    touch_status = TOUCH_UNKNOWN;
}


void MyTouch::readTouchData() {
    const size_t ev_size = sizeof (struct input_event);
    ssize_t size;
    size = read(touch_fd, &ev, ev_size);
    if (size < ev_size) {
        printf("Error Size when reading\n");
        close(touch_fd);
    } 
    else
    {
        
        
        if (ev.type == EVENT_TYPE && (ev.code == EVENT_CODE_X
            || ev.code == EVENT_CODE_Y)) {
            //printf("%s=%d\n", ev.code == EVENT_CODE_X ? "X" : "Y", ev.value);
        }
        if (ev.type = EVENT_TYPE && ev.code == BTN_TOUCH ){
            if (ev.value){
                (*this->touchContactCallback)(this,this->class_data);
                //this->touch_status = TOUCH_CONTACT;
            }
            else {
                (*this->touchReleaseCallback)(this,this->class_data);
                //this->touch_status = TOUCH_RELEASE;
            }
        }
        if (ev.type = EVENT_TYPE && ev.code == BTN_TOOL_QUINTTAP ){
            printf("Co touch 5 ngon tay\n");
        }
        if (ev.type = EVENT_TYPE && ev.code == BTN_TOOL_FINGER ){
            printf("Co touch 1 ngon\n");
        }
        if (ev.type = EVENT_TYPE && ev.code == BTN_TOOL_DOUBLETAP ){
            printf("Co touch 2 ngon\n");
        }
    }

}

void MyTouch::setTargetClass(void* target_class_p) {
    this->class_data = target_class_p;

}

void MyTouch::setTouchContactFcn(int(*fcn)(void*,void*)) {
    this->touchContactCallback = fcn;
}

void MyTouch::setTouchReleaseFcn(int(*fcn)(void*, void*)) {
    this->touchReleaseCallback = fcn;
}


void MyTouch::setupThreadFcn(void*(*fcn)(void*), void* local_class_p) {
    pthread_create(&this->touch_thread, 0, fcn, local_class_p);
}

MyTouch::~MyTouch() {
    if (touch_fd >= 0)
    close(touch_fd);

}









