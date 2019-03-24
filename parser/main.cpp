// Main.cpp
// Alex Cater and Andrew Adler
// Updated: 20 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, only the very first bits are parsed, but we have
//     plans to write the rest
// TODO:
//	 was unable to remove opening/closing file from parsing class
//		I couldn't figure out how to pass ifstream inside the fileName class
//		keep getting deleted function

#include "parse.h"
#include "helper.h"
#include "ebml.h"

using std::string;
using std::cout;
using std::endl;

void print(string name, string data, string type, uint8_t * id, uint8_t idWidth, uint8_t * size, uint8_t sizeWidth, int position, parse& p)
{
	int mask = 0x80;
	for (int i = 1; i < sizeWidth; i++)
	{ // calculate first byte of size
		mask >>= 1;
	}

	cout << "\n------------------------------------------------\n" << endl;
	cout << "(" << type << " at " << std::dec << position << ") " << name << " [" << p << "]" << endl;
	cout << std::showbase << std::hex << std::nouppercase;
	cout << "Element ID: " << getuint64(id, idWidth) << " of width " << std::noshowbase << getuint64(&idWidth) << endl;
	cout << "Element Size: (" << std::showbase << (size[0] ^ mask) << ") " << std::noshowbase << std::dec << getuint64(size, sizeWidth) << " of width " << std::hex << getuint64(&sizeWidth) << endl;
}

void parseFile(string fileName)
{
	readFile file = readFile(fileName);
	int size = file.getFileSize();
	cout << "File size: " << size << " bytes."<< endl;

	int pos = 0;
	while (pos < 20)
	{
		parse p = parse(fileName, pos);
		//cout << "POS: " << p.getPositionFile() << endl;
		p.parseElement();

		string typeName = getebmlTypeName(p.getType());
		string name = p.getName();

		print(name, "data", typeName, p.getID(), p.getIDWidth(), p.getSize(), p.getSizeWidth(), p.getPositionFile(), p);
		pos = p.getPositionFile();
	}
}

int main()
{
	string fileName = "../test1.mkv"; // eventually take input from user
	parseFile(fileName);

	system("pause"); // stop visual studio from exiting
}
