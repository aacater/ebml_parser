#include "ebml.h"
#include "spec.h"

using std::cout;
using std::endl;
using std::array;
using std::string;
using std::ios;

// convert data to unsigned into - big endian
uint64_t elementData::get_uint() { 
	uint64_t value = 0;
	value = data[width - 1];
	for (int i = width - 1; i > 0; --i) {
		value += ((uint64_t)data[i - 1] << ((width - i) * 8));
	}
	return value;
}

// compares data to spec.cpp to find which ebml element the data is

ebml_element get_element(array<uint8_t, 4> id, uint8_t level) {
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