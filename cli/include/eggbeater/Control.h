#ifndef _EGGBEATER_CONTROL_H_
#define _EGGBEATER_CONTROL_H_

#include <eggbeater/Options.h>

/*******************************************************************************
                     This class is not yet finalized
*******************************************************************************/
namespace EggBeater
{
  class Control
  {
  public:
    // Control(OptionsContainer) // or whatever the class is named
    
    bool run(void);
    
    String getStatus();
  private:
    //! Internal function to start a new session
    void newSession();
    //! Internal function to open a session with an existing fingerprint
    void openSession();
    //! Internal function to refresh the session timer
    void refreshSession();
    //! Internal function to close the session
    void closeSession();
    
    //! Internal function to encrypt the specified files
    void encryptFiles();
    //! Internal function to decrypt the specified files
    void decryptFiles();
  };
}

#endif // _EGGBEATER_CONTROL_H_
