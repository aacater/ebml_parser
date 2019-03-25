// Main.cpp
// Alex Cater and Andrew Adler
// Updated: 24 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, only the very first bits are parsed, but we have
//     plans to write the rest
// TODO:
//	 was unable to remove opening/closing file from parsing class
//		I couldn't figure out how to pass ifstream inside the fileName class
//		keep getting deleted function
//		maybe write to have file class call the parse class instead of other way around
//		this is only important for optimization/cleanliness
//	finish parsing differnt ebml element data types
//	add level tracking of element

#include "parse.h"
#include "helper.h"
#include "ebml.h"

using std::string;
using std::cout;
using std::endl;

// print parsed info about an ebml element
void print(string name, string type, uint8_t * id, uint8_t idWidth, uint8_t * size, uint8_t sizeWidth, int position, parse& p)
{
	int mask = 0x80;
	for (int i = 1; i < sizeWidth; i++)
	{ // calculate first byte of size
		mask >>= 1;
	}

	cout << "\n------------------------------------------------\n" << endl;
	cout << "(" << std::dec << position << "-" << type << ") " << name << " - " << p << endl;
	cout << std::showbase << std::hex << std::nouppercase;
	cout << "Element ID: " << getuint64(id, idWidth) << " of width " << std::noshowbase << getuint64(&idWidth) << endl;
	cout << "Element Size: " << std::dec << getuint64(size, sizeWidth) << " (" << std::showbase << std::hex << (size[0] ^ mask) << " with width "  << std::noshowbase << std::dec << getuint64(&sizeWidth) << ")" << endl;
}

// parses one ebml element at a time from file
void parseFile(string fileName)
{
	readFile file = readFile(fileName);
	int size = file.getFileSize();
	cout << "File size: " << size << " bytes."<< endl;

	int pos = 0;
	int startPos = 40;
	while (pos < startPos+1)
	{
		parse p = parse(fileName, pos); // create new parser object for next ebml element to be parsed
		p.parseElement(); // parse ebml element

		string typeName = getebmlTypeName(p.getType()); // find string of ebml type
		string name = p.getName(); // get ebml element name
		if (p.getPositionFile() > startPos)
		{
			print(name, typeName, p.getID(), p.getIDWidth(), p.getSize(), p.getSizeWidth(), p.getPositionFile(), p); // print parsed info to cout
		}
		else
		{ // for testing
			// probably not the best way.
			std::cout.setstate(std::ios_base::badbit);
			print(name, typeName, p.getID(), p.getIDWidth(), p.getSize(), p.getSizeWidth(), p.getPositionFile(), p); // print parsed info to cout
			std::cout.clear();
		}
		
		
		pos = p.getPositionFile(); // update position counter
	}
}

int main()
{
	// eventually will test all of matroska test files
	// https://www.matroska.org/downloads/test_w1.html
	// and/or take input from user
	string fileName = "../test1.mkv";
	parseFile(fileName); // start parsing file

	system("pause"); // stop visual studio from exiting
}
