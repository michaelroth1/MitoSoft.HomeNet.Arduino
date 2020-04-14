
#ifndef MitoSoft_h
#define MitoSoft_h

#include <SPI.h>
#include <Ethernet.h>
#include "Arduino.h"
#include <PubSubClient.h>
#include "StringHelper.h"
#include <ArduinoMqttClient.h>

const int STANDARD = 1;
const int INVERTED = 2;

// Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
// You need to set the IP address directly.
class EthernetHelper {

private:

	const int DHCP = 1;
	const int FIXIP = 2;

	int _mode = FIXIP;

	byte* _mac;
	IPAddress _ip;

	bool _writeLog;

	void writeSerial(String text);

	void disableSDCard();

public:

	EthernetHelper(byte mac[6], bool writeLog = false);

	void fixIpSetup(IPAddress ip);

	void dhcpSetup();

	void loop();
};

class InvertableOutput {

private:

	int _mode = 0;
	int _pin = 0;

public:

	InvertableOutput(int pin, int mode);

	void setOn();

	void setOff();
};

class DebouncingInput {

private:

	byte _pin;
	byte _state;
	byte _lastReading;
	bool _lastNegState;
	bool _lastPosState;
	int _pinMode;
	unsigned long _lastDebounceTime = 0;
	unsigned long _debounceTime = 10;

	byte getByteState();

public:

	DebouncingInput(byte pin, int mode = INPUT_PULLUP, unsigned long debounceTime = 50);

	bool getState();

	bool risingEdge();

	bool fallingEdge();
};

class DigitalOutput {

private:

	int _state = 0;
	int _pin = 0;
	InvertableOutput* _invertableOutput = nullptr;

public:

	DigitalOutput(int pin, int mode = STANDARD);

	int toggle();

	int setOn();

	int setOff();

	void setState(int mode);

	int getState();
};

class DigitalInput {

private:

	bool _lastNegState = false;
	bool _lastPosState = false;
	int _pin = 0;
	int _pinMode = INPUT_PULLUP;

public:

	DigitalInput(int pin, int mode = INPUT_PULLUP);

	bool getState();

	bool risingEdge();

	bool fallingEdge();
};

class ShutterController {

private:

	const int DOWN = 1;
	const int UP = 2;

	int _reverseTime;
	double _upReferencePos;
	double _downReferencePos;
	double _runToPosAfterRefRunSingle = -1.0;
	double _runToPosAfterRefRunDouble = -1.0;
	double _finPosAfterRun = -1.0;
	double _finPosAfterRefRun = -1.0;

	int _upPin = 0;
	int _downPin = 0;
	InvertableOutput* _up;
	InvertableOutput* _down;

	bool _isRunning = false;
	bool _isReferenceRun = false;
	bool _openFins = false;
	bool _hasStopped = 0; //Event 'stopped' 
	bool _hasStarted = 0; //Event 'started' 

	unsigned long _lastReferenceRun = 0;
	unsigned long _shutterRunTime = 0;
	unsigned long _finOpenTime = 0;
	unsigned long _startWaitTime = 0;
	unsigned long _actualTime = 0;

	double _shutterPos = 0.0;
	double _finPos = 0.0;
	double _requestedPos = 0.0;
	int _requestedDir = 0;
	int _lastRunDirection = 0;
	int _runDir = 0;

	double CalculatePositionFromRuntime(double lastPosition, int sutterDirection, unsigned long runTime, unsigned long maxRunTime);

	double CalculateShutterCorrection(double finStartPos, double finEndPos);

	void SetVariablesToInitValue();

public:

	ShutterController(int upPin, int downPin, unsigned long maxRunTime, unsigned long finOpenTime = 0, int mode = STANDARD, double upReferencePos = -2.0, double downReferencePos = 102.0, int reverseTime = 500);

	void referenceRun();

	void runDown();

	void runUp();

	void setPosition(double newPosition, double newFinPosition);

	void setFinPosition(double newFinPosition);

	void setShutterPosition(double newPosition);

	void setShutterAndFinPosition(double newPosition, double newFinPosition);

	void runStop();

	double getFinPosition();

	double getPosition();

	int getDirection();

	String getDirectionAsText();

	bool stopped();

	bool started();

	bool isReferenceRun();

	unsigned long lastReferenceRun();

	bool isRunning();

	void loop();
};

//https://github.com/arduino-libraries/ArduinoMqttClient
class MqttHelper {

private:

	MqttClient* _mqttClient = nullptr;

	bool _writeLog;

	String _clientId;
	IPAddress _brokerIP;
	String _topicPrefix;
	String _topic;
	String _message;

	unsigned long _pollingInterval = 10000;
	unsigned long _actualTime = 0;
	unsigned long _reconnectionTime = 3000;

	void writeSerial(String text);

	void subscribe(String topic);

public:

	MqttHelper(MqttClient& client, unsigned long reconnectionTime = 5000, bool writeLog = false);

	bool connect(IPAddress broker, String clientId, String topicPrefix);

	String getTopic();

	String getSubtopic();

	String getMessage();

	void publish(String subtopic, String message, bool retain = false);

	bool loop();
};

//https://github.com/knolleary/pubsubclient
class PubSubHelper
{
private:

	static EthernetClient _ethClient;

	static PubSubClient _mqttClient;

	static bool _writeLog;
	static String _topic;
	static String _message;

	char* _clientId;

	String _topicPrefix;

	IPAddress _broker;
	uint16_t _port = 1883;

	unsigned long _actualTime = 0;
	unsigned long _reconnectionTime = 15000;

	static void writeSerial(String text);

	void subscribe(String topic);

	static void onMessageReceived(char* topic, byte* payload, unsigned int length);

public:

	bool connect(char* clientId, String topicPrefix);

	PubSubHelper(IPAddress broker, uint16_t port, unsigned long reconnectionTime = 5000, bool writeLog = false);

	String getTopic();

	String getSubtopic();

	String getMessage();

	void publish(String subtopic, String message, bool retain = false);

	bool loop();
};

#endif