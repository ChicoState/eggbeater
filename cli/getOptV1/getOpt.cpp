#include "getOpt.h"


////////////////////////////////////////////////////////////
// getOpt constructor extracts all the data types via member functions.
// Use the get functions to access the data that has been extracted.

getOpt::getOpt(int argc, char *argv[]){
  if( ! extractSession(argc, argv) )   cerr << "No session ID!\n" ;
  if( ! extractAction(argc, argv) ) cerr << "No action given!\n";
  if( ! extractFiles(argc, argv) )     cerr << "No files given!\n";
  if( ! extractOptions(argc, argv) )   cerr << "No options given!\n";
}

////////////////////////////////////////////////////////////
// Member function to extract file paths from the command line args
// Looks for "-f" to signal arg is a file path.
// Files are added to "files" vector.

int getOpt::extractFiles(int argc, char *argv[]){
  string trigger="-f";
  int count;
  unsigned tmpFileCount = 0;
  for(count = 1; count < argc; count++){
  #ifdef DEBUG_FLAG_getOpt
    cout << "testing: " << argv[count] << " vs: " << trigger << endl; 
  #endif
    if( argv[count] == trigger ){
	  if( count + 1 > argc ){
        numFiles = tmpFileCount;
	    return tmpFileCount;
      }
	  #ifdef DEBUG_FLAG_getOpt
	    cout << "pass\n";
	  #endif
	  files.push_back( argv[count+1] );
	  tmpFileCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -f
	#ifdef DEBUG_FLAG_getOpt
      else 
	    cout << "fail\n";
	#endif
  }// end for.
  numFiles = tmpFileCount;
  return tmpFileCount;
} // end extract files


////////////////////////////////////////////////////////////
// Member function to extract the session ID from the cmd line args
// These should only be one ID given, but reading all occurrences we can check for
// errors such as multiple session ID's given.
// "-s" signals the session ID.

int getOpt::extractSession(int argc, char *argv[]){
  string trigger="--session-id";
  int count;
  unsigned tmpSessionCount = 0;
  for(count = 1; count < argc; count++){
    if(argv[count] == trigger ){                  
	  if( count + 1 > argc ){
        numSessions = tmpSessionCount;
	    return tmpSessionCount;
      }
	  sessions.push_back( argv[count+1] );
	  tmpSessionCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -f.
  }// end for.
  numSessions = tmpSessionCount;
  return tmpSessionCount;
} // end extract session ID


////////////////////////////////////////////////////////////
// Member function to extract the actions specified in the cmd line args
// Should be only one given at a time. For example encrypt, decrypt, authenticate, etc...
// "-a" signals an action being specified.

int getOpt::extractAction(int argc, char *argv[]){
  string trigger="-a";
  int count;
  unsigned tmpActionCount = 0;
  for(count = 1; count < argc; count++){
    if(argv[count] == trigger ){                
	  if( count + 1 > argc ){
        numActions = tmpActionCount;
	    return tmpActionCount;
      }
	  actions.push_back( argv[count+1] );
	  tmpActionCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -f.
  }// end for.
  numActions = tmpActionCount;
  return tmpActionCount;

} // end extract Action


////////////////////////////////////////////////////////////
// Member function to extract options from the cmd line args.
// These options will depend on what action is being performed.
// "-o" signals an option is being specified.

int getOpt::extractOptions(int argc, char *argv[]){
  string trigger="-o";
  int count;
  unsigned tmpOptionsCount = 0;
  for(count = 1; count < argc; count++){
    if(argv[count] == trigger ){            
	  if( count + 1 > argc ){
        numOptions = tmpOptionsCount;
	    return tmpOptionsCount;
      }
	  options.push_back( argv[count+1] );
	  tmpOptionsCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -f.
  }// end for.
  numOptions = tmpOptionsCount;
  return tmpOptionsCount;
  
}// end extract options


////////////////////////////////////////////////////////////
// Member function to access file path data.

unsigned getOpt::getNumFiles(){
  return files.size();
}// end get numFiles

string getOpt::getFilePath(unsigned index){
  if( index >= files.size() ) return "\0";
  else return files.at(index);
}// end get file path.


////////////////////////////////////////////////////////////
// Member function to access options data.

unsigned getOpt::getNumOptions(){
  return options.size();
}// end get numOptions

string getOpt::getOption(unsigned index){
  if(index >= options.size()) return "\0";
  else return options.at(index);
}// end get file path.


////////////////////////////////////////////////////////////
// Member function to access actions data.

unsigned getOpt::getNumActions(){
  return actions.size();
}// end get numOptions

string getOpt::getAction(unsigned index){
  if(index >= actions.size()) return "\0";
  else return options.at(index);
}// end get file path.


////////////////////////////////////////////////////////////
// Member function to access actions data.

unsigned getOpt::getNumSessions(){
  return sessions.size();
}// end get numOptions

string getOpt::getSessionID(unsigned index){
  if(index >= sessions.size()) return "\0";
  else return sessions.at(index);
}// end get file path.
