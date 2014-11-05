// getOpt header file.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#define DEBUG_FLAG_getOpt

using namespace std;

class getOpt{
protected:

std::vector<std::string> sessions;
unsigned numSessions;

std::vector<std::string> actions;
unsigned numActions;

std::vector<std::string> files;
unsigned numFiles;

std::vector<std::string> options;
unsigned numOptions;


public:

getOpt(int argc, char *argv[]); // Constructor.
~getOpt(){
       }; // Destructor.
	   
int extractFiles(int argc, char *argv[]);
int extractSession(int argc, char *argv[]);
int extractAction(int argc, char *argv[]);
int extractOptions(int argc, char *argv[]);

unsigned getNumFiles();
unsigned getNumActions();
unsigned getNumSessions();
unsigned getNumOptions();

string getFilePath(unsigned index);
string getAction(unsigned index = 0);
string getOption(unsigned index);
string getSessionID(unsigned index = 0);


}; // end getOpt header



// need to add code to handle file paths with spaces in them.





