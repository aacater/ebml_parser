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

#define stringify( type ) # type

using std::string;
using std::cout;
using std::endl;

void print(string name, string data, ebml_element_type type, uint8_t * id, uint8_t idWidth, uint8_t * size, uint8_t sizeWidth, int position)
{
	cout << "\n------------------------------------------------" << endl;
	printf("(%s at %d) %s []\n", stringify(type), position, name.c_str());
	cout << "Element ID: 0x" << std::hex << convHex(id, idWidth) << " of width " << std::hex << convHex(&idWidth) << endl;
	cout << "Element Size: " << convHex(size, sizeWidth) << " of width " << std::hex << convHex(&sizeWidth) << endl;
	cout << "------------------------------------------------" << endl;
}

void parseFile(string fileName)
{
	readFile file = readFile(fileName);
	int size = file.getFileSize();
	cout << "File size: " << size << " bytes."<< endl;

	int pos = 0;
	while (pos < 10)
	{
		parse p = parse(fileName, pos);
		cout << "POS: " << p.getPositionFile() << endl;
		p.parseElement();

		string name;
		ebml_element_type type = STRING;
		p.lookupElement(p.getID(), p.getIDWidth(), name, type);
		print(name, "data", type, p.getID(), p.getIDWidth(), p.getSize(), p.getSizeWidth(), p.getPositionFile());
		pos = p.getPositionFile();
		//delete p;
	}
}

int main()
{
	string fileName = "../test1.mkv"; // eventually take input from user
	parseFile(fileName);

	system("pause"); // stop visual studio from exiting
}
