Before compiling, install the suffix array library from -
https://github.com/simongog/sdsl

Then compile using:
g++ -ggdb SuffixArrayCode.cpp rank9b.cpp -lsdsl -ldivsufsort

Run using:
./a.out <input file>
