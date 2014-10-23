/////////////////////////////////////////////////////////////////
// main file for testing comIO2 class.
// cd /cygdrive/c/Users/SystemControl/Dropbox/OPCE/CSCI-430/eggBeater/comIO
// This code has been tested with cygwin. 
// Comment out #define BUF_MAX_SIZE 1000 to remove message output.

#include "comIO.h"

#define BUF_MAX_SIZE 1000


int main(){
  string file1="file1.txt";
  string file2="file2.txt";
  char* buffer = (char*) malloc(BUF_MAX_SIZE);
  comIO mycom;

  mycom.open(file1);
  if( mycom.test(file1) ) cout << "Open success!\n";
  else cout << "Open fail...\n";
  
  mycom.read(buffer, BUF_MAX_SIZE);
  cout << buffer << endl;
  mycom.close();
  
  mycom.open(file2);
  mycom.write(buffer,strlen(buffer));
  
  mycom.close();
  
return 0;
}// end main.