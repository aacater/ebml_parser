// readFile.h
// class to read binary data from file

#pragma once
#ifndef READFILE_H_
#define READFILE_H_

#include <iostream>
#include <fstream>
#include <string>

#include "helper.h"

#define BUFSIZE 104857

class readFile {
private:
	std::string fileName;
	std::ifstream file;
	uint8_t buffer[BUFSIZE];
	int positionFile; // can also use tellg() but wanted more control
	int positionBuffer;

public:
	readFile() {};
	readFile(std::string fileName);
	~readFile();
	uint8_t * readBits(int amount);
	uint8_t * readBits(int amount, int positonFile);
	uint8_t * getBuffer();
	int getPositionFile();
	void setPositionFile(int pos);
	int getPositionBuffer();
	void setPositionBuffer(int pos);
	int getFileSize();
	void clearBuffer();
};

#endif