# ebml Parser
This project is a parser for ebml files that reads an mkv file and outputs information about the file's metadata into a specified text file. The program reads the input into a buffer and goes through the buffer checking for element byte identifiers and parsing each element based on whatever type it is. After reading the information from whatever the data is, it adds that information to the output file.

This program allowed us to learn about the structure of ebml files, which is similar to other binary file types. We also learned about how choosing how data is stored in memory can lead to big sacrifices in efficiency or security if not done optimally.

## Related Links

[Matroska specifications]
(https://matroska.org/technical/specs/index.html)

[Some more matroska specifications]
(https://matroska-org.github.io/libebml/specs.html)

[mkv files was found here]
(https://github.com/Matroska-Org/matroska-test-files/tree/master/test_files)

## ebml Types
The possible types of elements in the mkv file are:

- Signed Integer

	- Big-endian
	- any size from 1 to 8 octets

- Unsigned Integer

	- Big-endian
	- any size from 1 to 8 octets

- Float

	- Big-endian
	- defined for 4 and 8 octets (32, 64 bits)

- String

	- Printable ASCII (0x20 to 0x7E)
	- zero-padded when needed

- UTF-8

	- Unicode string, zero padded when needed (RFC 2279)

- Date
	- signed 8 octets integer in nanoseconds with 0 indicating the precise beginning of the millennium (at 2001-01-01T00:00:00 UTC)

- Master-element
	- contains other EBML sub-elements of the next lower level

- Binary

	- the actual audio/video data
	- not interpreted by the parser

## File and Folder Descriptions
- /parser/

    - folder that contains code for the parser
    - parser.cpp/h is what figures out what each of the elements in the file is and reads each element as whatever type it is
    - main.cpp runs the main interface to set up input and output files and keeps track of progress of the parser
    - ebml.cpp/h contain information about the different byte identifiers in ebml files

- test files

    - the test files are the mkv files that the parser is tested with

- hex_dump.c and hexdump.txt

	- hex dump of test1.mkv to verify that parser is working properly

- output.txt

	- where the parser prints the parsed data to
	- currently example output of parsed data from test1.mkv

## Some Potential Security Implications
Since our current program reads the file into a buffer, if the file being read exceeds the possible buffer size, the file could potentially overwrite other data in memory. To avoid this, a maximum buffer size could be specified, allowing for a first buffer to be used until the maximum buffer size is reached, and then store the rest in a second buffer once the first buffer is done.
