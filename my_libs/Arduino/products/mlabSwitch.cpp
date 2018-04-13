#include "../../../inc/Arduino/products/mlabSwitch.h"
MlabSwitch::MlabSwitch()
{
	Serial.begin(115200);
	char s[20];
	sprintf(s, "%d", ESP.getChipId());
	Serial.print("ID:");
	
	this->chip_id = s;
	Serial.println(this->chip_id.c_str());
}

MlabSwitch::~MlabSwitch()
{
}
void MlabSwitch::Init()
{
	Serial.println("OK");
	if ((this->host.length() != 0) &&(this->auth.length() != 0))
	{
		this->Init(this->host, this->auth);

	}
}
void MlabSwitch::Init(const String host, const String auth)
{
	pinMode(D7, OUTPUT);
	pinMode(D1, OUTPUT);
	pinMode(D6, INPUT);
	digitalWrite(D7, LOW);
	digitalWrite(D1, LOW);
	Serial.print("Chip ID ");
	Serial.println(ESP.getChipId());
	button_status = (bool)digitalRead(D6);
	Serial.print("Button: ");
	Serial.println(button_status);

	this->host = host;
	this->auth = auth;
	Firebase.begin(host, auth);
	if (Firebase.failed() == true)
	{
		Serial.print("Setup Firebase Error:");
		Serial.println(Firebase.error());
		this->status = false;
	}
	else
	{
		this->status = true;
	}
}
MlabSwitch::MlabSwitch(const String host, const String auth)
{
	this->host = host;
	this->auth = auth;
	this->Init(host, auth);
}
int MlabSwitch::startSmartConfig(void)
{
	WiFi.mode(WIFI_STA);
	delay(500);
	WiFi.setAutoConnect(false);
	WiFi.beginSmartConfig();

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		Serial.println(WiFi.smartConfigDone());

		this->ssid = WiFi.SSID();
		this->psk = WiFi.psk();
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	String s;
	Serial.println(WiFi.hostname(s));
}
int MlabSwitch::initWifi(void)
{
	int res = 0;
	if (WiFi.SSID().length() == 0)
	{
		Serial.println("Start Smart Config because no saved ssid");
		WiFi.mode(WIFI_STA);
		delay(500);
		WiFi.setAutoConnect(false);
		WiFi.setAutoReconnect(false);
		WiFi.beginSmartConfig();
	}
	else
	{
		Serial.println("Connect Wifi base on saved ssid and psk");
 		WiFi.begin(WiFi.SSID().c_str(),WiFi.psk().c_str());
	}

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		count++;
		Serial.print(".");
		if (count >= 10)
		{
			Serial.println("Nomal Connect Timeout, switch to smartConfig");
			break;
		}
	}
	if (count >= 10)
	{
		Serial.println("Start Smart Config function");

		WiFi.mode(WIFI_STA);
		delay(500);
		WiFi.setAutoConnect(false);
		WiFi.beginSmartConfig();
		while (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
			Serial.print(".");
		}
		WiFi.smartConfigDone();
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		Serial.print("SSID:");
		Serial.println(WiFi.SSID());
		Serial.print("Psk:");
		Serial.println(WiFi.psk());
		res = 1;

	}
	else
	{
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		Serial.print("SSID:");
		Serial.println(WiFi.SSID());
		Serial.print("Psk:");
		Serial.println(WiFi.psk());
		res = 1;
	}

}
void MlabSwitch::initIO(uint8 relay_pin, uint8 status_pin, uint8 button_pin)
{
	this->relay_pin = relay_pin;
	this->status_pin = status_pin;
	this->button_pin = button_pin;
	pinMode(this->relay_pin, OUTPUT);
	pinMode(this->status_pin, OUTPUT);
	pinMode(this->button_pin, INPUT);
	this->relayOff();
	this->ledStatusOn();
	button_status = (bool)digitalRead(button_pin);
	this->firstTime = true;
	delay(500);


}
int MlabSwitch::initFirebase(String host, String auth)
{
	this->host = host;
	this->auth = auth;
	Firebase.begin(host, auth);
	if (Firebase.failed() == true)
	{
		Serial.print("Setup Firebase Error:");
		Serial.println(Firebase.error());
		this->status = false;
	}
	else
	{
		this->status = true;
		String s = "devices/";
		FirebaseObject obj = Firebase.get(s+this->chip_id);
		if (Firebase.success())
		{
			Serial.print("reading success, room name: ");
			String s = obj.getString("room_name");

			Serial.println(s);
		}
		else
		{
			Serial.println("reading fail");
		}
		Serial.println(Firebase.getString("devices"));
	}
	return this->status;

}
void MlabSwitch::Run()
{
	String s = "devices/";
	FirebaseObject obj = Firebase.get(s + this->chip_id);
	if (Firebase.success())
	{
		bool led1 = obj.getBool("status");
		if (firstTime == true)
		{
			firstTime = false;
			l1 = led1;
			digitalWrite(D7, !l1);
			digitalWrite(D1, l1);
			delay(100);
			Serial.print("Led1:");
			Serial.println(led1);
		}
		else
		{
			if (l1 != led1)
			{
				
				l1 = led1;
				digitalWrite(D7, !l1);
				digitalWrite(D1, l1);
				delay(100);
				Serial.print("Led1:");
				Serial.println(led1);
			}
		}

	}
	//this->getInput();
}
void MlabSwitch::relayOn(void)
{
	digitalWrite(relay_pin, HIGH);
}
void MlabSwitch::relayOff(void)
{
	digitalWrite(relay_pin, LOW);
}
void MlabSwitch::ledStatusOn(void)
{
	digitalWrite(status_pin, LOW);
}
void MlabSwitch::ledStatusOff(void)
{
	digitalWrite(status_pin, HIGH);
}
void MlabSwitch::getInput()
{
	bool x = (bool)digitalRead(D6);
	if (x != button_status)
	{
		button_status = x;
		Serial.print("Button: ");
		Serial.println(button_status);
		if (x == false)
		{
			digitalWrite(D7, LOW);
			digitalWrite(D1, HIGH);
		}
		else
		{
			digitalWrite(D7, HIGH);
			digitalWrite(D1, LOW);
		}
	}
}