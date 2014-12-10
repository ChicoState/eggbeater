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

//! @todo Change the D_RUN calls into .addError calls

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
  
  bool Options::haveErrors() const
  {
    for (auto iter : errorList)
      if (iter.Class == ErrorClass::FatalError || iter.Class == ErrorClass::Error)
        return true;
      
    return false;
  }
  
  bool Options::haveFatalError() const
  {
    for (auto iter : errorList)
      if (iter.Class == ErrorClass::FatalError)
        return true;
    
    return false;
  }
  const ErrorList& Options::getErrors() const
  {
    return errorList;
  }
  
  Status_t Options::getCurrentStatus() const
  {
    return currentStatus;
  }
  
  void Options::addError(ErrorClass errClass, String str)
  {
    Error_t err;
    err.Class = errClass;
    err.Message = str;
    
    this->errorList.push_back(err);
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
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::StartSession;
    return 0;
  }
  
  int Options::ParseRefreshSession(Options* _this, int i, int argc, const char** argv)
  {
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::RefreshSession;
    return 0;
  }
  
  int Options::ParseCloseSession(Options* _this, int i, int argc, const char** argv)
  {
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::CloseSession;
    return 0;
  }
  
  int Options::ParseEncrypt(Options* _this, int i, int argc, const char** argv)
  {
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::Encrypt;
    return 0;
  }
  
  int Options::ParseDecrypt(Options* _this, int i, int argc, const char** argv)
  {
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::Decrypt;
    return 0;
  }
  
  int Options::ParseDiscoverDevice(Options* _this, int i, int argc, const char** argv)
  {
    if (_this->cliAction != CLI_Action::None)
    {
      _this->addError(ErrorClass::FatalError, "CLI Action already set");
      return -1;
    }
    
    _this->cliAction = CLI_Action::DiscoverDevice;
    return 0;
  }
  
  int Options::ParseSessionID(Options* _this, int i, int argc, const char** argv)
  {
    if ((i + 1) >= argc)
    {
      _this->addError(ErrorClass::FatalError, "--session-id: missing argument");
      return -1;
    }
    
    _this->sessionID = argv[i + 1];
    return 1;
  }
  
  int Options::ParseCipherMode(Options* _this, int i, int argc, const char** argv)
  {
    String cipherMode;
    
    if ((i + 1) >= argc)
    {
      _this->addError(ErrorClass::FatalError, "--cipher-mode: missing argument");
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
      _this->addError(ErrorClass::FatalError, "--cipher-mode: invalid selection");
      return -1;
    }
    
    return 1;
  }
  
  int Options::ParseFileEntry(Options* _this, int i, int argc, const char** argv)
  {
    _this->fileList.push_back(argv[i]);
    
    return 0;
  }
}