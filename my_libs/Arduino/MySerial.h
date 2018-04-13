#ifndef MySerial_H
#define MySERIAL_H
#include<Arduino.h>
class MySerial
{
private:
    char enable;
  public:
   
   MySerial();
   char rx_terminal;
   char rx_buf[6];
   boolean rx_complete;
   void Init(int baud);
   void getString();
   float toFloat();
   int toInteger();
   char rx_sign();
   char rx_length();
   void reset_data();
  private:
   char *p;
   
   
};
#endif
/*
 void serialEvent(){
    while(Serial.available()){
        mSerial.getString();

    }
}
 */
