
#include "MitoSoft.h"

//Beispiel: so werden statische Variablen erzeugt
//bool EthernetHelper::_writeLog;

EthernetHelper::EthernetHelper(byte mac[6], EthernetClient& ethernetClient, IPAddress gateway, unsigned long reconnectionTime = 300000, bool writeLog = false) {
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

	Ethernet.begin(_mac, ip, _gateway);

	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		writeSerial("Network error: Ethernet shield was not found");
		return;
	}
	if (Ethernet.linkStatus() == LinkOFF) {
		writeSerial("Network error: Ethernet cable is not connected");
	}
	else {
		//_client->connect(_gateway, 80);
		String ip = String(Ethernet.localIP());
		writeSerial("Network connected and IP assigned: IP " + ip);
	}
}

void EthernetHelper::dhcpSetup() {
	writeSerial("Network try connecting via DHCP...");

	this->_mode = DHCP;

	if (Ethernet.begin(_mac, 5000, 4000) == 0) {
		writeSerial("Network error: Failed to configure Ethernet using DHCP");
		// Check for Ethernet hardware present
		if (Ethernet.hardwareStatus() == EthernetNoHardware) {
			writeSerial("Network error: Ethernet shield was not found");
			while (true) {
				delay(1); // do nothing, no point running without Ethernet hardware
			}
		}
		if (Ethernet.linkStatus() == LinkOFF) {
			writeSerial("Network error: Ethernet cable is not connected");
		}
	}
	else {
		//_client->connect(_gateway, 80);
		String ip = String(Ethernet.localIP());
		writeSerial("Network connected and DHCP assigned: IP " + ip);
	}
}

void EthernetHelper::loop() {

	if (millis() - _actualTime > _reconnectionTime) {
		if (!_client->connected()) {
			_actualTime = millis();
			writeSerial("Network reconnecting..." + String(millis()));
			if (FIXIP == _mode) {
				this->fixIpSetup(_ip);
			}
			else { //DHCP
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