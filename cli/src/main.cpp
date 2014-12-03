#include <eggbeater/Serial.h>
#include <eggbeater/Control.h>
#include <eggbeater/Crypto.h>
#include <eggbeater/Discovery.h>
#include <eggbeater/Options.h>

using namespace EggBeater;

int main(int argc, const char** argv)
{
  //! @todo Add Control object when its done
  
  Options opt;
  
  
  // Parse the passed command line args
  if (!opt.parseOptions(argc, argv))
  {
    // There was a problem with the arguments
    
    for (auto iter: opt.getErrors())
    {
      switch (iter.Class)
      {
        default:
        case ErrorClass::None:
          break;
        
        case ErrorClass::Info:
          std::cout << "[INFO] " << iter.Message << std::endl;
          break;
        
        case ErrorClass::Warning:
          std::cout << "[WARN] " << iter.Message << std::endl;
          break;
        
        case ErrorClass::Error:
          std::cout << "[ERROR] " << iter.Message << std::endl;
          break;
        
        case ErrorClass::FatalError:
          std::cout << "[FATAL] " << iter.Message << std::endl;
          break;
      }
    }
    
    if (opt.haveFatalError())
      return 1;
  }
  
  Control ctrl;                                  // This must be instantiated after options have been parsed.
  switch (opt.getAction())
  {
    default:
    case CLI_Action::None:
      std::cout << "No action specified" << std::endl;
      break;
    
    case CLI_Action::StartSession:
      std::cout << "start session" << std::endl;
      ctrl.newSession(opt)                        // Call ctrl function to start new session.
      break;
    
    case CLI_Action::RefreshSession:
      std::cout << "refresh session" << std::endl;
      break;
    
    case CLI_Action::CloseSession:
      std::cout << "close session" << std::endl;
      break;
    
    case CLI_Action::Encrypt:
      std::cout << "encrypt file" << std::endl;
      break;
    
    case CLI_Action::Decrypt:
      std::cout << "decrypt files" << std::endl;
      break;
    
    case CLI_Action::DiscoverDevice:
      std::cout << "discover device" << std::endl;
      break;
  }
  
  if (opt.hasSessionID())
    std::cout << "Session ID: " << opt.getSessionID() << std::endl;
  else
    std::cout << "Session ID: not set" << std::endl;
  
  if (opt.hasCipherMode())
    std::cout << "Cipher mode: " << (int)opt.getCipherMode() << std::endl;
  else
    std::cout << "Cipher mode: not set" << std::endl;
  
  std::cout << "Specified files:" << std::endl;
  
  for (auto f : opt.getFileList())
  {
    std::cout << "  " << f << std::endl;
  }
  
  
  return 0;
}