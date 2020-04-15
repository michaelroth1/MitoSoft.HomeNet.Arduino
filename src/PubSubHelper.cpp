#include "MitoSoft.h"
#include <Arduino.h>
#include <string.h>
#include <PubSubClient.h>

//https://github.com/knolleary/pubsubclient

bool PubSubHelper::_writeLog;

String PubSubHelper::_topic;

String PubSubHelper::_message;

void PubSubHelper::onMessageReceived(char* topic, byte* payload, unsigned int length)
{
	_topic = (String)topic;

	String message = "";
	for (int i = 0; i < length; i++) {
		message = message + (char)payload[i];
	}
	_message = message;

	writeSerial("Message received - topic: " + _topic + "; message: " + _message);
}

void PubSubHelper::writeSerial(String text) {
	if (true == _writeLog) {
		Serial.println(text);
	}
}

PubSubHelper::PubSubHelper(PubSubClient& client, unsigned long reconnectionTime, bool writeLog)
{
	this->_mqttClient = &client;
	this->_writeLog = writeLog;
	this->_reconnectionTime = reconnectionTime;
	this->_actualTime = millis();
}

bool PubSubHelper::connect(char* clientId, String topicPrefix) {

	this->_topicPrefix = topicPrefix;
	this->_clientId = clientId;

	if (!_mqttClient->connect(clientId)) {
		writeSerial("MQTT connection failed: Error code " + String(_mqttClient->state()));

		return false;
	}
	else {
		writeSerial("MQTT connected to broker: " + (String)_clientId);

		_mqttClient->setCallback(*onMessageReceived); //eventuell die setCallback Funktion in Konstruktor verlagern

		String topic = topicPrefix + "/In/#";

		this->subscribe(topic);

		return true;
	}
}

String PubSubHelper::getTopic()
{
	String topic = _topic;
	_topic = "";
	return topic;
}

String PubSubHelper::getSubtopic()
{
	String topic = this->getTopic();
	String subtopic = topic;
	subtopic.remove(0, (_topicPrefix + "/In/").length());
	return subtopic;
}

String PubSubHelper::getMessage()
{
	String message = _message;
	_message = "";
	return message;
}

//Es wird nur QOS Level 0 und 1 unterstützt (mit 2 funktioniert es erst gat nicht!)
//QOS0 => Nachrichten maximal einmal liefern (eine Nachricht wird nur einmal geschickt ohne Rücksicht auf Verluste)
//QOS1 => Nachrichten werden mindestens einmal abgeliefert (könnte aber auch mehrere Male abgeliefert werden!!!)
void PubSubHelper::subscribe(String topic)
{
	char t[topic.length() + 1];
	topic.toCharArray(t, topic.length() + 1);

	_mqttClient->subscribe(t, 0);

	writeSerial("MQTT subscription: Topic " + (String)topic);
}

//Es wird nur QOS Level 0 unterstützt
//QOS0 => Nachrichten maximal einmal liefern (Fire and forget)
void PubSubHelper::publish(String subtopic, String message, bool retain = false)
{
	if (_mqttClient->connected()) {

		String topic = _topicPrefix + "/Out/" + subtopic;

		char t[topic.length() + 1];
		topic.toCharArray(t, topic.length() + 1);

		char m[message.length() + 1];
		message.toCharArray(m, message.length() + 1);

		_mqttClient->publish(t, m, retain);

		writeSerial("MQTT message published: Topic " + (String)subtopic + "; Message " + (String)message);
	}
}

//nonblocking reconnect
//https://github.com/knolleary/pubsubclient/blob/v2.7/examples/mqtt_reconnect_nonblocking/mqtt_reconnect_nonblocking.ino
bool PubSubHelper::loop() {
	if (!_mqttClient->connected()) {
		if (millis() - _actualTime > _reconnectionTime) {
			_actualTime = millis();
			writeSerial("MQTT reconnecting..." + String(millis()));
			bool connected = this->connect(_clientId, _topicPrefix);
			if (!connected) {
				return false;
			}
		}
	}
	else {
		_mqttClient->loop(); // Client connected
	}

	return true;
};