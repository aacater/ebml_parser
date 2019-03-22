// ebml.h
// classes and functions for ebml stuff

#pragma once
#ifndef EBML_H_
#define EBML_H_

#include <iostream>
#include <iostream>
#include <fstream>
#include <array>

using std::string;
using std::array;

const int SPEC_LEN = 250; // number of types of ebml elements

enum ebml_element_type { // posible data types
	MASTER,	// other EBL sub-elemnts of next lower level
	UINT,	// unsigned integer, 1 to 8 bytes
	INT,	// signed integer, 1 to 8 bytes
	STRING,	// ASCII characters 0x20 to 0x7E
	UTF8,	// unicode, padded with zeros
	BINARY,	// not parsed
	FLOAT,	// big-endian, 4 and 8 bytes (32 or 64 bits)
	DATE	// signed 8 byte integer in nanoseconds
};

class elementData {
public:
	uint8_t width;
	uint8_t data[8];

	uint64_t get_uint();
};

class ebml_element { // ebml element defined using data from spec.cpp
public:
	string name;
	array<uint8_t, 4> id;
	enum ebml_element_type type;

	ebml_element() {};
	ebml_element(int) {};

	ebml_element(string name, array<uint8_t, 4>  const& id, enum ebml_element_type type)
		:name(name), id(id), type(type) {};
};

ebml_element get_element(array<uint8_t, 4> id, uint8_t level);

#endif