#include "MitoSoft.h"

DigitalInput::DigitalInput(int pin, int mode) {
	this->_pin = pin; // wir merken uns die Pins für die spätere Verwendung
	this->_pinMode = mode;

	pinMode(_pin, _pinMode);
}

bool DigitalInput::getState() {
	bool ret = false;
	int value = digitalRead(_pin);
	if (INPUT_PULLUP == _pinMode && LOW == value) {
		ret = true;
	}
	else if (INPUT == _pinMode && HIGH == value) {
		ret = true;
	}

	return ret;
};

bool DigitalInput::risingEdge() {
	bool ret = false;
	bool state = getState();

	if (state != _lastPosState) {
		// if the state has changed, increment the counter
		if (state == true) {
			// positive Flanke
			ret = true;
		}
	}

	_lastPosState = state;    // save the current state as the last state, for next time through the loop
	return ret;
}

bool DigitalInput::fallingEdge() {
	bool ret = false;
	bool state = getState();

	if (state != _lastNegState) {
		// if the state has changed, increment the counter
		if (state == false) {
			// negative Flanke
			ret = true;
		}
	}

	_lastNegState = state;    // save the current state as the last state, for next time through the loop
	return ret;
}