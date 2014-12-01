#include "InvokeCLI.h"

namespace EggBeater
{
  InvokeCLI::InvokeCLI() 
  : sessionID(0), proc(NULL)
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
    //needs to get session ID
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

  void InvokeCLI::encryptFiles(QObject* curr, QStringList fileNames, QString folderName)
  {
    /*
     * getOpt.exe -a encrypt -f file1 -f file2 -f file3 -s 0056 -f file4 -o option1 -o option2
     */
    proc = new QProcess(curr);
    QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOpt/getOpt.exe";
    QStringList attributes;
    attributes << "-a" << "encrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    attributes << "--session-id" << "0056";
    attributes << "-f" << folderName;
    attributes << "-o" << "option1";
    attributes  << "-o" << "option2";
    proc->start(program, attributes);
    if(!proc->waitForFinished())
        qDebug() << "Fail:\n"<< proc->errorString();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
  }

  void InvokeCLI::decryptFiles(QObject* curr, QStringList fileNames, QString folderName, QString cipherMode)
  {
   /*
    * getOpt.exe --decrypt -f file1 -f file2 -f file3 --session-id 70056 -f file4 -o option1 --cipher-mode ofb
    */

    proc = new QProcess(curr);
    QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptv2/getOpt.exe";
    QStringList attributes;
    attributes << "--decrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    attributes << "--session-id" << "70056";
    attributes << "-f" << folderName;
    attributes << "-o" << "option1";
    attributes  << "--cipher-mode" << cipherMode;
    proc->start(program, attributes);
    if(!proc->waitForFinished())
        qDebug() << "Fail:\n"<< proc->errorString();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
  }
}
