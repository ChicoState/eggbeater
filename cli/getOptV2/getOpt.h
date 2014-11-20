////////////////////////////////////////////////////////////
// getOpt header file.

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
//#define DEBUG_FLAG_getOpt // Enable this to see outputs from selected extraction functions.

using namespace std;

  class getOpt{
    protected:

      string sessionID;                    // Store session ID
      string action;                       // Store the first action found.
      string cipher;                       // Store the cypher mode if specified
      string destFolder;
      std::vector<std::string> files;      // Store list of files passed
      std::vector<std::string> options;    // Store list of passed options. This may not be used but gives us another 'channel' for extra options.
      std::vector<std::string> errors;     // store error messages.
      int errorLevel;                      // Store error level to differentiate between fatal and non fatal errors

    public:

      getOpt(int argc, char *argv[]);      // Constructor.
      ~getOpt(){                         
         };                                // Destructor.

      int extractFiles(int argc, char *argv[]);
      int extractSession(int argc, char *argv[]);
      int extractCipher(int argc, char *argv[]);
      int extractAction(int argc, char *argv[]);
      int extractOptions(int argc, char *argv[]);
      int extractDestFolder(int argc, char *argv[]);
      unsigned getNumFiles();
      unsigned getNumOptions();

      string getFilePath(unsigned index);
      string getAction();
      string getOption(unsigned index=0);
      string getSessionID();
      string getCipher();
      string getDestFolder();
      void pushError(string error1,string error2="\0");   // Add error message to list.
      unsigned getNumErrors();                            // Read the qty of errors.
      string getError(unsigned index);                    // Read an error message.
      int getErrorLevel();                                // Read the current error level. Need to agree on what level is fatal v non fatal.
	  int errorChecks();                                  // Do conditional/ global level error checking. See function for more notes.

}; // end getOpt header

// need to add code to handle file and folder paths with spaces in them. Will depend on how GUI passes them.
