
#include "MitoSoft.h"

//Beispiel: so werden statische Variablen erzeugt
//bool EthernetHelper::_writeLog;

EthernetHelper::EthernetHelper(byte mac[6], bool writeLog) {
	this->_mac = mac;
	this->_writeLog = writeLog;
}

void EthernetHelper::fixIpSetup(IPAddress ip) {
	writeSerial("Network try connecting via fix IP...");

	this->_mode = FIXIP;
	this->_ip = ip;

	disableSDCard();

	Ethernet.begin(_mac, ip);

	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		writeSerial("Ethernet shield was not found.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5100) {
		writeSerial("W5100 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5200) {
		writeSerial("W5200 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5500) {
		writeSerial("W5500 Ethernet controller detected.");
	}

	delay(2000);

	if (Ethernet.linkStatus() == Unknown) {
		writeSerial("Link status unknown. Link status detection is only available with W5200 and W5500.");
	}
	else if (Ethernet.linkStatus() == LinkON) {
		writeSerial("Link status: On");

		//_client->connect(_gateway, 80);
		String ip = String(Ethernet.localIP());
		writeSerial("Network connected and IP assigned: IP " + ip);
	}
	else if (Ethernet.linkStatus() == LinkOFF) {
		writeSerial("Link status: Off");
	}
}

void EthernetHelper::dhcpSetup() {
	writeSerial("Network try connecting via DHCP...");

	this->_mode = DHCP;

	disableSDCard();

	Ethernet.begin(_mac); // 5000, 4000);

	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		writeSerial("Ethernet shield was not found.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5100) {
		writeSerial("W5100 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5200) {
		writeSerial("W5200 Ethernet controller detected.");
	}
	else if (Ethernet.hardwareStatus() == EthernetW5500) {
		writeSerial("W5500 Ethernet controller detected.");
	}

	delay(2000);

	if (Ethernet.linkStatus() == Unknown) {
		writeSerial("Link status unknown. Link status detection is only available with W5200 and W5500.");
	}
	else if (Ethernet.linkStatus() == LinkON) {
		writeSerial("Link status: On");

		//_client->connect(_gateway, 80);
		String ip = String(Ethernet.localIP());
		writeSerial("Network connected and IP assigned: IP " + ip);
	}
	else if (Ethernet.linkStatus() == LinkOFF) {
		writeSerial("Link status: Off");
	}
}

//https://blog.startingelectronics.com/disabling-the-ethernet-chip-and-sd-card-on-an-arduino-ethernet-shield/
void EthernetHelper::disableSDCard() {
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);
}

void EthernetHelper::loop() {

	if (DHCP == _mode) {
		if (Ethernet.maintain() % 2 == 1) { //entweder 1 oder 3 (...failed) https://www.arduino.cc/en/Reference/EthernetMaintain
			// Cable disconnected or DHCP server hosed
			writeSerial("Network reconnecting..." + String(millis()));
			this->dhcpSetup();
		}
	}
}

void EthernetHelper::writeSerial(String text) {
	if (true == _writeLog) {
		Serial.println(text);
	}
};