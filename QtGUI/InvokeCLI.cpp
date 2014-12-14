#include "InvokeCLI.h"

using namespace EggBeater;

#define CLI_PATH "C:\\Stuff\\CSCI 430\\eggbeater\\cli\\build\\bin\\eggbeater.exe"
#define TEMP_FILE_NAME ( "C:\\Stuff\\status.txt" )
//( QDir::tempPath() + "/TempComm.cpp" )

namespace EggBeater
{
  InvokeCLI::InvokeCLI() : sessionID(-1)
  {
  }

  InvokeCLI::~InvokeCLI()
  {
  }

  bool InvokeCLI::sessionIsOpen() const
  {
    return sessionID < 20;
  }

  bool InvokeCLI::startSession(QWidget* curr, QProcess* proc2)
  {
      parse retvals;

      (void)(proc2);

      QString program = CLI_PATH;
      QStringList attributes;
      attributes << "--start-session";
      attributes << "-gui" << TEMP_FILE_NAME;
      QProcess* proc = new QProcess(curr);
      proc->start(program, attributes);
      if(!proc->waitForFinished())
      {
          qDebug() << "Fail:\n"<< proc->errorString();
      }
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }
      proc->close();
      retvals=fileParse(curr);

      if (retvals.sessionID >= 0)
      {
          this->sessionID = retvals.sessionID;
          return true;
      }
      else
      {
          return false;
      }
  }

  void InvokeCLI::refreshSession(SecDialog* curr, QProcess* proc)
  {
      parse retvals;
      retvals=fileParse(curr);

      QString program = CLI_PATH;
      QStringList attributes;
      attributes << "--refresh-session";
      attributes << "--session-id" << QString::number(sessionID);
      attributes << "-gui" << TEMP_FILE_NAME;
      proc->start(program, attributes);
      if(!proc->waitForFinished())
      {
          qDebug() << "Fail:\n"<< proc->errorString();
      }
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
  }

  void InvokeCLI::closeSession(SecDialog* curr, QProcess* proc)
  {
      parse retvals;
      retvals=fileParse(curr);

      QString program = CLI_PATH;
      QStringList attributes;
      attributes << "--close-session";
      attributes << "--session-id" << QString::number(sessionID);
      attributes << "-gui" << TEMP_FILE_NAME;
      proc->start(program, attributes);
      if(!proc->waitForFinished())
      {
          qDebug() << "Fail:\n"<< proc->errorString();
      }
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();
  }

  InvokeCLI::parse InvokeCLI::fileParse(QWidget* curr)
  {
      QString temp =  TEMP_FILE_NAME; ; //"C:\Users\sam\AppData\Local\TempComm.cpp" //System::GetTempPath()
      QString curFileCount=0;
      QString maxCount=0;
      QFile file(temp);
      parse retvals;
      retvals.progresscount=0;
      retvals.sessionID=0;
      retvals.errormessage="";
      retvals.done=0;
      retvals.discover=0;

      // The file stuff should go in its own function/class

      if(!file.open(QFile::ReadOnly))
      {
          //QMessageBox::warning(curr, "Application", file.fileName(), "ok"); //"Cant find temp file \n"
          retvals.progresscount=-1;
          retvals.sessionID=-1;
          retvals.errormessage="Can't find temp file.";
          return retvals;
      }
      QTextStream in(&file);

      while(!in.atEnd())
      {
          QString line=in.readLine();
          QStringList words = line.split(" ");
          QVector<char> curFileName;
          QString word=" ";
		  QString firstWord = line.split(" ").at(0);
		  if(firstWord=="^!error")
		  {
		    //read error message
            for(int i=1; i< words.size(); i++)
			{
              if(retvals.errormessage=="")
                retvals.errormessage=words.at(i);
              else
                retvals.errormessage=(retvals.errormessage)+" "+words.at(i);
			}
            retvals.errormessage=retvals.errormessage+"\n";
		  }
		  else if(firstWord=="^!fatal")
		  {
		    //read error message and stop
            for(int i=1; i<words.size(); i++)
			{
              if(retvals.errormessage=="" || i==1)
                retvals.errormessage="fatal: "+words.at(1);
              else
                retvals.errormessage=(retvals.errormessage)+" "+words.at(i);
			}
            retvals.errormessage=retvals.errormessage+"\n";
            retvals.done=-1;
		  }
          else if(firstWord=="sessionID")
		  {
            retvals.sessionID=(words.at(1)).toInt();
            //sessionID = retvals.sessionID;
		  }
		  else if(firstWord=="^!done")
		  {
		    //done with task ack
              retvals.done=1;
		  }
		  else if(firstWord=="refresh")
		  {
		    //retval.refresh=line.split(" ").at(1);
		  }
          else if(firstWord=="discover")
		  {
            retvals.discover=words.at(1).toInt();
		  }
		  else if(firstWord=="close")
		  {
		    //retval.close=line.split(" ").at(1);
		  }
		  else if(firstWord=="status")
		  {
              QString curFile = words.at(1);
			 // QString upCarrot = line.split(" ").at(2);
              QString totalFile = words.at(3);

              //! @todo How to handle file with space in name?
              retvals.currentFile = words.at(7);
//status 1 ^ 1 ^ 0 ^ <file> ^ 1 ^  0  ^  0
//0      1 2 3 4 5 6 7      8 9 10 11 12 13

			  int curC=curFile.toInt();
			  int maxC=totalFile.toInt();
			  retvals.progresscount=100*curC/maxC;
		  }
      }
	  file.close();
      if(retvals.done==1 && retvals.errormessage!="")
        QMessageBox::warning(curr, "Error Report", retvals.errormessage, "OK");
      if(retvals.done==-1)
        QMessageBox::critical(curr, "Error Report", retvals.errormessage, "OK");
	  return retvals;
  }

  void InvokeCLI::progressBarPopUp(SecDialog* curr, QTimer * t)
  {
      /****Start of progress bar update*******/
      parse retvals;
      retvals=fileParse(curr);

      // Special values
      /*
      if(retvals.progresscount==0)
      {
          retvals.progresscount=1;
      }
      else // */
          if(retvals.progresscount==100)
      {
          if(retvals.done==1)
          {
            //retvals.progresscount=100;
            t->stop();
            retvals.currentFile = "Done!";
          }
          else
          {
              retvals.progresscount=99;
          }
      }

      // Error condition
      if(retvals.done==-1)
      {
        retvals.progresscount=100;
        t->stop();
      }

      if (retvals.done==1)
      {
          curr->pd->setValue(100);
          curr->pd->setLabelText("Encrypting file: Done!");
          curr->pd->setCancelButtonText("Continue");
      }
      else
      {
          curr->pd->setValue(retvals.progresscount);
          curr->pd->setLabelText("Encrypting file: " + retvals.currentFile);
      }

      //use message box to test values
      //QMessageBox::warning(this, progress, curFileCount, maxCount); //tr("Cant find temp file \n")
  }

  bool InvokeCLI::discoverDevice(SecDialog* curr, QProcess* proc)
  {
      parse retvals;
      retvals.discover = -1;
      //retvals=fileParse(curr);

      QString program = CLI_PATH;
      QStringList attributes;
      attributes << "--discover-device";
      attributes << "-gui" << TEMP_FILE_NAME;
      proc->start(program, attributes);
      if(!proc->waitForFinished())
      {
          qDebug() << "Fail:\n"<< proc->errorString();
      }
      else
      {
          qDebug() << "Success:\n" << proc->readAll();
          qDebug("Done!\n");
      }

      proc->close();

      retvals=fileParse(curr);

      if (retvals.discover == 1)
          return true;
      else
          return false;
  }

  void InvokeCLI::encryptFiles(SecDialog* curr, QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc)
  {
    /*
     * getOpt.exe --encrypt -f file1 -f file2 -f file3 --session-id 70056 -fd someDrive/someFolder1/ -o option1 --cipher-mode ofb
     */
    parse retvals;
    retvals=fileParse(curr);

    QString program = CLI_PATH;
    QStringList attributes;
    attributes << "--encrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    QString ID = QString::number(sessionID);
    attributes << "--session-id" << ID;
    attributes << "-fd" << folderName;
    attributes << "-gui" << TEMP_FILE_NAME;
    if(cipherMode!="cfb" && cipherMode!="ofb" && cipherMode!="gcm")
        cipherMode = "cfb";
    attributes  << "--cipher-mode" << cipherMode;
    //attributes << "-o" << "option1";
    //attributes  << "-o" << "option2";
    proc->start(program, attributes);
    int exitCode = 0;
    /*
    if(!proc->waitForFinished() || ((exitCode = proc->exitCode()) != 0))
        qDebug() << "Fail: " << exitCode
                 << "\n"<< proc->errorString()
                 << "\nSTDOUT: " << proc->readAllStandardOutput()
                 << "\nSTDERR: " << proc->readAllStandardError();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
    proc->close();
    // */
}

  void InvokeCLI::decryptFiles(SecDialog* curr, QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc)
  {
   /*
    * getOpt.exe --decrypt -f file1 -f file2 -f file3 --session-id 70056 -fd someDrive/someFolder1/ -o option1 --cipher-mode ofb
    */
    parse retvals;
    retvals=fileParse(curr);

    QString program = CLI_PATH;
    QStringList attributes;
    attributes << "--decrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    QString ID = QString::number(sessionID);
    attributes << "--session-id" << ID;
    attributes << "-gui" << TEMP_FILE_NAME;
    attributes << "-fd" << folderName;
    if(cipherMode!="cfb" && cipherMode!="ofb" && cipherMode!="gcm")
        cipherMode = "cfb";
    attributes  << "--cipher-mode" << cipherMode;
    //attributes << "-o" << "option1";
    //attributes  << "-o" << "option2";
    proc->start(program, attributes);
    /*
    if(!proc->waitForFinished())
        qDebug() << "Fail:\n"<< proc->errorString();
    else
    {
        qDebug() << "Success:\n" << proc->readAll();
        qDebug("Done!\n");
    }
    proc->close();
    // */
  }
}
