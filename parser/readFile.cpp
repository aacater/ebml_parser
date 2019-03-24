// readFile.cpp
// function definitions for readFile class

#include "readFile.h"
using std::string;

// open fstream based on file name
readFile::readFile(string fileName)
{
	//std::cout << "Opening file..." << std::endl;
	file.open(fileName, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		std::cout << "ERROR: Can not open file: " << fileName << std::endl;
		std::cout << "Exiting..." << std::endl;
		system("pause");
		exit(-1);
	}
	//std::cout << "Opened file successfully." << std::endl;
}

// deconstructor to close file
readFile::~readFile()
{
	//std::cout << "Closing file..." << std::endl;
	file.close();
}

// read data from file of specific amount of bytes
// should not be greater than BUFSIZE
uint8_t * readFile::readBits(int amount)
{
	file.seekg(positionFile);
	int bufStart = positionBuffer;
	file.read(reinterpret_cast<char*>(&buffer[positionBuffer]), amount);
	positionBuffer += amount;
	positionFile += amount;

	return &buffer[bufStart];
}

// read data from file of specific amount of bytes
// starting at specific position in file
uint8_t * readFile::readBits(int amount, int posFile)
{
	int bufStart = positionBuffer;
	//std::cout << "Reading File: " << amount << " bytes with an offset of " << 0 << "from pos " << posFile << std::endl;

	file.seekg(posFile);
	file.read(reinterpret_cast<char*>(&buffer[positionBuffer]), amount);
	
	positionBuffer += amount;
	positionFile = posFile + amount;

	return &buffer[bufStart];
}

// returns pointer to buffer
uint8_t * readFile::getBuffer()
{
	return (uint8_t *)buffer;
}

// returns current position (in bytes) of file
int readFile::getPositionFile()
{
	return positionFile;
}

// sets current position (in bytes) of file
void readFile::setPositionFile(int pos)
{
	positionFile = pos;
}

// returns current position (in bytes) in buffer
int readFile::getPositionBuffer()
{
	return positionBuffer;
}

// sets current position (in bytes) of file
void readFile::setPositionBuffer(int pos)
{
	positionBuffer = pos;
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
void readFile::clearBuffer()
{
	for (int i = 0; i < BUFSIZE; i++)
	{
		buffer[i] = 0;
	}
}
