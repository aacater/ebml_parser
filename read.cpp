#include <iostream>
#include <cstring>
#include <cstdlib>
#include <bitset>

#include <unistd.h>

using std::strcmp;
using std::hex;
using std::dec;
using std::endl;
using std::cout;
using std::bitset;
using std::stoi;

#define BUFSIZE 8192

int main(int argc, char** argv){
	int len;
	uint8_t buffer[BUFSIZE];

	int total_bytes = 0;
	int from_bytes = 0;
	int to_bytes = 0;

	for(int i = 0; i < argc; i++){
		if((strcmp(argv[i], "-f") == 0) && argc > i){
                        from_bytes = stoi(argv[i + 1]);
                }else if((strcmp(argv[i], "-t") == 0) && argc > i){
                        to_bytes = stoi(argv[i + 1]);
                }else if((strcmp(argv[i], "-l") == 0) && argc > i){
                        to_bytes = from_bytes + stoi(argv[i + 1]);
                }
	}

	if((len = read(STDIN_FILENO, buffer, BUFSIZE)) == -1){
		cout << "Uh oh, read error!\n";
		return 1;
	}

	cout << "Read " << len << " bytes." << endl;

	for(int i = 0; i < len; ++i){
		if(to_bytes > 0 && total_bytes + i >= to_bytes){
			break;
		}

		bitset<8> bits(buffer[i]);
		cout << bits << endl;
		cout << hex << (int)buffer[i] << endl;
		cout << dec << (int)buffer[i] << endl;
		cout << endl;
	}

	total_bytes += len;

	return 0;
}
