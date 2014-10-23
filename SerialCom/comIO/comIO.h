// comIO2 header file.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>

#define DEBUG_FLAG_comIO

using namespace std;


class comIO{
protected:
fstream dataStream;
string currentFilepath;
public:

comIO(); // Constructor.
~comIO(){
       cout << "Destructor Structor\n";
	   currentFilepath = "\0";
       }; // Destructor.
	   
int open(string filePath);
int test(string Line);

unsigned write(char* buf, unsigned bufSize, unsigned startIndex = 0 );
unsigned read( char* buf, unsigned bufSize, unsigned startIndex = 0 );

int close();

}; // end comIO def

