#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
//---------------------------
#include <stdint.h>
#include <string>
#include <algorithm>

#include <cryptlib.h>
using CryptoPP::Exception;
 
#include <files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;
 
#include <hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder; 
 
#include <filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::FileSink;
using CryptoPP::FileSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;
 
#include <modes.h>
using CryptoPP::CBC_Mode;
 
#include <gcm.h>
using CryptoPP::GCM;
 
#include <aes.h>
using CryptoPP::AES;
 
#include <modes.h>
using CryptoPP::OFB_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::Exception;
//-------------------------------------------
#include <eggbeater/Options.h>
#include <eggbeater/Crypto.h>
/*
  struct Status_t
  {
  }; // */


/*******************************************************************************
                     This class is not yet finalized
*******************************************************************************/
namespace EggBeater
{
  class Control
  {
  public:
    Control(Options& opt); // or whatever the class is named
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
    StringList devList;          // List of connected devices.
    //Status_t    controlStatus;
    ByteArray key;
    ByteArray iv;
    
    char tmpFilePath[120]={'\0'};
    //! Internal function to start a new session
    void newSession();
    //! Internal function to open a session with an existing fingerprint
    void openSession();
    //! Internal function to refresh the session timer
    void refreshSession();
    //! Internal function to close the session
    void closeSession();
    //! Internal Function to get initialization vector (IV)
    int getIV(ByteArray &iv);
    //! Internal function to get key value from st board.
    int getKey(ByteArray &key);
    
    
    //! Internal function to encrypt the specified files
    int encryptFiles(std::string, std::string, std::vector<uint8_t>, std::vector<uint8_t>);
    //! Internal function to decrypt the specified files
    int decryptFiles(std::string, std::string, std::vector<uint8_t>, std::vector<uint8_t>);
	
	  int writeVec( std::vector<std::string> &lines, std::string targetFile);
    int addMsg(std::vector<std::string> &vec, std::string arg1, std::string arg2 = "\0" );
    int addMsg(std::vector<std::string> &vec, std::string arg1, int arg2 );
	  int addMsg(std::vector<std::string> &vec, Status_t status);
  };
}

#endif // _EGGBEATER_CONTROL_H_
