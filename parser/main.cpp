// Main.cpp
// Alex Cater and Andrew Adler
// Updated: 24 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, only the very first bits are parsed, but we have
//     plans to write the rest
// TODO:
//	 was unable to remove opening/closing file from parsing class
//		I couldn't figure out how to pass ifstream inside the fileName class (needs to be by reference)
//		keep getting deleted function
//		maybe change it to have readFile class call the parse class instead of other way around
//		this is only important for optimization/cleanliness
//	finish parsing differnt ebml element data types (only date left)
//	add level tracking of element
//	switch from bare points to unique or shared pointers
//		only memory allocated is buffer in readFile so probably not very important
//	add verbose log and otherwise control output
//		so not always just spammed with the whole file
//		maybe select parts of the file to parse or by name/id/type
//	add error/exception handling
//		currently crashing when try to parse some files
//		couts when some errors happens but errors not actually handled

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
	cout << "(" << std::hex << std::showbase << ( position  )<< ":" << type << ") " << name << " - " << p << endl;
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
	while (pos < 100)
	{
		parse p = parse(fileName, pos); // create new parser object for next ebml element to be parsed
		p.parseElement(); // parse ebml element

		string typeName = getebmlTypeName(p.getType()); // find string of ebml type
		string name = p.getName(); // get ebml element name
		print(name, typeName, p.getID(), p.getIDWidth(), p.getSize(), p.getSizeWidth(), pos, p); // print parsed info to cout
		
		pos = p.getPositionFile(); // update position counter

		if (pos == size)
		{
			cout << "Reached end of file. Exiting." << endl;
			break;
		}
	}
}

int main()
{
	// eventually will test all of matroska test files
	//		test2.mkv crashes
	//		test7.mkv hangs
	// https://www.matroska.org/downloads/test_w1.html
	// and/or take input from user
	string fileName = "../test files/test1.mkv";
	parseFile(fileName); // start parsing file
	system("pause"); // stop visual studio from exiting
}
