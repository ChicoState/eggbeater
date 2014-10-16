#ifndef _EGGBEATER_DISCOVERY_H_
#define _EGGBEATER_DISCOVERY_H_

#include <eggbeater/Common.h>

namespace EggBeater
{
  typedef std::vector<String> StringList;
  
  /**
    Use OS facilities to discover any attached fingerprint dongles.
    @return Returns a list of possible fingerprint dongles. Each device have the specified vendor ID and product ID.
  **/
  StringList discover_devices(uint16_t vid, uint16_t pid);
}

#endif // _EGGBEATER_DISCOVERY_H_
