#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <eggbeater/Options.h>

/*******************************************************************************
                     This class is not yet finalized
*******************************************************************************/
namespace EggBeater
{
  class Control
  {
  public:
    Control(Options optionsContainer); // or whatever the class is named
    
    bool run(void);
    
    String getStatus();
  private:
    //! Internal function to start a new session
    void newSession();
    //! Internal function to open a session with an existing fingerprint
    void openSession();
    //! Internal function to refresh the session timer
    void refreshSession();
    //! Internal function to close the session
    void closeSession();
    
    //! Internal function to encrypt the specified files
    void encryptFiles();
    //! Internal function to decrypt the specified files
    void decryptFiles();
	
	  int write( std::vector<std::string> &lines, std::string targetFile);
    int addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 = "\0" );
    int addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 );
	  int addMsg(std::vector<std::string> &vec, int fDone, int total, std::string path, int bDone, int bTotal, int bDoneSum, int bTotalSum );
  };
}

#endif // _EGGBEATER_CONTROL_H_
