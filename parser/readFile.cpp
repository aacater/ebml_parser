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

// read data from file
// amount : how many bytes to read
// can not be greater than BUFSIZE
// overloaded to read from different positions in file and to different positions in the buffer
uint8_t * readFile::readBits(int amount)
{
	int bufStart = positionBuffer;
	file.read(reinterpret_cast<char*>(&buffer[positionBuffer]), amount);
	positionBuffer += amount;
	positionFile += amount;

	return &buffer[bufStart];
	//return getData(save);
}

uint8_t * readFile::readBits(int amount, int posFile)
{
	int bufStart = positionBuffer;
	//std::cout << "Reading File: " << amount << " bytes with an offset of " << 0 << "from pos " << posFile << std::endl;

	file.seekg(posFile);
	file.read(reinterpret_cast<char*>(&buffer[positionBuffer]), amount);
	
	positionBuffer += amount;
	positionFile = posFile + amount;

	return &buffer[bufStart];
	//return getData(save);
}

// returns pointer to buffer
uint8_t * readFile::getBuffer()
{
	return (uint8_t *)buffer;
}

// returns read data
uint8_t * readFile::getData(int amount)
{
	positionBuffer -= amount;
	uint8_t* data = (uint8_t*)malloc(amount);
	for (int i = 0; i < amount; i++)
	{
		data[i] = buffer[positionBuffer];
		positionBuffer++;
		std::cout << positionBuffer << std::endl;
	}
	return data;
}

int readFile::getPositionFile()
{
	return positionFile;
}
void readFile::setPositionFile(int pos)
{
	positionFile = pos;
}
int readFile::getPositionBuffer()
{
	return positionBuffer;
}

void readFile::setPositionBuffer(int pos)
{
	positionBuffer = pos;
}

int readFile::getFileSize()
{
	file.seekg(0, file.end);
	return file.tellg();
}

void readFile::clearBuffer()
{
	for (int i = 0; i < BUFSIZE; i++)
	{
		buffer[i] = 0;
	}
}
