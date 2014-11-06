////////////////////////////////////////////////////////////
// main for testing getOpt V2, parsing 
// ./getOpt.exe --decrypt -f file1 -f file2 -f file3 --session-id 70056 -f file4 -o option1 --cipher-mode ofb
// This has been tested successfully on windows using cygwin.

#include <cstdlib>
#include <iostream>
#include "getOpt.h"


int main(int argc, char *argv[]){

  getOpt specs(argc,argv); // Make entity "specs" to hold all options/arguments passed on the command line.

  if(specs.getErrorLevel() > 0){                                // if error level, then output error messages and quit.
    for(int fire = 0; fire < specs.getNumErrors(); fire++ ){
	  cout << specs.getError(fire) << endl;
	}// end for.
    return 0;
  }// end if
  
  cout << "Session= " << specs.getSessionID() << endl;          // read and display the session ID
  cout << "Action= " << specs.getAction() << endl;
  cout << "Cipher= " << specs.getCipher() << endl;
  
  cout << "numFiles= " << specs.getNumFiles() << endl;
  cout << "file[0]= " << specs.getFilePath(0) << endl;
  cout << "file["<< (specs.getNumFiles() - 1) << "]= " << specs.getFilePath(specs.getNumFiles() - 1) << endl;


  return 0;
}// end main