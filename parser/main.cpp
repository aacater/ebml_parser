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
//	also parse more info about SimpleBlocks/Blocks
//	add level tracking of element
//	switch from bare points to unique or shared pointers
//		only memory allocated is buffer in readFile so probably not very important
//	add verbose log and otherwise control output
//		so not always just spammed with the whole file
//		maybe select parts of the file to parse or by name/id/type
//	add error/exception handling

#include "parse.h"
#include "helper.h"
#include "ebml.h"

#include <fstream>
using std::fstream;
using std::ofstream;

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

    std::ofstream outFile;
    outFile.open("../output.txt", fstream::app);

    outFile << "\n--------------------------------------------------\n" << endl;
    outFile << "(" << std::hex << std::showbase << ( position  )<< ":" << type << ") " << name << " - " << p << endl;
    outFile << std::showbase << std::hex << std::nouppercase;
    outFile << "Element ID: " << getuint64(id, idWidth) << " of width " << std::noshowbase << getuint64(&idWidth) << endl;
    outFile << "Element Size: " << std::dec << getuint64(size, sizeWidth) << " (" << std::showbase << std::hex << (size[0] ^ mask) << " with width "  << std::noshowbase << std::dec << getuint64(&sizeWidth) << ")" << endl;

    outFile.close();
}

// parses one ebml element at a time from file
void parseFile(string inputFile, string outputFile)
{
    cout << endl << "Parsing file..." << endl;

    readFile file(inputFile);
	int size = file.getFileSize();

	std::ofstream outFile;
	outFile.open(outputFile, fstream::app);
	outFile << endl << "File size: " << size << " bytes."<< endl;

	int pos = 0;
	while (true)
	{
		parse p(inputFile, pos); // create new parser object for next ebml element to be parsed
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
	//		test2.mkv: filePosition goes to -1. it tries to read past the end of the file because a size is too big
	//		test7.mkv: lots of 0s for IDs. currently just skips until valud id. then tries to read past end of file
	// https://www.matroska.org/downloads/test_w1.html
	// and/or take input from user
	//for (int i = 1; i < 9; i++)
	//{
	//	string inputFile = "../test files/test" + std::to_string(i) + ".mkv";
	//	parseFile(inputFile); // start parsing file
	//}
	
	string inputFile = "../test files/test1.mkv";
	string outputFile = "../output.txt";

	ofstream outFile;
	outFile.open(outputFile);
	outFile.close();

	parseFile(inputFile, outputFile); // start parsing file

	system("pause"); // stop visual studio from exiting
}
