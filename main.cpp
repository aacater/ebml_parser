#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <vector>
#include <array>
#include <chrono>
#include <iomanip>

#include <unistd.h> // requires unix
#include <time.h>

#include "spec.cpp" // defines different ebml types with name, identifier, and data type

using std::cout;
using std::string;
using std::endl;
using std::hex;
using std::ostream;
using std::array;
using std::bitset;
using std::dec;
using std::fixed;
using std::time_t;
using std::strcmp;

#define BUFSIZE 1048576 // 1MB = 1024^2

enum ebml_element_type { // posible data types
	MASTER,	// other EBL sub-elemnts of next lower level
	UINT,	// unsigned integer, 1 to 8 bytes
	INT,	// signed integer, 1 to 8 bytes
	STRING,	// ASCII characters 0x20 to 0x7E
	UTF8,	// unicode, padded with zeros
	BINARY,	// not parsed
	FLOAT,	// big-endian, 4 and 8 bytes (32 or 64 bits)
	DATE	// signed 8 byte integer in nanoseconds
};

class simple_vint{
public:
	uint8_t width;
	uint8_t data[8];

	bool is_all_ones(){ // check if data is 255 (0xFF )
		for(int i = 1; i < width - 1; i++){
			if(data[i] != 255){
				return false;
			}
		}
		return true;
	}

	uint64_t get_uint(){ // convert data to unsigned into - big endian
		uint64_t value = 0;
		value = data[width - 1];
		for(int i = width - 1; i > 0; --i){
			value += ((uint64_t)data[i - 1] << ((width - i) * 8));
		}
		return value;
	}
};

class ebml_element{ // ebml element defined using data from spec.cpp
public:
	string name;
	array<uint8_t, 4> id;
	enum ebml_element_type type;

	ebml_element(string name, array<uint8_t, 4> const& id, enum ebml_element_type type)
	:name(name), id(id), type(type){}
};

const int SPEC_LEN = 250;


// compares data to spec.cpp to find which ebml element the data is
ebml_element* get_element(array<uint8_t, 4> id, uint8_t level){
	bool found;
	for(int i = 0; i < SPEC_LEN; ++i){
		found = true;
		for(int j = 0; j < level; ++j){ // check for matching id
			if(ebml_spec[i]->id[j] != id[j]){
				found = false;
				break;
			}
		}
		if(found){	// if found return the element info
			return ebml_spec[i];
		}
	}
	return 0;
}
// logging stuff
// if verbose = true then prints bits of data
bool verbose = false;
typedef ostream& (*manip) (ostream&);
class verbose_log{};
template <class T> verbose_log& operator<< (verbose_log& l, const T& x){
	if(verbose)
		cout << x;
	return l;
}
verbose_log& operator<< (verbose_log& l, manip manipulator){
	if(verbose)
		cout << manipulator;
	return l;
}
verbose_log vlog;

class ebml_parser{
public:
void parse(int fd){
	int len, mask, pos = 0;
	uint8_t buffer[BUFSIZE];
	bitset<8> bits;

	while(1){
		// Get EBML Element ID first byte.
		if((len = read(fd, buffer, 1)) < 0){						// check if read length is less than zero
			cout << "Uh oh, read first id byte error!\n";		// exit if so
			break;
		}else if(len == 0){											// check if read length is zero
			cout << "DONE!" << endl;						// reached end of file
			break;
		}
		pos++;														// increment pos

		if(buffer[0] == 0){											// skip until buffer is non zero
			cout << "Read '0' byte..." << endl;
			continue;
		}

		bits = bitset<8>(buffer[0]);
		vlog << "Element ID First Byte: " << bits << endl;

		simple_vint id;
		id.width = 1;
		mask = 0x80;
		// Get EBML Element ID vint width.
		while(!(buffer[0] & mask)){
			mask >>= 1;
			id.width++;
		}

		id.data[0] = buffer[0];
		// Get EBML Element ID vint data.
		if((len = read(fd, buffer, id.width - 1)) != id.width - 1){
			cout << "Uh oh, read id data error!\n";
			break;
		}
		pos += id.width - 1;
		vlog << "Element ID Bytes: " << bits;
		// Get EBML Element ID.
		for(int i = 1; i < id.width; ++i){
			id.data[i] = buffer[i - 1];
			bits = bitset<8>(buffer[i]);
			vlog << ' ' << bits;
		}
		vlog << endl;
		if(verbose){
			vlog << "Element ID: 0x";
			for(int i = 0; i < id.width; ++i){
				vlog << hex << (int)id.data[i];
			}
			vlog << endl;
		}

		// Get EBML Element Size first byte.
		if((len = read(fd, buffer, 1)) != 1){
			cout << "Uh oh, read first size byte error!\n";
			break;
		}
		pos++;

		bits = bitset<8>(buffer[0]);
		vlog << "Element Size First Byte: " << bits << endl;

		simple_vint size;
		size.width = 1;
		mask = 0x80;
		// Get EBML Element Size vint width.
		while(!(buffer[0] & mask)){
			mask >>= 1;
			size.width++;
		}

		buffer[0] ^= mask;
		size.data[0] = buffer[0];
		// Get EBML Element Size vint data.
		if((len = read(fd, buffer, size.width - 1)) != size.width - 1){
			cout << "Uh oh, read id data error!\n";
			break;
		}
		pos += size.width - 1;
		bits = bitset<8>(size.data[0]);
		vlog << "Element Size Bytes: " << bits;
		// Get EBML Element Size.
		for(int i = 1; i < size.width; ++i){
			size.data[i] = buffer[i - 1];
			bits = bitset<8>(buffer[i]);
			vlog << ' ' << bits;
		}
		vlog << endl;
		vlog << "Element Size: " << dec << size.get_uint() << endl;

		// Specification for ID lookup.
		ebml_element* e = get_element(
			{{id.data[0], id.data[1], id.data[2], id.data[3]}},
			id.width);

		vlog << "--------------------------------------------------------" << endl;
		if(e != 0){
			if(e->type != MASTER){
				// Get EBML Element Data, parse it.
				uint64_t data_len = size.get_uint();
				if((len = read(fd, buffer, data_len) != data_len)){
					cout << "Uh oh, could not read all the data!" << endl;
					cout << "Wanted " << data_len << " found " << len << endl;
					break;
				}
				pos += data_len;
				cout << '(' << dec << pos << ") " << e->name << ": ";
				if(e->type == STRING || e->type == UTF8){
					for(int i = 0; i < data_len; ++i){
						cout << buffer[i];
					}
					cout << endl;
				}else if(e->type == BINARY){
					for(int i = 0; i < data_len; ++i){
						// I'll only care about the first 32 binary bytes.
						if(i == 32 && !verbose){
							cout << "...";
							break;
						}
						cout << hex << (int)buffer[i];
					}
					cout << endl;
					if(e->name == "SimpleBlock" || e->name == "Block"){
						bits = bitset<8>(buffer[0]);
						vlog << "Block First Byte: " << bits << endl;
						simple_vint track_number;
						track_number.width = 1;
						mask = 0x80;
						while(!(buffer[0] & mask)){
							mask >>= 1;
							track_number.width++;
						}
						buffer[0] ^= mask;
						vlog << "Block Track Number Bytes: " << endl;
						for(int i = 0; i < track_number.width; ++i){
							bits = bitset<8>(buffer[i]);
							vlog << bits << ' ';
							track_number.data[i] = buffer[i];
						}
						cout << "Track Number: " << dec << (int)track_number.get_uint() << endl;
						int16_t timecode = (int16_t)(((uint16_t)buffer[track_number.width] << 8) | buffer[track_number.width + 1]);
						cout << "Timecode: " << dec << (int)timecode << endl;
					}
				}else if(e->type == UINT){
					simple_vint data;
					data.width = 0;
					for(int i = 0; i < data_len; ++i){
						data.data[i] = buffer[i];
						data.width++;
					}
					uint64_t val = data.get_uint();
					cout << dec << val;
					if(e->id[0] == 0x83){
						if(val == 1){
							cout << " (video)";
						}else if(val == 2){
							cout << " (audio)";
						}else if(val == 3){
							cout << " (complex)";
						}
					}
					cout << endl;
				}else if(e->type == FLOAT){
					simple_vint data;
					data.width = 0;
					for(int i = 0; i < data_len; ++i){
						data.data[i] = buffer[i];
						data.width++;
					}
					int64_t int_val = int64_t(data.get_uint());
					if(data.width == 4){
						float float_val;
						memcpy(&float_val, &int_val, 4);
						cout << fixed << dec << float_val;
					}else if(data.width == 8){
						double double_val;
						memcpy(&double_val, &int_val, 8);
						cout << fixed << dec << double_val;
					}else{
						cout << "Bad float width:: " << dec << (int)data.width;
					}
					cout << endl;
				}else if(e->type == DATE){
					simple_vint data;
					data.width = 0;
					for(int i = 0; i < data_len; ++i){
						data.data[i] = buffer[i];
						data.width++;
					}
					// This is a time_t struct composed of:
					// time point 0 (Jan 1 1970)
					// nanoseconds to millenium (Jan 1 2001)
					// nanoseconds to date file was created.
					time_t date_val = std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point::time_point() + std::chrono::nanoseconds(978307200000000000) + std::chrono::nanoseconds(int64_t(data.get_uint())));
					// This is the local time; PST in San Francisco, -7h or something.)
					//cout << ctime(&date_val);
					// This is the UTC time.
					cout << asctime(gmtime(&date_val));
				}else if(e->type == INT){
					simple_vint data;
					data.width = 0;
					for(int i = 0; i < data_len; ++i){
						data.data[i] = buffer[i];
						data.width++;
					}
					cout << dec << int64_t(data.get_uint()) << endl;
				}else{
					for(int i = 0; i < data_len; ++i){
						cout << hex << (int)buffer[i];
					}
					cout << endl;
				}
			}else{
				// Master data is actually just more elements, continue.
				cout << '(' << dec << pos << ')' << " ----- " << e->name << " [";
				if(size.is_all_ones()){
					cout << "Unknown";
				}else{
					cout << size.get_uint();
				}
				cout << ']' << endl;
			}
		}else{
			cout << "UNKNOWN ELEMENT!" << endl;
		}
		vlog << "--------------------------------------------------------" << endl;
	}
}
};

int main(int argc, char** argv){
	for(int i = 0; i < argc; i++){
                if(strcmp(argv[i], "-v") == 0){
                        verbose = true;
                }
	}

	ebml_parser p;
	p.parse(STDIN_FILENO);

	return 0;
}