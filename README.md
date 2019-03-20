# ebml-parser
original plan was to write a parser from scratch. That's too complicated for now. 

So now I'm commenting a parser I found.

https://matroska.org/technical/specs/index.html

https://github.com/Matroska-Org/libebml

https://github.com/Matroska-Org/ebml-specification


needs to be complied on unix (requieres unistd.h library)

I've included my compiled version (file named ebml_parser)

then to run

./ebml-parser < ./test1.mkv | less

mkv file was found here (https://github.com/Matroska-Org/matroska-test-files/tree/master/test_files)
