#include "comIO.h"

////////////////////////////////////////////////////////////
// Constructor for comIO.
// Simply initializes the two variables.

comIO::comIO(){ 
  currentFilepath = "\0";
  //dataStream;
           };

////////////////////////////////////////////////////////////
// member function to open file.
// If a different file is already open, the old file is closed and the new one is opened.
// If the file is already open, nothing is done, the function returns success.
// If the file can not be opened a 1 is returned for failure.
		   
int comIO::open(string filePath){

	#ifdef DEBUG_FLAG_comIO
	  cout << "Opening file: " << filePath << endl;
	#endif
    if( dataStream.is_open() && currentFilepath != filePath)                     // If someone tries to open a new file while old one is still open.
	{
	  #ifdef DEBUG_FLAG_comIO
	    cerr << "A file was already open: " << currentFilepath <<" Closing, then opening: "<< filePath << endl;
	  #endif
	  dataStream.close(); 
      currentFilepath = "\0";	  
	}
	else if(dataStream.is_open() && currentFilepath == filePath)                  // If the specified file is already open.
	{
      #ifdef DEBUG_FLAG_comIO
	    cerr << "This file was already open. Doing nothing.\n";
	  #endif
	  return 0;
	}

	                                                                              // Open the dataStream and check for success.
    dataStream.open( filePath.c_str(), ios::in | ios::out | ios::binary);
    if(!dataStream)                                                               // if file failed to open. 
	{ 
	  #ifdef DEBUG_FLAG_comIO
        cerr << "Error: File could not be opened: " << filePath.c_str() << endl;
	  #endif
      return 1;
    }
	currentFilepath = filePath;
	
    return 0;
  }// end open function.

////////////////////////////////////////////////////////////
// member function to test if file descriptor is still alive.

int comIO::test(string filePath){
                                                                                   // test if fd still exists.
  if( dataStream.is_open() && filePath == currentFilepath) return 1;               // file path matches and is open.
  else if( filePath != currentFilepath ) return -1;                                // not the file that is open.
  else return 0;                                                                   // file not open.
}// end constructor.

////////////////////////////////////////////////////////////
// member function to write to file.
// bufSize is the number of bytes to write.
// startIndex is the position to begin writing from.

unsigned comIO::write( char* buf, unsigned bufSize, unsigned startIndex){
    #ifdef DEBUG_FLAG_comIO
	cout << "Writing file: " << currentFilepath << endl;
	#endif
	
    if(dataStream.is_open() && buf != NULL)
	{
      dataStream.seekp (startIndex);
      dataStream.write (buf , bufSize);
	}
	else
    {
	  #ifdef DEBUG_FLAG_comIO
	    cout << "Error writing file: " << currentFilepath << " No buffer or no file.\n";
	  #endif
	  return 1;
	}
	return 0;
}// end send.

////////////////////////////////////////////////////////////
// member function to read data from file.
// bufSize is the number of bytes to read. Must be <= actual size of buf
// startIndex is the position in the file you wish to begin reading from.

unsigned comIO::read( char* buf, unsigned bufSize, unsigned startIndex){

    #ifdef DEBUG_FLAG_comIO
	cout << "Reading file: " << currentFilepath << endl;
	#endif
	
    if(dataStream.is_open() && buf != NULL)
	{
      dataStream.seekp (startIndex);
      dataStream.read (buf , bufSize);
	}
	else
	{
	  #ifdef DEBUG_FLAG_comIO
	    cerr << "Error reading file: " << currentFilepath << " No buffer or no file.\n";
	  #endif
	  return 1;
	}
	
    return 0;

}// end read.

////////////////////////////////////////////////////////////
// Member function to close file.

int comIO::close(){

    #ifdef DEBUG_FLAG_comIO
	cout << "Closing file: " << currentFilepath << endl;
	#endif
    dataStream.close();
	currentFilepath="\0";
	return 0;

}// end close funct



