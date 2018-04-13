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

/* 
 * File:   MyTouch.hpp
 * Author: thanh
 *
 * Created on August 9, 2017, 5:54 PM
 */

#ifndef MYTOUCH_HPP
#define MYTOUCH_HPP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string>
#include<inttypes.h>
#include<pthread.h>
#include<iostream>
#include<assert.h>
#include<cstring>
#include <stddef.h>
#include <stdbool.h>
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y
#define EVENT_DEVICE    "/dev/input/event2"
typedef enum {
    TOUCH_RELEASE,
    TOUCH_CONTACT,
    TOUCH_UNKNOWN
} TOUCH_Typedef;

void *doTouchThread(void *local_class_p);
int touchContactFcn(void *local_class_p, void *target_class_p);
int touchReleaseFcn(void *local_class_p, void *target_class_p);
int timerCallback(void *local_class_p, void *target_class_p);
class MyTouch
{
public:
    virtual ~MyTouch();
    char name[256];
    void * class_data;
    
    int(*touchContactCallback)(void *, void *);
    int(*touchReleaseCallback)(void *, void *);
    
    struct timespec tim,tim2;
    void setTouchContactFcn(int(*fcn)(void*, void *));
    void setTouchReleaseFcn(int(*fcn)(void*, void *));
    
    MyTouch();;
    void Init(std::string dev_name);
    void Init();
    void Init(void *target_class);
    void Init(void *target_class,std::string dev_name);
    bool isReady();
    void setTargetClass(void *target_class_p);
    void readTouchData(void);
    void touchClean(void);
    TOUCH_Typedef getTouchStatus(void);
    


private:
    int touch_fd;
    std::string dev_name;
    struct input_event ev;
    pthread_t touch_thread;
    void setupThreadFcn(void*(*fcn)(void *),void *local_class_p);
    bool status;
    TOUCH_Typedef touch_status;
protected:        
    
};



#endif /* MYTOUCH_HPP */

