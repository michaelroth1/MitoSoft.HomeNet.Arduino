#include "MitoSoft.h"

//https://github.com/arduino-libraries/ArduinoMqttClient

MqttHelper::MqttHelper(MqttClient& client, unsigned long reconnectionTime, bool writeLog) {
	this->_mqttClient = &client;
	this->_writeLog = writeLog;
	this->_pollingInterval = 1000;
	this->_reconnectionTime = reconnectionTime;
	this->_actualTime = millis();
}

bool MqttHelper::connect(IPAddress broker, String clientId, String topicPrefix) {
	this->_topicPrefix = topicPrefix;
	this->_clientId = clientId;
	this->_mqttClient->setId(clientId);
	this->_brokerIP = broker;

	if (!_mqttClient->connect(broker)) {
		writeSerial("MQTT connection failed: Error code " + String(_mqttClient->connectError()));

		return false;
	}
	else {
		writeSerial("MQTT connected to broker: " + clientId);

		this->subscribe(topicPrefix + "/In/#");

		return true;
	}
}

void MqttHelper::subscribe(String topic) {
	_mqttClient->subscribe(topic, 2); //TODO war ohne qos Level
	writeSerial("MQTT subscription: Topic " + topic);
}

bool MqttHelper::loop() {

	if (millis() - _actualTime > _reconnectionTime) {
		if (!_mqttClient->connected()) {
			_actualTime = millis();
			writeSerial("MQTT reconnecting..." + String(millis()));
			bool connected = this->connect(_brokerIP, _clientId, _topicPrefix);
			if (false == connected) {
				return false;
			}
		}
	}

	if (!_mqttClient->connected()) {
		return false;
	}

	int messageSize = _mqttClient->parseMessage();
	if (messageSize) {
		_topic = _mqttClient->messageTopic();
		if (_mqttClient->available()) {
			_message = _mqttClient->readString();
		}
		else {
			_message = "";
		}
		writeSerial("MQTT message received: Topic " + _topic + "; Message " + _message);
		return true;
	}

	return false;
}

String MqttHelper::getTopic() {
	String topic = _topic;
	_topic = "";
	return topic;
}

String MqttHelper::getSubtopic() {
	String topic = this->getTopic();
	String subtopic = topic;
	subtopic.remove(0, (_topicPrefix + "/In/").length());
	return subtopic;
}

String MqttHelper::getMessage() {
	String message = _message;
	_message = "";
	return message;
}

void MqttHelper::publish(String subtopic, String message, bool retain) {
	if (_mqttClient->connected()) {
		String topic = _topicPrefix + "/Out/" + subtopic;
		_mqttClient->beginMessage(topic, retain, 2); //TODO war ma qos Level 2
		_mqttClient->print(message);
		_mqttClient->endMessage();
		writeSerial("MQTT message published: Topic " + topic + "; Message " + message);
	}
}

void MqttHelper::writeSerial(String text) {
	if (true == _writeLog) {
		Serial.println(text);
	}
};