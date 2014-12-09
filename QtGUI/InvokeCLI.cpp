#include "InvokeCLI.h"

using namespace EggBeater;

namespace EggBeater
{
  InvokeCLI::InvokeCLI()
  : sessionID(0)
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

  void InvokeCLI::startSession(QProcess* proc)
  {
    //needs to get session ID
      QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
      QStringList attributes;
      attributes << "--start-session";
      proc->start(program, attributes);
      if(!proc->waitForFinished())
          qDebug() << "Fail:\n"<< proc->errorString();
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
  }

  void InvokeCLI::refreshSession(QProcess* proc)
  {
      QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
      QStringList attributes;
      attributes << "--refresh-session";
      proc->start(program, attributes);
      if(!proc->waitForFinished())
          qDebug() << "Fail:\n"<< proc->errorString();
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
  }

  void InvokeCLI::closeSession(QProcess* proc)
  {
      QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
      QStringList attributes;
      attributes << "--close-session";
      proc->start(program, attributes);
      if(!proc->waitForFinished())
          qDebug() << "Fail:\n"<< proc->errorString();
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
  }

  InvokeCLI::parse InvokeCLI::fileParse(SecDialog* curr)
  {
      QString temp =  QDir::tempPath()+"/TempComm.cpp"; ; //"C:\Users\sam\AppData\Local\TempComm.cpp" //System::GetTempPath()
      qint64 i=0;
      qint64 progresscount=0;
      // qint64 curBlock=0;
      qint64 positionTrack=0;
      QString curFileCount=0;
      QString maxCount=0;
      QFile file(temp);
      parse retvals;
      retvals.progresscount=0;
      retvals.sessionID=0;
      retvals.errormessage="";

      // The file stuff should go in its own function/class

      if(!file.open(QFile::ReadOnly))
      {
          QMessageBox::warning(curr, "Application", file.fileName(), "ok"); //"Cant find temp file \n"
          retvals.progresscount=-1;
          retvals.sessionID=-1;
          retvals.errormessage="Can't find temp file.";
          return retvals;
      }
      QTextStream in(&file);

      while(!in.atEnd())
      {
          QString line=in.readLine();
          QString::iterator it= line.begin();
          QVector<char> curFileName;
          QString word=" ";
		  QString firstWord = line.split(" ").at(0);
		  if(firstWord=="^!error")
		  {
		    //read error message
            for(int i=1; i<line.split(" ").size(); i++)
			{
              if(retvals.errormessage=="")
                retvals.errormessage=line.split(" ").at(i);
              else
                retvals.errormessage=(retvals.errormessage)+" "+line.split(" ").at(i);
			}
			
		  }
		  else if(firstWord=="^!fatal")
		  {
		    //read error message and stop
            for(int i=1; i<line.split(" ").size(); i++)
			{
			  if(retvals.errormessage==NULL)
			    retvals.errormessage=line.split(" ").at(1);
			  retvals.errormessage=(retvals.errormessage)+line.split(" ").at(i);  
			}
            retvals.sessionID=-1;
		  }
          else if(firstWord=="sessionID")
		  {
            retvals.sessionID=(line.split(" ").at(1)).toInt();
		  }
		  else if(firstWord=="^!done")
		  {
		    //done with task ack
		  }
		  else if(firstWord=="refresh")
		  {
		    //retval.refresh=line.split(" ").at(1);
		  }
		  else if(firstWord=="discovered")
		  {
		    //retval.discover=line.split(" ").at(1);
		  }
		  else if(firstWord=="close")
		  {
		    //retval.close=line.split(" ").at(1);
		  }
		  else if(firstWord=="status")
		  {
			  QString curFile = line.split(" ").at(1);
			 // QString upCarrot = line.split(" ").at(2);
			  QString totalFile = line.split(" ").at(3);

			  int curC=curFile.toInt();
			  int maxC=totalFile.toInt();
			  retvals.progresscount=100*curC/maxC;
		  }          
      }
	  file.close();
	  return retvals;
  }

  void InvokeCLI::progressBarPopUp(SecDialog* curr)
  {
      /****Start of progress bar update*******/
      parse retvals;
      retvals=fileParse(curr);
      curr->pd->setValue(retvals.progresscount);

      //use message box to test values
      //QMessageBox::warning(this, progress, curFileCount, maxCount); //tr("Cant find temp file \n")
  }

  bool InvokeCLI::discoverDevice(QProcess* proc)
  {
      QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
      QStringList attributes;
      attributes << "--close-session";
      proc->start(program, attributes);
      if(!proc->waitForFinished())
          qDebug() << "Fail:\n"<< proc->errorString();
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
      return true;
  }

  void InvokeCLI::encryptFiles(QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc)
  {
    /*
     * getOpt.exe --encrypt -f file1 -f file2 -f file3 --session-id 70056 -fd someDrive/someFolder1/ -o option1 --cipher-mode ofb
     */
    QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
    QStringList attributes;
    attributes << "--encrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    attributes << "--session-id" << "70056";
    attributes << "-fd" << folderName;
    if(cipherMode!="cfb" && cipherMode!="ofb" && cipherMode!="gcm")
        cipherMode = "cfb";
    attributes  << "--cipher-mode" << cipherMode;
    //attributes << "-o" << "option1";
    //attributes  << "-o" << "option2";
    proc->start(program, attributes);
    if(!proc->waitForFinished())
        qDebug() << "Fail:\n"<< proc->errorString();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
    proc->close();
}

  void InvokeCLI::decryptFiles(QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc)
  {
   /*
    * getOpt.exe --decrypt -f file1 -f file2 -f file3 --session-id 70056 -fd someDrive/someFolder1/ -o option1 --cipher-mode ofb
    */
    QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOptV2/getOpt.exe";
    QStringList attributes;
    attributes << "--decrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    attributes << "--session-id" << "70056";
    attributes << "-fd" << folderName;
    if(cipherMode!="cfb" && cipherMode!="ofb" && cipherMode!="gcm")
        cipherMode = "cfb";
    attributes  << "--cipher-mode" << cipherMode;
    //attributes << "-o" << "option1";
    //attributes  << "-o" << "option2";
    proc->start(program, attributes);
    if(!proc->waitForFinished())
        qDebug() << "Fail:\n"<< proc->errorString();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
    proc->close();
  }
}
