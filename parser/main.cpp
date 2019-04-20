// Main.cpp
// Alex Cater and Andrew Adler
// Updated: 19 April 2019
// This file reads the binary data from a file and prints info about
// like the bytes in hex and the meaning of the bytes
// TODO:
//	also parse more info about SimpleBlocks/Blocks
//	add level tracking of element
//	switch from bare points to unique or shared pointers
//		only memory allocated is buffer in readFile so probably not very important
//	add (more) error/exception handling

#include "parse.h"
#include "ebml.h"

using std::fstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

// parses one ebml element at a time from file
void parseFile(const string& inputFileName, const string& outputFileName)
{
	cout << endl << "Parsing file..." << endl;

	std::ofstream outFile;
	outFile.open(outputFileName, fstream::trunc);
	if (!outFile.is_open())
	{
		perror("ERROR: main: Unable to open file");
	}
	parse p = parse(inputFileName);
	int size = p.getSize();

	int pos = 0;
	while (true)
	{
		outFile << p; // parse and print data from file
		pos = p.getPosition(); // update position counter

		if (pos == size)
		{
			cout << "Reached end of file. Exiting." << endl;
			break;
		}
	}
	outFile.close();
}

int main()
{
	// results of running parser on all the test files. 
	//		test2.mkv: filePosition goes to -1. it tries to read past the end of the file because a size is too big
	//		test7.mkv: lots of 0s for IDs. currently just skips until valud id. then tries to read past end of file
	//		all the other files are parsed correctly.
	// https://www.matroska.org/downloads/test_w1.html
	// and/or take input from user
	//for (int i = 1; i < 9; i++)
	//{
	//	string fileName = "../test files/test" + std::to_string(i) + ".mkv";
	//	parseFile(fileName); // start parsing file
	//}
	
	string inputFileName = "../test files/test1.mkv";
	string outputFileName = "../output.txt";

	parseFile(inputFileName, outputFileName); // start parsing file

	return 0;
}
