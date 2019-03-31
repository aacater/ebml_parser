// readFile.cpp
// function definitions for readFile class

#include "readFile.h"
using std::string;

// open fstream based on file name
readFile::readFile(string fileName)
{
	file.open(fileName, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		perror("ERROR: readFile::readFile: Cannot open file");
		system("pause");
		exit(1);
	}
}

// deconstructor to close file
readFile::~readFile()
{
	file.close();
}

// read data from file of specific amount of bytes
// offset is added to current filePosition
// mainly to have the first id byte in returned data without having to reread it
uint8_t * readFile::readBits(int amount, int offset)
{
	if (getPositionFile() < 0)
	{
		perror("ERROR: readFile::readBits: Invalid file position");
		system("pause");
		exit(1);
	}
	int bufStart = positionBuffer + offset;
	amount += offset;
	file.read(reinterpret_cast<char*>(&buffer[positionBuffer]), amount);
	positionBuffer += (amount);
	return &buffer[bufStart];
}

// returns current position (in bytes) of file
int readFile::getPositionFile()
{
	return file.tellg();
}

// sets current position (in bytes) of file
void readFile::setPositionFile(int pos)
{
	file.seekg(pos);
}

// returns size of file (in bytes)
int readFile::getFileSize()
{
	file.seekg(0, file.end);
	return file.tellg();
}

// zero out buffer
// prevents acidently reading old data
// maybe should change to just creating new buffer object?
// that might create bad memory access though
void readFile::clearBuffer()
{
	positionBuffer = 0;
	for (int i = 0; i < BUFSIZE; i++)
	{
		buffer[i] = 0;
	}

}
