// parse.h
// class for parsing an ebml element from a file

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <memory>
#include <stdlib.h> 
#include <windows.h>

#include "ebml.h"

class parse {
private:
	std::shared_ptr<uint8_t> buffer;	// pointer to buffer to store data
	int fileSize;						 // size of file to parse
	int positionBuffer;					// current position in buffer

	uint8_t idWidth;					// specifies ID length
	uint8_t sizeWidth;					// specifies size length
	uint8_t* id;						// id can be maximum length of 4 bytes 
	uint8_t* size;						// size can be maximum length of 8 bytes

	std::string name;					// ebml element name
	enum ebml_element_type type;		// ebml element data type

public:
	parse(const std::string& fileName);
	~parse();

	uint8_t* parseEleSizeorID(bool size = false);
	void parseElement();
	bool lookupElement(uint8_t* const &id, const int& width, std::string &name, ebml_element_type &type);

	int getPosition();
	int getSize();

	void getData(std::ostream &os);
	void print(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, parse &p);
uint64_t getuint64(uint8_t * data, int length = 1);
#endif