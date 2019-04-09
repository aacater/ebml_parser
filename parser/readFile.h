// readFile.h
// class to read binary data from file

#pragma once
#ifndef READFILE_H_
#define READFILE_H_

#include <iostream>
#include <fstream>
#include <string>

#define BUFSIZE 104857

class readFile {
private:
	std::string fileName;		// file to open and read
	std::ifstream file;			// stream of file
	uint8_t buffer[BUFSIZE];	// buffer to store read data
	int positionBuffer;			// current position in buffer

public:
	readFile() {};
	readFile(std::string fileName);
	~readFile();
	uint8_t * readBits(int amount=1, int offset = 0);
	int getPositionFile();
	void setPositionFile(int pos);
	int getFileSize();
	void clearBuffer();
};

#endif