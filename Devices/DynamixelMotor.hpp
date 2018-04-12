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
 * File:   DynamixelMotor.hpp
 * Author: thanh
 *
 * Created on May 3, 2017, 9:03 PM
 */

#ifndef __DYNAMIXELMOTOR_HPP
#define __DYNAMIXELMOTOR_HPP
#include "COMMUNICATION/Uart.hpp"
#include "TIMER/Timer.hpp"
#include <cmath>
typedef struct {
    uint8_t id;
    double sign;
    double home_pos_deg; //
    double pos_range_deg;
    
    double min_speed_rpm;
    double max_speed_rpm;
    double max_torque;
    double min_punch;
    double max_punch;
    
    std::string type;

}Joint;
class DynamixelMotor{

    
public:
    static const uint8_t     MODEL_NUM_L		= 1;
    static const uint8_t     MODEL_NUM_H		= 2;
    static const uint8_t     MOTOR_ID                   = 3;
    static const uint8_t     BAUDRATE                   = 4;
    static const uint8_t     RETURN_DELAY_TIME          = 5;
    static const uint8_t     CW_ANGLE_LIMIT_L           = 6;
    static const uint8_t     CW_ANGLE_LIMIT_H           = 7;
    static const uint8_t     CCW_ANGLE_LIMIT_L          = 8;
    static const uint8_t     CCW_ANGLE_LIMIT_H          = 9;
    static const uint8_t     HIGHEST_LIMIT_TEM          = 11;
    static const uint8_t     LOWEST_LIMIT_VOLTAGE	= 12;
    static const uint8_t     HIGHEST_LIMIT_VOLTAGE	= 13;
    static const uint8_t     MAX_TORQUE_L		= 14;
    static const uint8_t     MAX_TORQUE_H		= 15;
    static const uint8_t     STATUS_RETURN_LEVEL	= 16;
    static const uint8_t     ALARM_LED                  = 17;
    static const uint8_t     ALARM_SHUTDOWN		= 18;	
    static const uint8_t     TORQUE_ENABLE		= 24;
    static const uint8_t     LED                        = 25;
    static const uint8_t     CW_MARGIN                  = 26;
    static const uint8_t     CCW_MARGIN                 = 27;
    static const uint8_t     CW_SLOPE                   = 28;
    static const uint8_t     CCW_SLOPE                  = 29;
    static const uint8_t     GOAL_POS_L                 = 30;
    static const uint8_t     GOAL_POS_H                 = 31;
    static const uint8_t     SPEED_L			= 32;
    static const uint8_t     SPEED_H			= 33;
    static const uint8_t     TORQUE_LIMIT_L		= 34;
    static const uint8_t     TORQUE_LIMIT_H		= 35;
    static const uint8_t     PRESENT_POS_L		= 36; 
    static const uint8_t     PRESENT_POS_H		= 37;
    static const uint8_t     PRESENT_SPEED_L		= 38;
    static const uint8_t     PRESENT_SPEED_H		= 39;
    static const uint8_t     PRESENT_LOAD_L		= 40;
    static const uint8_t     PRESENT_LOAD_H		= 41;
    static const uint8_t     PRESENT_VOLTAGE		= 42;
    static const uint8_t     PRESENT_TEM		= 43;
    static const uint8_t     MOVING			= 46;
    static const uint8_t     PUNCH_L			= 48;
    static const uint8_t     PUNCH_H			= 49;

    static const uint8_t     MAXNUM_TXPARAM             = 150;
    static const uint8_t     MAXNUM_RXPARAM 		= 60;  	
    static const uint8_t     BROADCAST_ID               = 254;		

    // Instruction for communication - Hardware Define
    static const uint8_t     INST_PING                  = 1; 		
    static const uint8_t     INST_READ  		= 2;		
    static const uint8_t     INST_WRITE 		= 3;	
    static const uint8_t     INST_REG_WRITE 		= 4; 	
    static const uint8_t     INST_ACTION 		= 5;		
    static const uint8_t     INST_RESET 		= 6;			
    static const uint8_t     INST_SYNC_WRITE            = 131;	
    
    static const uint8_t     ERRBIT_VOLTAGE 		= 1; 	
    static const uint8_t     ERRBIT_ANGLE 		= 2;	
    static const uint8_t     ERRBIT_OVERHEAT            = 4; 	


    static const uint8_t     ERRBIT_RANGE 		= 8; 		
    static const uint8_t     ERRBIT_CHECKSUM            = 16;	
    static const uint8_t     ERRBIT_OVERLOAD            = 32;	
    static const uint8_t     ERRBIT_INSTRUCTION 	= 64;
   
    
    static const uint8_t IND_START_BYTE1           = 0;
    static const uint8_t IND_START_BYTE2           = 1;
    static const uint8_t IND_ID                    = 2;
    /*
     * The length of the packet where its value is “Number of parameters (N) + 2”
     */
    static const uint8_t IND_LENGTH                = 3; 
    static const uint8_t IND_INSTRUCTION           = 4;
    static const uint8_t ERRBIT                = 4;
    static const uint8_t IND_START_ADDRESS         = 5;
    static const uint8_t IND_PARAM = 6;
    static const uint8_t START_BYTE1         = 255;
    static const uint8_t START_BYTE2         = 255;
    
    static const uint8_t E_INPUT_VOL            = 1<<0;
    static const uint8_t E_ANGLE_LIMIT          = 1<<1;
    static const uint8_t E_OVERHEATING          = 1<<2;
    static const uint8_t E_RANGE                = 1<<3;
    static const uint8_t E_CHECKSUM             = 1<<4;
    static const uint8_t E_OVERLOAD             = 1<<5;
    static const uint8_t E_INSTRUCTION          = 1<<6;
    Uart v_uart;
    bool port_busy = false;
    bool timer_triggered = false;
    Timer *v_timer;
    DynamixelMotor();
    DynamixelMotor(Joint *joint);
    DynamixelMotor(uint8_t id);
    void init(Joint *joint);
    DynamixelMotor *motors;
    void setSpeed(double sped_rpm);
    
    void reset();
    void setDegPos(double pos_deg);
    
    double getDegPos();
    
    void setRadPos(double pos_rad);
    void setTorque(double torque_percent); // 0% to 100%
    void setPunch(double punch_percent); // 0% to 100%
    
    
    
    
    void enableTorque(bool cmd);
    double getRadPos();
    uint8_t getId();
    void ping(uint8_t _id);
    void ping();
    void resetTxBuffer();
    void resetRxBuffer();
    void updateTxChecksum();    
    static uint8_t getLowByte(uint16_t word);
    static uint8_t getHighByte(uint16_t word);
    void printTxBuf();
    void ledOn();
    void ledOff();
    void setDefautSmoothlyMove();
    
    uint8_t tx_buffer[DynamixelMotor::MAXNUM_TXPARAM];
    uint8_t rx_buffer[DynamixelMotor::MAXNUM_RXPARAM];
    uint16_t tx_length; // number of bytes to be transmitted
    uint16_t rx_length;
    
    bool isMoving();
    uint8_t getTemperature();
    uint8_t getReturnLevel();
    void setReturnLevel(uint8_t level);
    double getSpeed();
private:
    /*
     * Speed
     */
    uint16_t max_speed_reg; // limited by product 
    double min_speed;       // limited by product
    double max_speed;       // limited by product
    double min_speed_limit; // limited by user
    double max_speed_limit; // limited by user
    double speed_resolution;// 1023:113
    double default_speed = 1;
    
    /*
     * Position
     */
    uint16_t max_pos_reg;   // 1023 for AX12 4096 for MX28
    double min_pos;         // limited by product 
    double max_pos;         // limited by product 
    double min_pos_limit;   // limited by user
    double max_pos_limit;   // limited by user 
    double pos_resolution;  // AX12: 1023/300
    double home_pos;        // (max_pos - min_pos)/2
    double current_pos;     //
    double pos_range;
    /*
     * torque
     */
    uint16_t max_torque_reg;    // limited by product
    double max_torque;         // limited by product
    double min_torque;         //limited by product
    double max_torque_limit;  // limited by user
    double min_torque_limit;   // limited by user
    double torque_resolution;
    double default_torque = 100;
    /*
     * Punch
     */
    uint16_t max_punch_reg;
    double min_punch;
    double max_punch;
    double min_punch_limit;
    double max_punch_limit;
    double punch_resolution;
    double default_punch;
    
    
    std::string motor_type="AX12";
   
    uint8_t id;
    double sign;
     
    bool uart_busy = false;
    void dlxSend();
    void trigger();
    /*
     * Write functions
     */
    
    void writeRegByteNoAction(uint8_t address, uint8_t value);
    
    void writeRegByteAction(uint8_t address, uint8_t value);        

    void writeRegWordNoAction(uint8_t address, uint16_t value);
    
    void writeRegWordAction(uint8_t address, uint16_t value);
    
    
    /*
     * Read Function
     */
    void readRegByteAction(uint8_t address);
    
    void readRegWordAction(uint8_t address);
    /*
     * Utilities functions
     */
    double posCheck(double pos);
    
    double speedCheck(double speed);
    
    double torqueCheck(double torque);
    
    double punchCheck(double punch);
    
};


#endif /* DYNAMIXELMOTOR_HPP */

