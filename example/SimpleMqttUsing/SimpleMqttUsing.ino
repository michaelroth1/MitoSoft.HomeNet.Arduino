/*
 Name:		SimpleMqttUsing.ino
 Created:	1/21/2020 8:49:25 PM
 Author:	M. Roth
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
#include <MitoSoft.h>

// network configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress broker(192, 168, 2, 100);

EthernetClient ethClient;
EthernetHelper ethHelper(mac, ethClient, broker);

PubSubClient mqttClient(broker, 1883, ethClient);
PubSubHelper mqttHelper(mqttClient);

unsigned long lastMillis = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("start SimpleMqttUsing.ino");

    ethHelper.dhcpSetup();

    mqttHelper.connect("Room1Controller", "Room1Controller");

    Serial.println("start loop");
}

void loop() {

    String t = mqttHelper.getSubtopic();
    String m = mqttHelper.getMessage();

    if (m != "") {
        Serial.println("Message received in MAIN LOOP: " + m);
    }

    // publish a message roughly every second.
    if (millis() - lastMillis > 1000) {
        lastMillis = millis();
        mqttHelper.publish("Output2", "High");
    }

    mqttHelper.loop();

    delay(50);
}