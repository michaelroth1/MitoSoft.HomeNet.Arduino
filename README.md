# MitoSoft.HomeNet.Arduino

Lightweight project to use Arduino devices in a IoT environment for smart-homes.

## includes functionality:

- debounced GPIO input (usage as pullup as well as pulldown)
- inverted GPIO output (to use with standarsd optocoupler)
- blind and jalousie controller (refennrence run, actual position, run to a specific position, syncron run, blind positioning, ...)
- MQTT helpers (included stanard interfaces for different polpular Arduino MQTT-broker)
- Ethernet helper (standar interface with reconnetion pattern for Arduino EthernetShilds)

In smart homes its easy to implement:

- blind controller,
- light controller,
- alarm controller,
- control the main door, as well as
- controll the garage doors.

## Dependencies

The library has dependencies to the following packages:

- SPI.h
- Ethernet.h
- ArduinoMqttClient.h
- PubSubClient.h

## Licence

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Examples

Blind controlling sketch:

```c++
#include <MitoSoft.h>
#include <StringHelper.h>

DebouncingInput shutter1Pos(23, INPUT_PULLUP, 50);
DebouncingInput shutter1DownTaster(24, INPUT_PULLUP, 50);
DebouncingInput shutter1UpTaster(25, INPUT_PULLUP, 50);
ShutterController shutter1(26, 27, 20000, 2000);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	Serial.println("start GPIOUsing.ino");

	shutter1.referenceRun();
	shutter1.setShutterAndFinPosition(50.0, 0.0);
}

// the loop function runs over and over again until power down or reset
void loop() {

	if (shutter1DownTaster.risingEdge()) {
		shutter1.runDown();
	}
	else if (shutter1UpTaster.risingEdge()) {
		shutter1.runUp();
	}
	else if (shutter1Pos.risingEdge()) {
		String message = "60;50";
		double absPos = StringHelper().split(message, ';', 0).toDouble();
		double finPos = StringHelper().split(message, ';', 1).toDouble(); 
		shutter1.setPosition(absPos, finPos);
	}

	if (shutter1.started()) {
		Serial.println("Started Direction: " + shutter1.getDirectionAsText());
	}
	else if (shutter1.stopped()) {
		Serial.println("Stopped Pos: " + String(shutter1.getPosition()) + "; Fin-Pos: " + String(shutter1.getFinPosition()));
	}
	
	//looping
	shutter1.loop();
	delay(10);
}
```

Beispielsketch für die Verwendung mit MQTT (mit Paket <ArduinoMqttClient.h>):

```c++
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <MitoSoft.h>

// Netzwerk-Konfiguration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress broker(192, 168, 2, 100);

EthernetClient ethClient;
EthernetHelper ethHelper(mac, ethClient, broker);

PubSubClient client(broker, 1883, ethClient);
PubSubHelper mqttHelper(client, 15000, true);

unsigned long lastMillis = 0;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    Serial.println("start MqttUsing.ino");

    ethHelper.dhcpSetup();

	  delay(2000);
    
    mqttHelper.connect("Room1Controller", "Room1Controller");

    Serial.println("start loop");
}

// the loop function runs over and over again until power down or reset
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
```
