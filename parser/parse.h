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

#include "readFile.h"
#include "ebml.h"

class parse {
private:
	readFile file;					 // file to parse from
	string log;

	uint8_t idWidth;				// specifies ID length
	uint8_t sizeWidth;				// specifies size length
	uint8_t * id;					// id can be maximum length of 4 bytes 
	uint8_t * size;					// size can be maximum length of 8 bytes

	std::string name;				// ebml element name
	enum ebml_element_type type;	// ebml element data type

public:
	parse(string fileName, int positionFile = 0) : file(fileName), idWidth(1), sizeWidth(1) { file.clearBuffer(); file.setPositionFile(positionFile); };

	uint8_t * parseEleSizeorID(bool size = false);
	void parseElement();
	bool lookupElement(uint8_t* const &id, const int& width, std::string &name, ebml_element_type &type);

	int getPositionFile();

	void getData(std::ostream &os);
	void print(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, parse &p);
uint64_t getuint64(uint8_t* data, int length = 1);

#endif