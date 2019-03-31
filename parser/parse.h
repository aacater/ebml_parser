// parse.h
// class for parsing an ebml element from a file

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include  <iomanip>

#include "readFile.h"
#include "ebml.h"

class parse {
private:
	readFile file; // file to parse from
	string fileName; // name of file to parse from

	uint8_t idWidth; // specifies ID length
	uint8_t sizeWidth; // specifies size length
	uint8_t * id; // id can be maximum length of 4 bytes 
	uint8_t * size; // size can be maximum length of 8 bytes

	std::string name; // ebml element name
	enum ebml_element_type type; // ebml element data type

public:
	parse(string fileName, int positionFile = 0) : file(fileName), idWidth(1), sizeWidth(1) { file.clearBuffer(); file.setPositionFile(positionFile); };
	uint8_t * parseEleSizeorID(bool size = false);
	uint8_t * getID();
	uint8_t getIDWidth();

	uint8_t * getSize();
	uint8_t getSizeWidth();

	string getName();
	enum ebml_element_type getType();

	void parseElement();
	bool lookupElement(uint8_t* const &id, const int& width, std::string &name, ebml_element_type &type);
	int getPositionFile();

	void getData(std::ostream &os);
	void print(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, parse &p);

#endif