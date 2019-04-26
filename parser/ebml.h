// ebml.h
// classes and functions for ebml specific stuff

#pragma once
#ifndef EBML_H_
#define EBML_H_

#include <iostream>
#include <array>
#include <new>

using std::string;
using std::array;

const int SPEC_LEN = 250; // number of types of ebml elements

enum ebml_element_type { // posible data types
	_MASTER,	// other EBL sub-elemnts of next lower level
	_UINT,	// unsigned integer, 1 to 8 bytes
	_INT,	// signed integer, 1 to 8 bytes
	_STRING,	// ASCII characters 0x20 to 0x7E
	_UTF8,	// unicode, padded with zeros
	_BINARY,	// not parsed
	_FLOAT,	// big-endian, 4 and 8 bytes (32 or 64 bits)
	_DATE	// signed 8 byte integer in nanoseconds
};

// class containing info about an EMBL element's Name, ID, and Type
class ebml_element {
public:
	string name;
	array<uint8_t, 4> id;
	enum ebml_element_type type;

	ebml_element(string name, array<uint8_t, 4> const& id, enum ebml_element_type type)
		:name(name), id(id), type(type) {}
};

string getebmlTypeName(enum ebml_element_type type);
extern const ebml_element * ebml_spec[SPEC_LEN];

#endif