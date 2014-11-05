// main for testing getOpt, parsing 
// ./getOpt.exe -a encrypt -f file1 -f file2 -f file3 -s 0056 -f file4 -o option1 -o option2



#include <cstdlib>
#include <iostream>
#include "getOpt.h"


int main(int argc, char *argv[]){

  getOpt specs(argc,argv);

  cout << "numFiles= " << specs.getNumFiles() << endl;
  cout << "file[0]= " << specs.getFilePath(0) << endl;
  cout << "file["<< (specs.getNumFiles() - 1) << "]= " << specs.getFilePath(specs.getNumFiles() - 1) << endl;

  if(specs.getNumSessions() == 1) cout << "Session= " << specs.getSessionID() << endl;
  else if(specs.getNumSessions() >= 1)  cout << "Too many session ID\'s specified!\n";
  else cout << "No session ID specified!\n";


  return 0;
}// end main