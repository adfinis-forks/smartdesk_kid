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
#include "LCD/MyLcd.hpp"
MyLcd * MyLcd::lcd_instance = NULL;

MyLcd* MyLcd::getInstance() {
    if(lcd_instance == NULL)
        lcd_instance = new MyLcd();
    return lcd_instance;
}
MyLcd::MyLcd() {
    printf("Khoi tao backlight\n");
    printf("Khoi tao brightness\n");
    
    // Init File Descriptor
    backligh_fd = open("/sys/class/backlight/rpi_backlight/bl_power", O_RDWR);
    brighness_fd = open("/sys/class/backlight/rpi_backlight/brightness", O_RDWR);
    if (backligh_fd == -1) {
        printf("Cannot Init Backlight\n");
    }
    if (brighness_fd == -1) {
        printf("Cannot Init Brighness\n");
    }
    // Read backlight status
    if (read(backligh_fd, backlight_value_str, 3) == -1) {
        printf("Read backlight Error\n");
    } else {
        on_off_status = (LcdStatus_Typedef) atoi(backlight_value_str);
        printf("str:%s\n",backlight_value_str);
        printf("bl:%d\n",on_off_status);

    }
    // Read brightness level
    if (read(brighness_fd, brightness_value_str, 3) == -1) {
        printf("Read brightness Error\n");
    } else {
        brightness_level = atoi(brightness_value_str);
        memset(brightness_value_str, 0, sizeof (brightness_value_str));
    }


}

int MyLcd::backlightOff() {
    const char s[] = "1";
    if (write(backligh_fd, s, 1) == -1) {
        return -1;
    } else {
        on_off_status = LCD_OFF;
        return 0;
    }
}

int MyLcd::backlightOn() {
    const char s[] = "0";
    if (write(backligh_fd, s, 1) == -1) {
        return -1;
    } else {
        on_off_status = LCD_ON;
        return 0;
    }
}

LcdStatus_Typedef MyLcd::getBacklighStatus() {
    backligh_fd = open("/sys/class/backlight/rpi_backlight/bl_power", O_RDWR);
    memset(backlight_value_str,0,sizeof(backlight_value_str));
    if (read(backligh_fd, backlight_value_str, 3) == -1) {
        printf("Read backlight Error\n");
    } else {
        on_off_status = (LcdStatus_Typedef) atoi(backlight_value_str);
    }
    return on_off_status;
}

unsigned char MyLcd::getBrightnessLevel() {
    memset(brightness_value_str, 0, sizeof (brightness_value_str));
    brighness_fd = open("/sys/class/backlight/rpi_backlight/brightness", O_RDWR);
    if (read(brighness_fd, brightness_value_str, 3) == -1) {
        printf("Read brightness Error\n");
    } else {
        brightness_level = atoi(brightness_value_str);
        
    }
    return brightness_level;
}

int MyLcd::setBrightnessLevel(unsigned char level) {
    if (level > 100) level = 100;
    unsigned int x = (unsigned int) level * 255 / 100;
    if (x > 255) x = 255;
    char s[10];
    sprintf(s, "%d", x);
    if (write(brighness_fd, s, 3) == -1) {
        return -1;
    } else return 0;

}

MyLcd::~MyLcd() {
    if (backligh_fd != -1) {
        close(backligh_fd);
    }
    if (brighness_fd != -1) {
        close(brighness_fd);
    }
    delete(lcd_instance);
}







