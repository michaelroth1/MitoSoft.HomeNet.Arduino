#include "MitoSoft.h"

byte DebouncingInput::getByteState() {
	byte newReading = digitalRead(_pin);
	if (INPUT_PULLUP == _pinMode) { //Invertieren
		newReading = !newReading;
	}
	if (newReading != _lastReading) {
		_lastDebounceTime = millis();
	}
	if (millis() - _lastDebounceTime > _debounceTime) {
		// Update the 'state' attribute only if debounce is checked
		_state = newReading;
	}
	_lastReading = newReading;
	return _state;
}

DebouncingInput::DebouncingInput(byte pin, int mode, unsigned long debounceTime) {
	this->_pin = pin;
	this->_pinMode = mode;
	_lastReading = LOW;
	this->_debounceTime = debounceTime;

	pinMode(_pin, _pinMode);
}

bool DebouncingInput::getState() {
	if (HIGH == getByteState()) {
		return true;
	}
	else {
		return false;
	}
}

bool DebouncingInput::risingEdge() {
	bool ret = false;
	bool state = getState();
	if (state != _lastPosState) {
		if (state == true) { // positive Flanke
			ret = true;
		}
	}

	_lastPosState = state;    // save the current state as the last state, for next time through the loop
	return ret;
}

bool DebouncingInput::fallingEdge() {
	bool ret = false;
	bool state = getState();
	if (state != _lastNegState) {
		if (state == false) { // negative Flanke
			ret = true;
		}
	}

	_lastNegState = state;    // save the current state as the last state, for next time through the loop
	return ret;
};
