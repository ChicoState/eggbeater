////////////////////////////////////////////////////////////
// class getOpt cpp file.

#include "getOpt.h"

////////////////////////////////////////////////////////////
// getOpt constructor extracts all the data types via member functions.
// Use the get functions to access the data that has been extracted.

getOpt::getOpt(int argc, char *argv[]){
  errorLevel=0;
  extractSession(argc, argv);
  extractAction(argc, argv);
  extractFiles(argc, argv);
  extractOptions(argc, argv);
  extractCipher(argc, argv);
  errorChecks();               // After all data extracted, do global level error checking. Aka errors between extraction functions, not inside them.
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
	    return tmpFileCount;
      }
	  #ifdef DEBUG_FLAG_getOpt
	    cout << "pass\n";
	  #endif
	  files.push_back( argv[count+1] );
	  tmpFileCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -fi
	#ifdef DEBUG_FLAG_getOpt
      else 
	    cout << "fail\n";
	#endif
  }// end for.
  return tmpFileCount;
} // end extract files


////////////////////////////////////////////////////////////
// Member function to extract the session ID from the cmd line args
// This code does not look for multiple session ID's.

int getOpt::extractSession(int argc, char *argv[]){
  string trigger="--session-id";
  sessionID = "none";
  for(int count = 1; count < argc; count++){
    if(argv[count] == trigger ){                  
	  sessionID = argv[count+1];
	  return 1;                     // Return true if found.
	}// end if trigger.
  }// end for.
  pushError("Error: No session ID passed to getOpt.");
  errorLevel += 5;
  return 0;                         // Return false if not found.
} // end extract session ID

////////////////////////////////////////////////////////////
// Member function to extract the cipher from the cmd line args
// cipher must be lower case, or it will throw an error.

int getOpt::extractCipher(int argc, char *argv[]){
  string trigger="--cipher-mode";
  cipher = "none";
  for(int count = 1; count < argc; count++){
    if(argv[count] == trigger ){                  
	  cipher = argv[count+1];
	  if(cipher != "ofb" && cipher != "cfb" && cipher != "gcm"){         // If cipher is not valid record the error.
	    pushError("Error: Invalid cipher: ", cipher);
		errorLevel += 5;
	  }// end if invalid.
	  return 1;                     // Return true if found.
	}// end if trigger.
  }// end for.
  return 0;                         // Return false if not found.
} // end extract cipher.


////////////////////////////////////////////////////////////
// Member function to extract the actions specified in the cmd line args
// Should be only one given at a time. For example encrypt, decrypt, authenticate, etc...
// "-a" signals an action being specified.

int getOpt::extractAction(int argc, char *argv[]){
  const short triggerCount=6;                          // triggerCount must be the total number of trigger values.
  string trigger[triggerCount]={"--encrypt","--decrypt","--start-session","--refresh-session","--close-session","--discover"}; // List of possible actions
  action="\0";
  
  for(int trgtIndex=0;trgtIndex < triggerCount; trgtIndex++){
    for(int count = 1; count < argc; count++){
      if(argv[count] == trigger[trgtIndex] ){                
	    action=trigger[trgtIndex];
        return 1;
      }// end if trigger.
    }// end inner for loop.
  }// end outer for loop.
  pushError("Error: No action passed to getOpt.");
  errorLevel+=5;
  return 0;
} // end extract Action


////////////////////////////////////////////////////////////
// Member function to extract options from the cmd line args.
// These options will depend on what action is being performed.
// "-o" signals an option is being specified.

int getOpt::extractOptions(int argc, char *argv[]){
  string trigger="-o";
  unsigned tmpOptionsCount = 0;
  
  for(int count = 1; count < argc; count++){
    if(argv[count] == trigger ){            
	  if( count + 1 > argc ){
	    return tmpOptionsCount;
      }
	  options.push_back( argv[count+1] );
	  tmpOptionsCount++;
	  count += 1; // Only increment by one since for loop will also add one, thus skipping over the last copied value.
	}// end if -f.
  }// end for.
  
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
// Member function to access cipher data.

string getOpt::getAction(){
  return action;
}// end get file path.


////////////////////////////////////////////////////////////
// Member function to access actions data.

string getOpt::getSessionID(){
  return sessionID;
}// end get file path.

////////////////////////////////////////////////////////////
// Member function to access actions data.

string getOpt::getCipher(){
  return cipher;
}// end get file path.

////////////////////////////////////////////////////////////
// Member functions to deal with errors.

void getOpt::pushError(string error1,string error2){
  errors.push_back(error1 + error2);
}// end pushError.

unsigned getOpt::getNumErrors(){
  return errors.size();
}// end getNumErrors.

string getOpt::getError(unsigned index){
  if(index >= errors.size()) return "\0";
  else return errors.at(index);
}// end getError.

int getOpt::getErrorLevel(){
  return errorLevel;
}// end getErrorLevel.

int getOpt::errorChecks(){
  // do conditional error checking in here.
  // For example if the Action was decrypt or encrypt, there must be a cipher given.
  // But for other actions cipher is not needed. 
  // Low level error checking is done in the extract functions, for validity of data.
  // Code here should only look at errors that span multiple extract functions or
  // require global knowledge to detect as in the example above.
  
return 0;
}// end getOptErrorChecks,








