// helper.cpp
// function definitions for functions in helper.h

#include "helper.h"

using std::string;

// returns a uint64_t made from data of variable length starting at passed pointer
// converts from big endian to little endian 
uint64_t getuint64(uint8_t* data, int length)
{
	uint64_t value = 0;
	value = data[length - 1];
	for (int i = length - 1; i > 0; --i) {
		value += ((uint64_t)data[i - 1] << ((length - i) * 8));
	}
	return value;
}
