#!/bin/bash

g++ -std=c++11 -g -O3 -Wall -pedantic -fpermissive -Wunused-variable ./read.cpp -o read

g++ -std=c++11 -g -O3 -Wall -pedantic -fpermissive -Wunused-variable ./main.cpp -o ebml-parser
