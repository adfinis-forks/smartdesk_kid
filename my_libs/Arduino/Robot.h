#ifndef ROBOT_H
#define ROBOT_H
#include "MyServo.h"
#define JOINT1_CTL_PIN	6
#define JOINT1_EN_PIN	0
#define JOINT2_CTL_PIN	7
#define JOINT2_EN_PIN	0
#define JOINT3_CTL_PIN	8
#define JOINT3_EN_PIN	0
#define JOINT4_CTL_PIN	9
#define JOINT4_EN_PIN	0
#define FREE  0
#define BUSY  1
#define RB_FREE_STATE   0
#define RB_HOME_POS 	1
#define RB_GAP_POS 		2
#define RB_THA1_POS 	3
#define RB_THA2_POS 	4
#define RB_THA3_POS 	5
#define RB_TRIGGER_GRIPPER 6
#define RB_RELEASE_GRIPPER 7
#define RB_SENSOR_DETECTED 8
#define RB_SENSOR_NOT_DETECTED 9

typedef struct{
	char angle1;
	char angle2;
	char angle3;

}RobotData;
class Robot{
private:
    char enable;
public:
    MyServo Joint1, Joint2, Joint3,Gripper;
    void Init();
    void run();
    void pause();
    void moveToHome();
    void move(RobotData *rData);
    void gapVat();
    void thaVat();
    void prepare();
    char status;
    char CurState;
    char PreState;
    char stateUpdated;
    RobotData HomeData,ViTriGap,ThaVat1,ThaVat2,ThaVat3;
    void UpdateState(char state);
		
};


#endif



































