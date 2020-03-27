#include "MitoSoft.h"

InvertableOutput::InvertableOutput(int pin, int mode) {
	this->_pin = pin;
	this->_mode = mode;

	pinMode(pin, OUTPUT);
	this->setOff();
}

void InvertableOutput::setOff() {
	if (_mode == INVERTED) {
		digitalWrite(_pin, HIGH);
	}
	else {
		digitalWrite(_pin, LOW);
	}
}

void InvertableOutput::setOn() {
	if (_mode == INVERTED) {
		digitalWrite(_pin, LOW);
	}
	else {
		digitalWrite(_pin, HIGH);
	}
}