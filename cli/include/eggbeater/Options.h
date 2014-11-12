#ifndef _EGGBEATER_OPTIONS_H_
#define _EGGBEATER_OPTIONS_H_

#include <eggbeater/Common.h>
#include <eggbeater/Crypto.h>
//#include <list>

namespace EggBeater
{
  enum class CLI_Action : uint8_t
  {
    None            = 0,
    StartSession    = 1,
    RefreshSession  = 2,
    CloseSession    = 3,
    Encrypt         = 4,
    Decrypt         = 5,
    DiscoverDevice  = 6
  };
  
  enum class ErrorClass : uint8_t
  {
    None        = 0,
    Info        = 1,
    Warning     = 2,
    Error       = 3,
    FatalError  = 4
  };
  
  struct Status_t
  {
    uint32_t CurrentBlocksDone,
             CurrentBlocksTotal,
             OverallFilesDone,
             OverallBlocksDone,
             OverallBlocksTotal;
    std::string CurrentPath;
  };
  
  struct Error_t
  {
    String      Message;
    ErrorClass  Class;
  };
  
  typedef std::list<Error_t> ErrorList;
  
  class Options
  {
  public:
    Options();
    ~Options();
    
    //! Parse the command line arguments for the action, options and files
    bool parseOptions(int argc, const char** argv);
    //! Check if any errors have occurred.
    bool haveErrors() const;
    //! Check if any fatal errors have occurred.
    bool haveFatalError() const;
    
    const StringList& getFileList() const;
    
    CLI_Action getAction() const;
    
    bool hasSessionID() const;
    String getSessionID() const;
    
    bool hasCipherMode() const;
    CipherMode getCipherMode() const;
    
    const ErrorList& getErrors() const;
    
    Status_t getCurrentStatus() const;
    
    void addError(ErrorClass, String);
    
  private:
    static bool ParseCLIOptions(Options*, int, const char**);
    static int  ParseStartSession(Options*, int, int, const char**);
    static int  ParseRefreshSession(Options*, int, int, const char**);
    static int  ParseCloseSession(Options*, int, int, const char**);
    static int  ParseEncrypt(Options*, int, int, const char**);
    static int  ParseDecrypt(Options*, int, int, const char**);
    static int  ParseSessionID(Options*, int, int, const char**);
    static int  ParseCipherMode(Options*, int, int, const char**);
    static int  ParseFileEntry(Options*, int, int, const char**);
    static int  ParseDiscoverDevice(Options*, int, int, const char**);
  
    String      sessionID;
    CLI_Action  cliAction;
    CipherMode  cipherMode;
    Status_t    currentStatus;
    ErrorList   errorList;
    StringList  fileList;
  };
  
  //! @todo Integrate Sterling's getopt code
}

#endif // _EGGBEATER_OPTIONS_H_
