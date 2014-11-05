#ifndef _QTGUI_INVOKE_CLI_H_
#define _QTGUI_INVOKE_CLI_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <QProcess>

namespace EggBeater
{
  typedef std::string String;
  typedef std::vector<String> StringList;

  class InvokeCLI
  {
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
    void startSession();

    /**
      Refresh the current session.
    **/
    void refreshSession();

    /**
      Close the current session.
    **/
    void closeSession();

    /**
      Check for the connected fingerprint device.
    **/
    bool discoverDevice();

    /**
      Encrypt the specified files with the specified cipher mode. Defaults to
      CFB if no cipher mode is passed.
    **/
    void encryptFiles(const QStringList& files, String cipherMode);

    /**
      Decrypt the specified files with the specified cipher mode. Defaults to
      CFB if no cipher mode is passed.
    **/
    void decryptFiles(const QStringList& files, String cipherMode);

    QProcess *process;
    InvokeCLI* invoke;

  private:
    uint32_t sessionID;
  };
}

#endif // _QTGUI_INVOKE_CLI_H_
