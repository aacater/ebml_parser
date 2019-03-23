// helper.h
// helper functions


#ifndef HELPER_H_
#define HELPER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "readFile.h"

uint64_t convHex(uint8_t* data, int length = 1);
uint64_t convHex(uint8_t temp, int length = 1);

#endif