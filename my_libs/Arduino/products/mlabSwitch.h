#ifndef __MLAB_SWITCH
#define __MLAB_SWITCH
#include <Arduino.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
typedef enum WIFI_STATUS
{
	UNKNOWN = 0,
	CONNECT_SSID,
	CONNECT_SMART,
	CONNECTED

};
typedef enum a {
	OFF = 0,
	ON

}LED_Status_Typedef;
using namespace std;
class MlabSwitch
{
public:
	MlabSwitch();
	MlabSwitch( String host,  String auth);
	~MlabSwitch();
	void Init();
	void Init(String host, String auth);
	void Run();
	int initWifi(void);
	void initIO(uint8 relay_pin, uint8 status_pin, uint8 button_pin);
	int initFirebase(String host, String auth);
	int startSmartConfig(void);
	void relayOn(void);
	void relayOff(void);
	void setRelay();
	void ledStatusOn(void);
	void ledStatusOff(void);
	void getInput(void);

private:
	String chip_id;
	int ii = 10;
	uint8_t w_status = UNKNOWN;
	uint8 count = 0;
	String host;
	String auth;
	bool status = false;
	String chipId = "123";
	byte mac[6];
	bool l1 = false;
	bool l2 = false;
	bool l3 = false;
	bool firstTime = true;
	bool button_status = false;
	String ssid;
	String psk;
	uint8 relay_pin;
	uint8 status_pin;
	uint8 button_pin;

		 

};


#endif
