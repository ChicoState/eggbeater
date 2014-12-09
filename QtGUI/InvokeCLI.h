#ifndef _QTGUI_INVOKE_CLI_H_
#define _QTGUI_INVOKE_CLI_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <QStringList>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QTranslator>
#include <QTextStream>
#include <QProgressDialog>
#include <iterator>
#include <QDir>

#include "secdialog.h"

namespace EggBeater
{
  typedef std::string String;
  typedef std::vector<String> StringList;

  class InvokeCLI
  {
  struct parse{
      int progresscount;
      int sessionID;
      QString errormessage;
      int done;
  };
  public:
    InvokeCLI();
    ~InvokeCLI();

    /**
      Check if the session is open.
    **/
    bool sessionIsOpen() const;

    /**
      Start the session and get the session ID.
    **/
    void startSession(QProcess* proc);

    /**
      Refresh the current session.
    **/
    void refreshSession(QProcess* proc);

    /**
      Close the current session.
    **/
    void closeSession(QProcess* proc);

    parse fileParse(SecDialog* curr);

    //function to update the progressbar
    void progressBarPopUp(SecDialog* curr);

    /**
      Check for the connected fingerprint device.
    **/
    bool discoverDevice(QProcess* proc);

    /**
      Encrypt the specified files with the specified cipher mode. Defaults to
      CFB if no cipher mode is passed.
    **/
    void encryptFiles(QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc);

    /**
      Decrypt the specified files with the specified cipher mode. Defaults to
      CFB if no cipher mode is passed.
    **/
    void decryptFiles(QStringList fileNames, QString folderName, QString cipherMode, QProcess* proc);

  private:
    uint32_t sessionID;
  };
}

#endif // _QTGUI_INVOKE_CLI_H_
