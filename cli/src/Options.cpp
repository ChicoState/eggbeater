#include <eggbeater/Options.h>
#include <map>
#include <functional>
#include <iostream>

//#define DEBUG 1

#if defined(DEBUG) && (DEBUG > 0)
  #define D_RUN(...) { __VA_ARGS__; }
#else
  #define D_RUN(...)
#endif

namespace EggBeater
{
  bool CipherMode_IsValid(CipherMode cm)
  {
    return cm != CipherMode::None;
  }

  Options::Options() : cliAction(CLI_Action::None),
                       cipherMode(CipherMode::None)
  {
    // No-op
  }
  
  Options::~Options()
  {
    // No-op
  }
  
  bool Options::parseOptions(int argc, const char** argv)
  {
    return ParseCLIOptions(this, argc, argv);
  }
  
  bool Options::haveErrors() const
  {
    return false;
  }
  
  bool Options::haveFatalError() const
  {
    return false;
  }
  
  const StringList& Options::getFileList() const
  {
    return fileList;
  }
  
  CLI_Action Options::getAction() const
  {
    return cliAction;
  }
  
  bool Options::hasSessionID() const
  {
    return sessionID != "";
  }
  
  String Options::getSessionID() const
  {
    return sessionID;
  }
  
  bool Options::hasCipherMode() const
  {
    return CipherMode_IsValid(cipherMode);
  }
  
  CipherMode Options::getCipherMode() const
  {
    return cipherMode;
  }
  
  const ErrorList& Options::getErrors() const
  {
    return errorList;
  }
  
  Status_t Options::getCurrentStatus() const
  {
    return currentStatus;
  }
  
  bool Options::ParseCLIOptions(Options* _this, int argc, const char** argv)
  {
    std::map<String, std::function<int(Options*, int, int, const char**)> > lut;
    lut["--start-session"] = &ParseStartSession;
    lut["--refresh-session"] = &ParseRefreshSession;
    lut["--close-session"] = &ParseCloseSession;
    lut["--encrypt"] = &ParseEncrypt;
    lut["--decrypt"] = &ParseDecrypt;
    lut["--session-id"] = &ParseSessionID;
    lut["--cipher-mode"] = &ParseCipherMode;
    lut["--discover-device"] = &ParseDiscoverDevice;
    
    for (int i = 1; i < argc; i++)
    {
      auto iter = lut.find(argv[i]);
      
      if (iter != lut.end())
      {
        int stride = iter->second(_this, i, argc, argv);
        
        D_RUN(std::cout << "stride: " << stride << std::endl);
        
        if (stride < 0)
          return false;
        
        i += stride;
      }
      else
      {
        ParseFileEntry(_this, i, argc, argv);
      }
    }
    
    return true;
  }
  
  int Options::ParseStartSession(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseStartSession" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::StartSession;
    return 0;
  }
  
  int Options::ParseRefreshSession(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseRefreshSession" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::RefreshSession;
    return 0;
  }
  
  int Options::ParseCloseSession(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseCloseSession" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::CloseSession;
    return 0;
  }
  
  int Options::ParseEncrypt(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseEncrypt" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::Encrypt;
    return 0;
  }
  
  int Options::ParseDecrypt(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseDecrypt" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::Decrypt;
    return 0;
  }
  
  int Options::ParseDiscoverDevice(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseDecrypt" << std::endl);
    
    if (_this->cliAction != CLI_Action::None)
    {
      D_RUN(std::cout << "CLI action already set" << std::endl);
      return -1;
    }
    
    _this->cliAction = CLI_Action::DiscoverDevice;
    return 0;
  }
  
  int Options::ParseSessionID(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseSessionID" << std::endl);
    
    if ((i + 1) >= argc)
    {
      D_RUN(std::cout << "(" << i << " + 1) >= " << argc << std::endl);
      return -1;
    }
    
    _this->sessionID = argv[i + 1];
    return 1;
  }
  
  int Options::ParseCipherMode(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseCipherMode" << std::endl);
    String cipherMode;
    
    if ((i + 1) >= argc)
    {
      D_RUN(std::cout << "(" << i << " + 1) >= " << argc << std::endl);
      return -1;
    }
    
    cipherMode = argv[i + 1];
    
    for (uint32_t j = 0; j < cipherMode.length(); j++)
      cipherMode[j] = tolower(cipherMode[j]);
    
    if (cipherMode == "cfb")
    {
      _this->cipherMode = CipherMode::CFB;
    }
    else if (cipherMode == "ofb")
    {
      _this->cipherMode = CipherMode::OFB;
    }
    else if (cipherMode == "gcm")
    {
      _this->cipherMode = CipherMode::GCM;
    }
    else
    {
      D_RUN(std::cout << "invalid cipher mode" << std::endl);
      return -1;
    }
    
    return 1;
  }
  
  int Options::ParseFileEntry(Options* _this, int i, int argc, const char** argv)
  {
    D_RUN(std::cout << "ParseFileEntry" << std::endl);
    
    _this->fileList.push_back(argv[i]);
    
    return 0;
  }
  
}