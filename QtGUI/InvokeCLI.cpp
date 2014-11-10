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
    //if()
    //{
    //    return true;
    //}
    return true;
  }

  void InvokeCLI::startSession()
  {
    //process = new QProcess(this);
    //process->start(program);
    //sessionID = ;
  }

  void InvokeCLI::refreshSession()
  {

  }

  void InvokeCLI::closeSession()
  {
    process->close();
  }

  bool InvokeCLI::discoverDevice()
  {
    //if()
     //   return true;

    return true;
  }

  void InvokeCLI::encryptFiles(const QStringList& files, String cipherMode)
  {
    /*
      eggbeater-cli --cipher-mode <$cipherMode> --encrypt --session-id <$this->sessionID> <$files ...>
    */
    for(int i=0; i<files.size(); i++)
    {
        //Encrypt all files user selected
    }
  }

  void InvokeCLI::decryptFiles(const QStringList& files, String cipherMode)
  {
    for(int i=0; i<files.size(); i++)
    {
        //decypt all files user selected
    }
  }
}
