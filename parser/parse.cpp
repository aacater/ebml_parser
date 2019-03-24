#include "parse.h"

using std::string;
using std::ostream;

// read the first byte and find the class.
// class specifies the length of the data piece 
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
	{
		sizeWidth = width;
		readData[0] = readData[0] ^ mask;
	}
	else
	{
		idWidth = width;
	}
		
	return readData;
}

uint8_t * parse::getID()
{
	return id;
}

uint8_t parse::getIDWidth()
{
	return idWidth;
}

uint8_t * parse::getSize()
{
	return size;
}

uint8_t parse::getSizeWidth()
{
	return sizeWidth;
}

string parse::getName()
{
	return name;
}

ebml_element_type parse::getType()
{
	return type;
}

void parse::parseElement()
{
	id = parseEleSizeorID(false);
	size = parseEleSizeorID(true);
	if (!lookupElement(id, idWidth, name, type))
	{
		name = "UNKNOWN";
	}

}

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

int parse::getPositionFile()
{
	return file.getPositionFile();
}

void parse::getData(std::ostream & os)
{
	switch (type)
	{
	case MASTER:
	{
		os << "N/A";
		break;
	}
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
	default:
		file.readBits(getuint64(size, sizeWidth));
		break;
	}
}

ostream& operator<<(ostream &os, parse &p)
{
	p.getData(os);
	return os;
}