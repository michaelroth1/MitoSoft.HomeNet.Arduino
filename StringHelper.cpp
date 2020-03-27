#include "StringHelper.h"
#include "string.h"

//https://stackoverflow.com/questions/9072320/split-string-into-string-array
String StringHelper::split(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}

	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
};

bool StringHelper::contains(String s, String search) {
	int max = s.length() - search.length();
	int lgsearch = search.length();

	for (int i = 0; i <= max; i++) {
		if (s.substring(i, i + lgsearch) == search) {
			return true;
		}
	}

	return false;
}