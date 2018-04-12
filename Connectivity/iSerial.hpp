/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iSerial.hpp
 * Author: ThanhLe
 * Company: autonomous.ai
 * Email: qthanh_le@hotmail.com
 *
 * Created on March 31, 2016, 1:31 AM
 */

#ifndef __ISERIAL_HPP
#define __ISERIAL_HPP
#include"iCommunication.hpp"
class iSerial : public iCommunication{
public:
    std::string deviceName;
    const char * getDeviceName()  {
        return deviceName.data();
    }
    void setDeviceName(char* deviceName) {
        this->deviceName = deviceName;
    }
    void setDeviceName(std::string devName){
        this->deviceName = devName;
    }
    virtual ~iSerial(){
    
    }
    iSerial(){
    
    }
    //Interface functions inherited from iCommnunication
    /*
     * Overload for put String method
     */
    virtual char putString(char * s) = 0;
    virtual char putString(std::string s) = 0;
    virtual char putString(const char * s)= 0;
    /*
     * Overload for putByte method.
     */
    virtual char putByte(void *data) = 0;
    virtual char putByte(char data) = 0;
    virtual char putByte(unsigned char data) = 0;
    virtual char putBytes(void *data, int len) = 0;
    /*
     * Each device will have different  start/stop/init condition
     */
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void init()=0;


    

    
    







    





    














    



    public:
//        virtual ~iSerial(){printf("Deconstruct iSerial\n");}
            




};


#endif /* ISERIAL_HPP */

