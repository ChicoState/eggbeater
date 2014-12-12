#ifndef _EGGBEATER_COMMON_H_
#define _EGGBEATER_COMMON_H_

#include <stdint.h>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>

#define EGGBEATER_SOF_BYTE  0x55
#define EGGBEATER_EOF_BYTE  0xff

namespace EggBeater
{
  typedef std::vector<uint8_t> ByteArray;
  typedef std::string String;
  typedef std::list<String> StringList;
  
  //! @todo Add common types, etc as needed
  
  class Exception : public std::runtime_error
  {
  public:
    Exception() : std::runtime_error("Unknown exception") {};
    Exception(const String& msg) : std::runtime_error(msg) {};
    Exception(const Exception& msg) : std::runtime_error(msg.what()) {};
    
    String msg() const { return String(this->what()); };
  };
  
  String getTempPath();
}

#endif // _EGGBEATER_COMMON_H_
