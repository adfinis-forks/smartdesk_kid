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
 * File:   Smartdesk.hpp
 * Author: thanh
 *
 * Created on June 20, 2017, 12:33 AM
 */

#ifndef SMARTDESK_HPP
#define SMARTDESK_HPP
#include "COMMUNICATION/Uart.hpp"
#include "AddonBoardController.hpp"
#include "TIMER/Timer.hpp"
#include "TimotionController.hpp"
int checkConnectCallback(void *timer_class, void *target_class);
class Smartdesk{
    
    
public:
    virtual ~Smartdesk();
    Smartdesk();
    static const uint8_t  UNKOWN = 0;
    static const uint8_t  ADDON_BOARD = 1;
    static const uint8_t  TIMOTION = 2;
    uint8_t controller = Smartdesk::UNKOWN;
    Uart o_uart;
    int8_t detectController();
    void init(std::string dev_name);
    Timer *checkConnectTimer;
    TiMotionController tim_obj;
    AddonBoardController addon_obj;
    std::string dev_name = "null";
    void processUartCallback();
};


#endif /* SMARTDESK_HPP */

