// hex_read.cpp
// Alex Cater and Andrew Adler
// Updated: 20 March 2019
// This file reads the binary data from a file and prints it in bytes
//     currently, the bytes are not parsed or interpreted, but we have
//     plans to write those parts

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ios;

int main()
{
    char x;
    std::ifstream infile;
    infile.open("test1.mkv", ios::binary | ios::in);
    if (!infile.is_open())
    {
        cout << "Error opening input file" << endl;
        return 0;
    }
    int counter = 0;
    while (infile.read(&x, sizeof x))
    {
        counter++;
        if (counter == 100) break;
        printf("0x%X\n",(unsigned char)x);
        //cout << x << endl;
    }
}
