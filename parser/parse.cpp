// parse.cpp
// function definitions for parse class

#include "parse.h"

using std::string;
using std::ostream;

// read the first byte and find the length of data
// class specifies the length of the data piece 
// then read data based on length
// returns pointer to data
uint8_t * parse::parseEleSizeorID(bool size)
{
	int mask = 0x80;
	int width = 1;
	uint8_t* firstByte = file.readBits(1); // read first byte
	//std::cout << "First Byte: 0x" << std::hex << getuint64(firstByte, 1) << std::endl;
	while (!(firstByte[0] & mask)) 
	{ // find first zero bit, this shows width
		mask >>= 1;
		width++;
	}
	uint8_t * readData = file.readBits(width, file.getPositionFile()-1);
	if (size)
	{ // if size == true
		// then have to remove first zero
		// if it stays there then the size would always be very large (or negative if in most significant bit)
		sizeWidth = width;
		readData[0] = readData[0] ^ mask;
	}
	else
	{
		idWidth = width;
	}
		
	return readData;
}

// returns pointer to id
uint8_t * parse::getID()
{
	return id;
}

// returns uint8_t of width of ID
// can be between 1 and 4
uint8_t parse::getIDWidth()
{
	return idWidth;
}

// returns pointer to size
uint8_t * parse::getSize()
{
	return size;
}

// returns uint8_t of width of Size
// can be between 1 and 8
uint8_t parse::getSizeWidth()
{
	return sizeWidth;
}

// returns name of EBML element
string parse::getName()
{
	return name;
}

// turns type of type of EBML element
ebml_element_type parse::getType()
{
	return type;
}

// parses the id then size of element
void parse::parseElement()
{
	id = parseEleSizeorID(false);
	size = parseEleSizeorID(true);
	if (!lookupElement(id, idWidth, name, type))
	{
		name = "UNKNOWN";
	}

}

// uses the id to lookup the Element Name and Type from list
bool parse::lookupElement(uint8_t * const& id, const int& width, std::string& name, ebml_element_type& type)
{
	bool found = false;
	for (int i = 0; i < SPEC_LEN; i++)
	{ // loop through each ebml element
		for (int j = 0; j < width; j++)
		{ // check each byte of ID
			if (ebml_spec[i]->id[j] != id[j])
				break;
			if(width - 1 == j)
				found = true;
		} // if found stop looking
		if (found)
		{
			type = ebml_spec[i]->type;
			name = ebml_spec[i]->name;
			return found;
		}
	}
	return false;
}

// returns current position (in bytes) of file
int parse::getPositionFile()
{
	return file.getPositionFile();
}

// prints ebml element data based on element type to ostream 
void parse::getData(std::ostream & os)
{
	switch (type)
	{
	case MASTER:
	{
		os << "N/A";
		break;
	}
	case UTF8:
	case STRING:
	{
		uint8_t * data = file.readBits(getuint64(size, sizeWidth));
		os << std::hex << data;
		break;
	}
	case UINT:
	{
		uint8_t * data = file.readBits(getuint64(size, sizeWidth));
		os << getuint64(data, sizeWidth);
		break;
	}
	case BINARY:
	{
		uint8_t * data = file.readBits(getuint64(size, sizeWidth));
		//os << getuint64(data, sizeWidth);
	}
	case FLOAT:
	{ // can be 4 or 8 bytes long
		if (sizeWidth != 4 && sizeWidth != 8)
			std::cout << "Bad float width.";
		uint8_t * data = file.readBits(getuint64(size, sizeWidth));
		int64_t val = int64_t(getuint64(data, sizeWidth));
		float float_val;
		memcpy(&float_val, &val, sizeWidth);
		std::cout << std::fixed << std::dec << float_val;
	}
	case DATE:
	{ // 8 byte integer in nanosecods
		// 0 indicating the precise beginning of the millennium
		uint8_t * data = file.readBits(getuint64(size, sizeWidth));
	}
	default:
		file.readBits(getuint64(size, sizeWidth));
		break;
	}
}

// overloads parse class object to << operator.
// used to print data to ostream
ostream& operator<<(ostream &os, parse &p)
{
	p.getData(os);
	return os;
}