// hex_read.cpp
// Alex Cater and Andrew Adler
// Updated: 20 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, only the very first bits are parsed, but we have
//     plans to write the rest

#include "ebml.h"
#include "spec.h"
using std::cout;
using std::endl;
using std::array;
using std::string;

using std::ios;


#define BUFSIZE 1024
// couldn't get it to work in ebml.cpp
ebml_element get_element2(array<uint8_t, 4> id, uint8_t level) {
	bool found;
	for (int i = 0; i < SPEC_LEN; ++i) {
		found = true;
		for (int j = 0; j < level; ++j) { // check for matching id
			if (getSpecPointer()[i].id[j] != id[j]) {
				found = false;
				break;
			}
		}
		if (found) {	// if found return the element info
			return getSpecPointer()[i];
		}
	}
	return 0;
}

int main()
{
	uint8_t buffer[BUFSIZE];
	int mask = 0x80;
	int counter = 0;

	// open test mkv file
    std::ifstream infile;
    infile.open("../test1.mkv", ios::binary | ios::in);
    if (!infile.is_open())
    {
        cout << "Error opening input file" << endl;
        return 0;
    }
	int pos = 0;
    while (true)
    {
		counter++;
		//read one byte from file
		infile.read(reinterpret_cast<char*>(&buffer[pos]), 1);
		// find first high bit, this shows the class of the ID
		elementData id;
		id.width = 1;
		mask = 0x80;
		while (!(buffer[pos] & mask)) { // find class
			mask >>= 1;
			id.width++;
		}
		printf("Element ID First Byte: 0x%X (len: %d)\n", buffer[pos], id.width);
		pos++;

		// read full element ID using the width of class
		infile.read(reinterpret_cast<char*>(&buffer[pos]), id.width - 1);
		// print full element ID
		printf("Element ID Bytes: 0x");
		for (int i = 0; i < id.width; ++i) {
			id.data[i] = buffer[pos+i-1];
			printf("%X", id.data[i]);
		}
		printf("\n");
		pos += id.width - 1;

		// get Element Size
		infile.read(reinterpret_cast<char*>(&buffer[pos]), 1);
		elementData size;
		size.width = 1;
		mask = 0x80;
		while (!(buffer[pos] & mask)) { // find class
			mask >>= 1;
			size.width++;
		}
		printf("Element ID First Byte: 0x%X (len: %d)\n", buffer[pos], size.width);
		// XOR - for some reason this calculates the actual size
		// I think because for the element ID the class bits are included where as for the size the class is not included in the actual size.
		// if it was all the sizes would be very big (because theres a high bit in one of the most signifigant bits)
		buffer[pos] = buffer[pos] ^ mask; 
		pos++;
		
		printf("Element Size Bytes: 0x");
		for (int i = 0; i < size.width; ++i) {
			size.data[i] = buffer[pos+i-1];
			printf("%X", size.data[i]);
		}
		printf("\n");
		pos += size.width - 1;
		array<uint8_t, 4> idarr = { id.data[0], id.data[1], id.data[2], id.data[3] };
		auto ele = get_element2(idarr, id.width);
		printf("(%d) %s\n", pos, ele.name);


		if(counter > 1)
			break;
    }
	system("PAUSE"); // stop visual studio from exiting
}
