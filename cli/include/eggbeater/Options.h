#ifndef _EGGBEATER_OPTIONS_H_
#define _EGGBEATER_OPTIONS_H_

#include <eggbeater/Common.h>

namespace EggBeater
{
  enum class CLI_Command : uint8_t
  {
    None = 0,
    StartSession = 1,
    RefreshSession = 2,
    CloseSession = 3,
    Encrypt = 4,
    Decrypt = 5
  };
  
  //! @todo Integrate Sterling's getopt code
}

#endif // _EGGBEATER_OPTIONS_H_
