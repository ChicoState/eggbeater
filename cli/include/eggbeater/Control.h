#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <eggbeater/Options.h>
#include <eggbeater/Crypto.h>
#include <eggbeater/Serial.h>

/*******************************************************************************
                     This class is not yet finalized
*******************************************************************************/
namespace EggBeater
{
  class Control
  {
  public:
    Control(Options opt); // or whatever the class is named
    ~Control();
    
    bool run(void);
    
    String getStatus();
  private:
  
    std::vector<std::string> fileVec; // Hold the lines to write to the output file. Sending data to GUI.
    String      sessionID;
    String      tmpFile;
    CLI_Action  cliAction;
    CipherMode  cipherMode;
    Status_t    currentStatus;   
    ErrorList   errorList;       // List of current errors.
    StringList  fileList;        // List of files to operate on.
    StringList  devList;          // List of connected devices.
    //Status_t    controlStatus;
    ByteArray key;
    ByteArray iv;
    
    //! Send a packet to the comm device at @param device
    //! @note This function does not read a response packet from the device
    bool sendPacket(String device, Packet& in);
    //! Send and receive a packet from the comm device at @param device
    bool sendPacket(String device, Packet& in, Packet& out);
    //! Internal function to start a new/existing session
    void newSession();
    //! Internal function to refresh the session timer
    void refreshSession();
    //! Internal function to close the session
    void closeSession();
    //! Internal Function to get initialization vector (IV)
    int getIV();
    //! Internal function to get key value from st board.
    int getKey();
    
    //! Internal function to encrypt the specified files
    int encryptFiles();
    //! Internal function to decrypt the specified files
    int decryptFiles();
	
	  int writeVec( std::vector<std::string> &lines, std::string targetFile);
    int addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 = "\0" );
    int addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 );
	  int addMsg(std::vector<std::string> &vec, Status_t status);
  };
}

#endif // _EGGBEATER_CONTROL_H_
