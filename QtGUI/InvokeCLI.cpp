#include "InvokeCLI.h"

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

  void InvokeCLI::startSession()
  {
    //needs to get session ID
  }

  void InvokeCLI::refreshSession()
  {

  }

  void InvokeCLI::closeSession()
  {
    //process->close();
  }

  void InvokeCLI::progressBarPopUp(QWidget* curr)
  {
      /****Start of progress bar update*******/
      QString temp = QDir::tempPath()+"/TempComm.cpp"; //"C:\Users\sam\AppData\Local\TempComm.cpp"; //System::GetTempPath();
     // qint64 min=0;
     // qint64 max=100;
      qint64 i=0;
      qint64 progresscount=0;
     // qint64 curBlock=0;
      qint64 positionTrack=0;
      QString curFileCount=0;
      QString maxCount=0;
      //ui->progressBar->setMaximum(max);
      //ui->progressBar->setMinimum(min);
      QFile file(temp);
      if(!file.open(QFile::ReadOnly))
      {
          QMessageBox::warning(curr, "Application", file.fileName(), "ok"); //"Cant find temp file \n"
          return;
      }
      QTextStream in(&file);
      while(!in.atEnd())
      {
          QString line=in.readLine();
          QString::iterator it= line.begin();
          QVector<char> curFileName;
          if(it->isDigit()==true)
          {
              int linePos=0;
              for(it=line.begin(); linePos<line.length(); linePos++)
              {
                  if(*it== "\"")
                  {
                      i=0;
                      while(*it+1!="\"")
                      {
                          QByteArray temparray = (*it+1).toUtf8();
                          curFileName[i] = temparray.at(i);
                          i++;
                      }
                  }
                  switch(positionTrack)
                  {
                      case 0:
                          i=0;
                          while(it->isDigit()==true)
                          {
                              if(i=0)
                                  curFileCount=*it;
                              else
                                  curFileCount=curFileCount.append(*it);
                              i++;
                              it++;
                          }
                          positionTrack++;
                      case 1:
                          i=0;
                          if(it->isDigit()==false)
                              it++;
                          while(it->isDigit()==true)
                          {
                              if(i=0)
                              {
                                  maxCount=*it;
                              }
                              else
                                  maxCount=maxCount.append(*it);
                              i++;
                              it++;
                              positionTrack++;
                          }
                      case 2:
                          while(it->isDigit()==true)
                          {
                              it++;
                              positionTrack++;
                          }
                      case 3:
                          while(it->isDigit()==true)
                          {
                              it++;
                              positionTrack++;
                          }
                  }
              }
          }
      }
      file.close();
      qreal curC=curFileCount.toInt();
      qreal maxC=maxCount.toInt();
      progresscount=100*curC/maxC;

      //use message box to test values
      //QMessageBox::warning(this, progress, curFileCount, maxCount); //tr("Cant find temp file \n")
      int numTasks = 100000;
      QProgressDialog progress("Encrypting File(s)...", "Cancel Encryption", 0, numTasks, curr);
      progress.setWindowModality(Qt::WindowModal);

      for (int i = 0; i < numTasks; i++) {
          progress.setValue(i);

          if (progress.wasCanceled())
              break;
      }
      progress.setValue(numTasks);
      //QString sprogress=QString::number(progresscount);
      //QProgressDialog progress("Encrypting File(s)...", "Cancel", 0, 100);
      //ui->progressBar->setValue(progress);

      /****End of progressbar update --SamAdams*******/

      /*
          QProgressDialog *dlg = new QProgressDialog(this);
          qint64 len = src.bytesAvailable();
          dlg->setRange(0,len);
          dlg->show();

          char ch;
          while(!src.atEnd())
          {
            src.getChar(&ch);
            dst.putChar(ch);
            dlg->setValue(dlg->value()+1);
            qApp->processEvents();
          }*/
  }

  bool InvokeCLI::discoverDevice()
  {
    //if()
     //   return true;

    return true;
  }

  void InvokeCLI::encryptFiles(QStringList fileNames, QString folderName, QProcess* proc)
  {
    /*
     * getOpt.exe -a encrypt -f file1 -f file2 -f file3 -s 0056 -fd file4 -o option1 -o option2
     */
    QString program = "C:/Qt/Tools/QtCreator/bin/EncryptApp/GetOpt/getOpt.exe";
    QStringList attributes;
    attributes << "-a" << "encrypt";
    for(int i=0; i<fileNames.size(); i++)
        attributes << "-f" << fileNames.at(i);
    attributes << "--session-id" << "0056";
    attributes << "-fd" << folderName;
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

  void InvokeCLI::decryptFiles(QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc)
  {
   /*
    * getOpt.exe --decrypt -f file1 -f file2 -f file3 --session-id 70056 -f file4 -o option1 --cipher-mode ofb
    */

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
