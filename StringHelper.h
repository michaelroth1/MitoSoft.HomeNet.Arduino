#include "Arduino.h"

#ifndef MitoSoftStringHelper_h
#define MitoSoftStringHelper_h

class StringHelper {

public:

	//https://stackoverflow.com/questions/9072320/split-string-into-string-array
	static String split(String data, char separator, int index);

	//https://stackoverflow.com/questions/5029612/how-to-match-text-in-string-in-arduino?rq=1
	static bool contains(String s, String search);
};

#endif