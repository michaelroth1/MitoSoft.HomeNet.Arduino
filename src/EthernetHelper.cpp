
#include "MitoSoft.h"

//Beispiel: so werden statische Variablen erzeugt
//bool EthernetHelper::_writeLog;

EthernetHelper::EthernetHelper(byte mac[6], EthernetClient& ethernetClient, IPAddress gateway, unsigned long reconnectionTime, bool writeLog) {
	this->_mac = mac;
	this->_writeLog = writeLog;
	this->_client = &ethernetClient;
	this->_reconnectionTime = reconnectionTime;
	this->_gateway = gateway;
	this->_actualTime = millis();
}

void EthernetHelper::fixIpSetup(IPAddress ip) {
	writeSerial("Network try connecting via fix IP...");

	this->_mode = FIXIP;
	this->_ip = ip;

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

void EthernetHelper::loop() {

	if (DHCP == _mode) {
		if (Ethernet.maintain() % 2 == 1) { //entweder 1 oder 3 (...failed) https://www.arduino.cc/en/Reference/EthernetMaintain
			// Cable disconnected or DHCP server hosed
			writeSerial("Network reconnecting..." + String(millis()));
			this->dhcpSetup();
		}
	}

	if (millis() - _actualTime > _reconnectionTime) {
		if (!_client->connected()) {
			_actualTime = millis();
			writeSerial("Network reconnecting..." + String(millis()));
			if (FIXIP == _mode) {
				this->fixIpSetup(_ip);
			}
			else { //DHCP
				Ethernet.maintain();
				this->dhcpSetup();
			}
		}
	}
}

void EthernetHelper::writeSerial(String text) {
	if (true == _writeLog) {
		Serial.println(text);
	}
};