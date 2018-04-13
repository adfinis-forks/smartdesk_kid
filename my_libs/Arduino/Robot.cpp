#include "Robot.h"
#include "EEPROM.h"
extern HardwareSerial Serial;
const char *cStr  = "www.mlabviet.com";
void Robot::Init(){
    String s1;
    char id[17];
    for (int i = 0; i < 16; i++){
       id[i] = EEPROM.read(i); 
    }
    s1 = id;
    
    if (s1.equals( cStr) == 1){
        this->enable = 1;
    } 
    else 
        this->enable = 0;
    
    //Init Joint 1
      this-> Joint1.init(JOINT1_CTL_PIN, JOINT1_EN_PIN);
        this->Joint1.maxX = 90;
        this->Joint1.minX = -40;
      this-> Joint2.init(JOINT2_CTL_PIN, JOINT2_EN_PIN);
      this-> Joint3.init(JOINT3_CTL_PIN, JOINT3_EN_PIN);
    // Init Gripper
      this->Gripper.init(JOINT4_CTL_PIN, JOINT4_EN_PIN);
        this->Gripper.maxX = 0;
        this->Gripper.minX = -60;
        this->status = RB_HOME_POS;
    //Define Vi tri Home
        this->HomeData.angle1 = 0;
        this->HomeData.angle2 = -20;
        this->HomeData.angle3 = -10;
    // Define Vi tri Gap Vat
        this->ViTriGap.angle1 = -35;
        this->ViTriGap.angle2 = 45;
        this->ViTriGap.angle3 = -70;
    // Define Vi tri tha vat 1
        this->ThaVat1.angle1 = 10;
        this->ThaVat1.angle2 = 45;
        this->ThaVat1.angle3 = -70;
    // Define Vi tri tha vat 2
        this->ThaVat2.angle1 = 45;
        this->ThaVat2.angle2 = 45;
        this->ThaVat2.angle3 = -70;
    // Define Vi tri tha vat 3
        this->ThaVat3.angle1 = 80;
        this->ThaVat3.angle2 = 45;
        this->ThaVat3.angle3 = -70;
    // Set Default State
        this->CurState = FREE_STATE;
        this->PreState = FREE_STATE;
        this->stateUpdated = false;
	
	
}
void Robot::moveToHome(){
	
}
void Robot::move(RobotData *rData){
    if(this->enable == 1){
        this->Joint1.writeAngle(rData->angle1);
	this->Joint2.writeAngle(rData->angle2);
	this->Joint3.writeAngle(rData->angle3);
    }
	
}
void Robot::gapVat(){
    if(this->enable == 1){
        this->Gripper.writeAngle(this->Gripper.maxX);
    }
	
}
void Robot::thaVat(){
    if(this->enable == 1){
        this->Gripper.writeAngle(this->Gripper.minX);
    }
	
}
void Robot::UpdateState(char state){
    if(this->enable == 1){
        this->PreState = this->CurState;
	this->CurState = state;
    }
	

}
void Robot::run(){
    if(this->enable == 1){
	this->Joint1.turnServo();
	this->Joint2.turnServo();
	this->Joint3.turnServo();
	this->Gripper.turnServo();
	if((this->Joint1.state == FREE_STATE)&&(this->Joint2.state == FREE_STATE)&&(this->Joint3.state == FREE_STATE)&&(this->Gripper.state == FREE_STATE)){
		switch (this->CurState){
			case RB_TRIGGER_GRIPPER:
				Serial.println("GRIPPER_HOLD");
				break;
			case RB_RELEASE_GRIPPER:
				Serial.println("GRIPPER_RELEASED");
				break;
			case RB_HOME_POS:
				Serial.println("AT_HOME");
				break;
			case RB_GAP_POS:
				Serial.println("AT_GAP");
				break;
			case RB_THA1_POS:
				Serial.println("AT_THA1");
				break;
			case RB_THA2_POS:
				Serial.println("AT_THA2");
				break;
			case RB_THA3_POS:
				Serial.println("AT_THA3");
				break;
      case RB_SENSOR_DETECTED:
        Serial.println("SENSOR_DETECTED");
        break;
      case RB_SENSOR_NOT_DETECTED:
        Serial.println("SENSOR_NOT_DETECTED");
        break;

		}
		this->UpdateState(RB_FREE_STATE );
	}
	else{
		this->stateUpdated = false;
	}
    }
}
void Robot::prepare(){
	
}











