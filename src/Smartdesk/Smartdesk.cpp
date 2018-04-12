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
#include "SmartDesk/Smartdesk.hpp"

int8_t Smartdesk::detectController() {

}

Smartdesk::Smartdesk() {
    this->controller = UNKOWN;
}

void Smartdesk::init(std::string dev_name) {
    UART_InitTypedef uart_init;
    uart_init.baudRate = 9600;
    uart_init.deviceName = dev_name;
    uart_init.target_class = this;
    uart_init.terminator_type = Uart::TERMINATOR_FAFA;
    this->o_uart.Init(&uart_init);
    this->checkConnectTimer->Init(300000000L,10,TimotionCheckConnectCallback,this); //3s


}

Smartdesk::~Smartdesk() {
    this->o_uart.clearApi();
    
    this->o_uart.closeApi();
}


void Smartdesk::processUartCallback() {
    if(this->controller == Smartdesk::ADDON_BOARD){
        this->addon_obj.ProcessUartCallback();
    }
    else if(this->controller == Smartdesk::TIMOTION){
        this->tim_obj.ProcessUartCallback();
    
    }
    else if(this->controller == Smartdesk::UNKOWN){
        this->controller == ADDON_BOARD;
    }
}

__attribute__ ((weak)) int checkConnectCallback(void *timer_class, void *target_class){
    Timer * timer_obj = (Timer *)timer_class;
    Smartdesk *desk_obj = (Smartdesk*)target_class;
    timer_obj->Disable();
    if(desk_obj->controller == Smartdesk::ADDON_BOARD){
        desk_obj->addon_obj.Init("data.dat",desk_obj->dev_name);
    }
    else {
        //desk_obj->tim_obj.Init()
    }

}