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

#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
 
#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;
 
#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder; 
 
#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::FileSink;
using CryptoPP::FileSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;
 
#include <cryptopp/modes.h>
using CryptoPP::CBC_Mode;
 
#include <cryptopp/gcm.h>
using CryptoPP::GCM;
 
#include <cryptopp/aes.h>
using CryptoPP::AES;
 
#include <cryptopp/modes.h>
using CryptoPP::OFB_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::Exception;
//-------------------------------------------
#include <eggbeater/Options.h>

  struct Status_t
  {
    uint32_t CurrentBlocksDone,
             CurrentBlocksTotal,
             OverallFilesDone,
             OverallFilesTotal,
             OverallBlocksDone,
             OverallBlocksTotal;
    std::string CurrentPath;
  };

/*******************************************************************************
                     This class is not yet finalized
*******************************************************************************/
namespace EggBeater
{
  class Control
  {
  public:
    Control(Options opt); // or whatever the class is named
    
    bool run(void);
    
    String getStatus();
  private:
  
    vector<string> fileVec; // Hold the lines to write to the output file. Sending data to GUI.
    String      sessionID;
    CLI_Action  cliAction;
    CipherMode  cipherMode;
    Status_t    currentStatus;
    ErrorList   errorList;
    StringList  fileList;
    Status_t    controlStatus;
    byte key[CryptoPP::AES::MAX_KEYLENGTH];
    byte iv[ CryptoPP::AES::BLOCKSIZE ];
    
    char tmpFilePath[120]={'\0'};
    //! Internal function to start a new session
    void newSession();
    //! Internal function to open a session with an existing fingerprint
    void openSession();
    //! Internal function to refresh the session timer
    void refreshSession();
    //! Internal function to close the session
    void closeSession();
    
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
