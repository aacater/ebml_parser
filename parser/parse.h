// parse.h
// classes and functions for ebml stuff


#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

#include "readFile.h"
#include "ebml.h"

class parse {
private:
	readFile file;
	string fileName;

	uint8_t idWidth;
	uint8_t sizeWidth;
	uint8_t * id; // id can be maximum of 4 bytes 
	uint8_t * size; // size can be maximum of 8 bytes

	std::string name;
	enum ebml_element_type type;

public:
	parse(string fileName, int positionFile = 0) : file(fileName), idWidth(1), sizeWidth(1) { file.setPositionBuffer(0); file.clearBuffer(); file.setPositionFile(positionFile); };
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
};

std::ostream& operator<<(std::ostream &os, parse &p);

#endif