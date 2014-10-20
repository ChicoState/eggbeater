#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>

#define DEBUG_FLAG_comIO

using namespace std;


///////////////////////////////////////////////////////////
// Function to abstract I/O to file or com port
// File path is full path to file 
// mode can be: open, close, read, write
// buf is pointer to I/O buffer type char* 
// bufSize is the size of the data to be moved. Must be <= actual size of buffer. 
// start index is used if you do not want to begin read/write from beginning of file. For example could keep incrementing by size of data.
// This code operates on existing files. It will not create a if it does not exist.
// Once a file is open it can be operated on repeatedly since the comIO uses a static var for the file descriptor and path.
// You can not operate on more than one file at a time. But this should not be an issue since we only need one com port at a time.
// Code is written to operate on binary data not just ascii. However this means main must keep track of data sizes since the terminator is unknown to this funct.
// For actual use, comment out the "#define DEBUG_FLAG_comIO" aka just don't define the flag
// comIO returns a 0 on success and 1 on failure.
// Last edit 10-19-14

int comIO(string filePath, string mode, char* buf = NULL, unsigned bufSize = 0, unsigned startIndex = 0)
{
  static string currentFilePath;
  static fstream dataStream;
  
  if(filePath != currentFilePath && mode != "open")
  {
    #ifdef DEBUG_FLAG_comIO
	  cerr << "Trying to operate on: "<< filePath <<" But open file is: " << currentFilePath << endl;
	  cerr << "Closing: " << currentFilePath << endl;
	#endif
    dataStream.close();
	return 1;
  }
  
  if(mode == "open")///////////////////////////////////////////////////////////////// If mode is "open" 
  {
    #ifdef DEBUG_FLAG_comIO
	  cout << "Opening file: " << filePath << endl;
	#endif
    if( dataStream.is_open() && currentFilePath != filePath) // If someone trys to open a new file while old one is still open.
	{
	  #ifdef DEBUG_FLAG_comIO
	    cerr << "A file was already open: " << currentFilePath <<" Closing, then opening: "<< filePath << endl;
	  #endif
	  dataStream.close();  
	}
	else if(dataStream.is_open() && currentFilePath == filePath)
	{
      #ifdef DEBUG_FLAG_comIO
	    cerr << "This file was already open. Doing nothing.\n";
	  #endif
	  return 0;
	}
    dataStream.open( filePath.c_str(), ios::in | ios::out | ios::binary);
    if(!dataStream)                                                               // if file failed to open. 
	{ 
	  #ifdef DEBUG_FLAG_comIO
        cerr << "Error: File could not be opened: " << filePath.c_str() << endl;
	  #endif
      return 1;
    }
	currentFilePath=filePath;
  }
  else if(mode == "read")/////////////////////////////////////////////////////////////////// If mode is "read" 
  {
    #ifdef DEBUG_FLAG_comIO
	  cout << "Reading file: " << currentFilePath << endl;
	#endif
    if(dataStream.is_open() && buf != NULL)
	{
      dataStream.seekp (startIndex);
      dataStream.read (buf , bufSize);
	}
	else
	{
	  #ifdef DEBUG_FLAG_comIO
	    cerr << "Error reading file: " << currentFilePath << " No buffer or no file.\n";
	  #endif
	  return 1;
	}
  }
  else if(mode == "write")///////////////////////////////////////////////////////////////// If mode is "write"
  {
    #ifdef DEBUG_FLAG_comIO
	  cout << "Writing file: " << currentFilePath << endl;
	#endif
    if(dataStream.is_open() && buf != NULL)
	{
      dataStream.seekp (startIndex);
      dataStream.write (buf , bufSize);
	}
	else
    {
	  #ifdef DEBUG_FLAG_comIO
	    cout << "Error writing file: " << currentFilePath << " No buffer or no file.\n";
	  #endif
	  return 1;
	}
  }
  else ///////////////////////////////////////////////////////////////////////////////////// If mode is "close" or unknown
  {
    #ifdef DEBUG_FLAG_comIO
	  cout << "Closing file: " << currentFilePath << endl;
	#endif
    dataStream.close();
	currentFilePath="None";
	return 0;
  }
  
  return 0;
}// end funct comIO



////////////////////////////////////////////////////////////
// Demonstration code for comIO function
// Must have files poopIn.txt and poopOut.txt in the local dir.
// Populate poopIn with some text data. That will be read, operated on, and written into poopOut.
// Note that in this example code I do not check the return values for errors. In actual use this should
// be done so errors can be detected and handled properly.
// This was tested using cygwin, but should work in any environment. Windows or linux.


int main(){                              // main prog to demonstrate comIO
  const int bufSize=1000;
  unsigned dataSize = 0;
  string myFile1="comIn.txt";
  string myFile2="comOut.txt";

  char* buf = (char*) malloc(bufSize);    // Create IO Buffer
  memset(buf,'\0',bufSize);               // Purge buffer of garbage
  
  comIO(myFile1, "open");                 // open myFile 1
  comIO( myFile1, "read" ,buf, bufSize);  // read from myFile1
  comIO(myFile1,"close");                 // Close myFile 1 
  dataSize = strlen(buf);                 // Save size of the ascii data read from the file.
  
  comIO(myFile2,"open");                  // open a different file.
  

  string header="<dst><src><flags>PacketData:";
  string ender=":EOF";
  int headerSize= strlen(header.c_str() );
  int enderSize = strlen(ender.c_str()  );

  for(int k= 0; k <= dataSize; k++) buf[dataSize+headerSize-k] = buf[dataSize-k];  // encapsulate date between header and footer.
  for(int p= 0; p < headerSize; p++) buf[p] = header[p];
  for(int b= 0; b < enderSize; b++) buf[dataSize+headerSize+b] = ender[b];
  
  comIO(myFile2,"write",buf,headerSize+dataSize+enderSize);                         // write data to file.
  
  char userData[256]={'\0'};
  cout << "Enter some text.\n";
  cin.getline( userData,256 );                                                      // Read a line of data from the user 
  
  comIO(myFile2,"write",userData, strlen(userData), headerSize+dataSize+enderSize); // write data to file with starting position after previous data.
  
  comIO( myFile2, "read" ,buf, bufSize);                                            // read from myFile
  cout << buf << endl;                                                              // display contents read from file.
  
  comIO(myFile2,"close");                                                           // close file.
  
  
  free(buf);                                                                        // free buffer
  
  return 0;
}// end main



