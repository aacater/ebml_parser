#include "helper.h"

using std::string;

uint64_t convHex(uint8_t* data, int length)
{
	uint64_t value = 0;
	value = data[length - 1];
	for (int i = length - 1; i > 0; --i) {
		value += ((uint64_t)data[i - 1] << ((length - i) * 8));
	}
	return value;
}
uint64_t convHex(uint8_t temp, int length)
{
	uint8_t* ptemp;
	ptemp = &temp;
	uint64_t * data;
	memcpy(&data, &ptemp, sizeof data);
	uint64_t value = 0;
	value = data[length - 1];
	for (int i = length - 1; i > 0; --i) {
		value += ((uint64_t)data[i - 1] << ((length - i) * 8));
	}
	return value;
}
