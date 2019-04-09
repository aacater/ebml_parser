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
//	when printing the position of end of element is printed not start

#include "parse.h"
#include "ebml.h"

using std::string;
using std::cout;
using std::endl;

// parses one ebml element at a time from file
void parseFile(string fileName)
{
	readFile file = readFile(fileName);
	int size = file.getFileSize();
	cout << "File size: " << size << " bytes."<< endl;

	int pos = 0;
	while (true)
	{
		parse p = parse(fileName, pos); // create new parser object for next ebml element to be parsed
		p.parseElement(); // parse ebml element

		cout << p << endl;
		
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
	//	string fileName = "../test files/test" + std::to_string(i) + ".mkv";
	//	parseFile(fileName); // start parsing file
	//}
	
	string fileName = "../test files/test1.mkv";


	parseFile(fileName); // start parsing file
	
	system("pause"); // stop visual studio from exiting
}
