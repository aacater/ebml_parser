// hex_read.cpp
// Alex Cater and Andrew Adler
// Updated: 20 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, only the very first bits are parsed, but we have
//     plans to write the rest

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ios;

#define READLEN 1 // read one byte

int main()
{

	unsigned char readByte;
	char read;
    std::ifstream infile;
    infile.open("../test1.mkv", ios::binary | ios::in);
    if (!infile.is_open())
    {
        cout << "Error opening input file" << endl;
        return 0;
    }
    int position = 0;
    while (infile.read(&read, READLEN))
    {
		unsigned char readByte = (unsigned char)read;
        position++;
        if (position == 5) break;

		
		unsigned char highBits = readByte >> 4; // get high 4-bits
		printf("0x%X\n", highBits);


		printf("Element ID First Byte: %X\n", readByte);
		int idLen = 1;
		switch (highBits) // class dictates how long in bytes the element id 
		{
		case 0x8:
			cout << "Class A ID" << endl;
			idLen = 1;
			break;
		case 0x4:
			cout << "Class B ID" << endl;
			idLen = 2;
			break;
		case 0x2:
			cout << "Class C ID" << endl;
			idLen = 3;
			break;
		case 0x1:
			cout << "Class D ID" << endl;
			idLen = 4;
			break;
		default:
			cout << "Unknown Class" << endl;
			break;
		}
		idLen = 2;
		unsigned char elementId[4];
		elementId[3] = readByte;

		for (int i = 1; i < idLen; i++) // atempting to put the read bytes into a character array. getting messed up with endianess.
		{
			infile.read(&read, READLEN);
			printf("READ: %X\n", (unsigned char)read);
			elementId[idLen] = (unsigned char)read;
		}
		printf("Element ID Bytes: 0x%X%X%X%X\n", elementId[3], elementId[2], elementId[1], elementId[0]);
		printf("Element ID Bytes: %X\n", elementId);

		break;
    }
	system("PAUSE"); // stop visual studio from exiting
}
