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
 * File:   MyLcd.hpp
 * Author: thanh
 *
 * Created on August 8, 2017, 6:51 PM
 */

#ifndef MYLCD_HPP
#define MYLCD_HPP
#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef enum {
    LCD_ON,
    LCD_OFF
} LcdStatus_Typedef;
class MyLcd{
public:
    virtual ~MyLcd();
    
    static MyLcd *lcd_instance;
    static MyLcd *getInstance();
    int backlightOn();
    int backlightOff();
    LcdStatus_Typedef getBacklighStatus(void);
    unsigned char getBrightnessLevel(void);;
    int setBrightnessLevel(unsigned char level);
private:
    MyLcd();
    int backligh_fd;
    int brighness_fd;
    LcdStatus_Typedef on_off_status;
    unsigned char brightness_level;
    char backlight_value_str[10];
    char brightness_value_str[10];
protected:
};


#endif /* MYLCD_HPP */

