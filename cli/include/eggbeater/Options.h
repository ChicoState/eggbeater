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
             OverallFilesTotal,
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
    //! Get the list of specified files/folders
    const StringList& getFileList() const;
    //! Get the specified action
    CLI_Action getAction() const;
    //! Check if a session ID was specified
    bool hasSessionID() const;
    //! Get the specified session ID
    String getSessionID() const;

    //! Check if a cipher mode was specified
    bool hasCipherMode() const;
    //! Get the specified cipher mode
    CipherMode getCipherMode() const;
    
    //! @todo This should go in the control class
    //! Check if any errors have occurred.
    bool haveErrors() const;
    //! Check if any fatal errors have occurred.
    bool haveFatalError() const;
    //! Get the list of errors
    const ErrorList& getErrors() const;
    //! Get the current status
    Status_t getCurrentStatus() const;
    //! Add an error to the error list
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
    char tmpFilePath[120]={'\0'};
  };
}

#endif // _EGGBEATER_OPTIONS_H_
