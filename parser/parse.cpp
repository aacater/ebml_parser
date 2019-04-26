// parse.cpp
// function definitions for parse class

#include "parse.h"

using std::string;
using std::ostream;
using std::stringstream;
using std::shared_ptr;

// reads file contents into memory
parse::parse(const string& fileName) : idWidth(1), sizeWidth(1), positionBuffer(0), id(nullptr), size(nullptr), type(MASTER)
{
	std::cout << "Reading from media file" << std::endl;
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		perror("ERROR: main: Unable to open file");
	}

	file.seekg(0, file.end);
	fileSize = file.tellg();
	file.seekg(0);

	char* pointer = reinterpret_cast<char*>(malloc(fileSize));
	file.read(pointer, fileSize);
	
	shared_ptr<uint8_t> temp(reinterpret_cast<uint8_t*>(pointer));
	buffer.swap(temp);
	file.close();
}

// read the first byte and find the length of data
// class specifies the length of the data piece 
// then read data based on length
// returns pointer to data
uint8_t* parse::parseEleSizeorID(bool size)
{
	int mask = 0x80;
	int width = 1;

	while (true)
	{
		if (getuint64(buffer.get(), 1) == 0)
		{
			std::cout << "Element ID can not be zero" << std::endl;
			positionBuffer += 1;
			continue;
		}
		break;
	}

	while (!(buffer.get()[positionBuffer] & mask)) 
	{ // find first zero bit, this shows width
		mask >>= 1;
		width++;
	}

	uint8_t* readData = &buffer.get()[positionBuffer];
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
	positionBuffer += width;
	return readData;
}

// parses the id then size of element
void parse::parseElement()
{
	while (true)
	{
		id = parseEleSizeorID(false);
		size = parseEleSizeorID(true);
		if (!lookupElement(id, idWidth, name, type))
		{
			std::cout << "EBML Element not found. Position: " << std::hex << std::showbase << positionBuffer << std::endl;
			name = "UNKNOWN";
			positionBuffer += 1;
			continue;
		}
		break;
	}
}

// uses the id to lookup the Element Name and Type from list
bool parse::lookupElement(uint8_t* const& id, const int& width, std::string& name, ebml_element_type& type)
{
	bool found = false;
	for (int i = 0; i < SPEC_LEN; i++)
	{ // loop through each ebml element
		for (int j = 0; j < width; j++)
		{ // check each byte of ID
			if (ebml_spec[i]->id[j] != id[j])
			{
				//std::cout << std::hex << getuint64(&id.get()[j]) << " AND " << ebml_spec[i]->id[j] << std::endl;
				break;
			}
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
int parse::getPosition()
{
	return positionBuffer;
}

// returns size of file
int parse::getSize()
{
	return fileSize;
}

// prints ebml element data based on element type to ostream 
void parse::getData(std::ostream & os)
{
	if (type == MASTER)
	{
		// master type contains other elements so no data to parse
		os << "N/A";
		return;
	}
	//uint8_t* data = getuint64(size, sizeWidth));
	uint8_t* data = &buffer.get()[positionBuffer];
	uint8_t dataLength = getuint64(size, sizeWidth);
	switch (type)
	{
	case MASTER:
	{
		os << "N/A";
		break;
	}
	case UTF8: // parsed the same
	case STRING:
	{
		std::string str(data, data + dataLength); // substring
		os << std::noshowbase << std::hex << std::nouppercase << str;
		break;
	}
	case UINT:
	{
		os << std::noshowbase << std::dec << getuint64(data, dataLength);
		break;
	}
	case BINARY:
	{
		os << std::showbase << std::hex << std::nouppercase << std::setfill('0');
		for (int i = 0; i < dataLength; i++)
		{
			os << std::setw(2) << getuint64(&data[i],1);
			os << std::noshowbase;
			if (i > 8)
			{
				os << "...";
				break;
			}
		}
		break;
	}
	case FLOAT:
	{ // can be 4 or 8 bytes long
		if (dataLength != 4 && dataLength != 8)
			perror("ERROR: parse::getData: Invalid float dataLength");
		int64_t temp = int64_t(getuint64(data, dataLength));
		float float_val = *(float *)(&temp); // pointer magic
		os << std::fixed << std::dec << float_val;
		break;
	}
	case DATE:
	{	// 8 byte integer in nanosecods
		// 0 indicating the precise beginning of the millennium
		
		stringstream ss;
		time_t time_in_nanosec;
		ss << std::dec << getuint64(data, dataLength) / 1000000000; // convert to seconds
		ss >> time_in_nanosec;
		time_in_nanosec += 978307200; // nanoseconds between unix epoch (1970-01-01) and start of millennium (2001-01-01)

		time_t t = time_in_nanosec;
		struct tm buf;
		int dateerror = gmtime_s(&buf, &t); // might only work on windows. 
		//struct tm * buf = gmtime(&time_in_nanosec); // works on linux

		char dateString[80];
		// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
		strftime(dateString, sizeof(dateString), "%a %b %d, %Y @ %H:%M:%S", &buf);

		os << dateString;

		break;
	}
	case INT:
	{
		os << std::noshowbase << std::dec << int64_t(getuint64(data, dataLength));
		break;
	}
	default:
	{
		perror("ERROR: parse::getData: Invalid EBML element data type");
		exit(1);
		break;
	}
	}
	positionBuffer += (int)getuint64(size, sizeWidth);
}

// prints id, size, type and other data to ostream
void parse::print(std::ostream & os)
{
	int pos = positionBuffer;
	parseElement();

	int mask = 0x80;
	for (int i = 1; i < sizeWidth; i++)
	{ // calculate first byte of size
		mask >>= 1;
	}
	os << "(" << std::hex << std::showbase << pos << ":" << getebmlTypeName(type) << ") " << name << " - ";
	getData(os);
	os << std::endl;
	os << std::showbase << std::hex << std::nouppercase;
	os << "Element ID: " << getuint64(id, idWidth) << " of width " << std::noshowbase << getuint64(&idWidth) << std::endl;
	os << "Element Size: " << std::dec << getuint64(size, sizeWidth) << " (" << std::showbase << std::hex << (size[0] ^ mask) << " with width " << std::noshowbase << std::dec << getuint64(&sizeWidth) << ")" << std::endl;
	os << "\n--------------------------------------------------\n" << std::endl;
}

// overloads parse class object to << operator.
// used to print data to ostream
ostream& operator<<(ostream &os, parse &p)
{
	p.print(os);
	return os;
}

// returns a uint64_t made from data of variable length starting at passed pointer
// converts from big endian to little endian 
uint64_t getuint64(uint8_t * data, int length)
{
	uint64_t value = 0;
	value = data[length - 1];
	for (int i = length - 1; i > 0; --i) {
		value += ((uint64_t)data[i - 1] << ((length - i) * 8));
	}
	return value;
}