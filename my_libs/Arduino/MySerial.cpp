#include "MySerial.h"
#include "EEPROM.h"
extern HardwareSerial Serial;
const char *cStr  = "www.mlabviet.com";
MySerial::MySerial()
{
    // Checking ID on EPPROM 0 to 16
    
    
    
    this ->rx_terminal = '\n';
    this ->rx_complete = false;
    this -> p = rx_buf;
}
void MySerial::Init(int baud){
    String s1;
    char id[17];
    for (int i = 0; i < 16; i++){
       id[i] = EEPROM.read(i); 
    }
    s1 = id;
    
    if (s1.equals( cStr) == 1){
        this->enable = 1;
        Serial.print("Serial Enabled\n");
    } 
    else {
        this->enable = 0; 
        Serial.print("Serial Not Enabled\n");
    }
    if(this->enable == 1){
        Serial.begin(baud);
    }
  
}
void MySerial::getString()
{
    if(this->enable == 1){
        while(Serial.available()){
            char c = (char)Serial.read();
            if (c == (this->rx_terminal)){
              char *tam = (this -> p)-1;
              if((*tam)=='\r'){
                  (this->rx_complete )= true;   
              }else {
                  this-> reset_data();  
              } 
            }else{
              *(this->p) = c;
              (this -> p)++;
            }
        }
    }
    
}
float MySerial::toFloat()
{
  return (float)atof(this->rx_buf);
}
int MySerial::toInteger()
{
  return (int) atoi(this->rx_buf);
}
char MySerial::rx_length()
{
  return (char)*(this->p-1);
}
void MySerial::reset_data()
{
    if(this->enable == 1){
        memset(this->rx_buf,0,sizeof(this->rx_buf));
        this->p = this->rx_buf;
    }
  
}
char MySerial::rx_sign()
{
  return (char)*((this->p)-2);
}

