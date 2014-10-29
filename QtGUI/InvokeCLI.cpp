#include "InvokeCLI.h"

namespace EggBeater
{
  InvokeCLI::InvokeCLI() : sessionID(0)
  {
  }
  
  InvokeCLI::~InvokeCLI()
  {
  }
  
  bool InvokeCLI::sessionIsOpen() const
  {
    return false;
  }
  
  void InvokeCLI::startSession()
  {
  }
  
  void InvokeCLI::refreshSession()
  {
  }
  
  void InvokeCLI::closeSession()
  {
  }
  
  bool InvokeCLI::discoverDevice()
  {
    return false;
  }
  
  void InvokeCLI::encryptFiles(const StringList& files, String cipherMode)
  {
    /*
      eggbeater-cli --cipher-mode <$cipherMode> --encrypt --session-id <$this->sessionID> <$files ...>
    */
  }
  
  void InvokeCLI::decryptFiles(const StringList& files, String cipherMode)
  {
  }  
}
