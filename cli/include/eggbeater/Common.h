#ifndef _EGGBEATER_SERIAL_H_
#define _EGGBEATER_SERIAL_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <stdexcept>

#include "Common.h"

namespace EggBeater
{
  typedef std::vector<uint8_t> ByteArray;
  typedef std::string String;
  
  //! @todo Add common types, etc as needed
  
  class Exception : public std::runtime_error
  {
  public:
    Exception() : std::runtime_error("Unknown exception") {};
    Exception(const String& msg) : std::runtime_error(msg) {};
    Exception(const Exception& msg) : std::runtime_error(msg.what()) {};
    
    String msg() const { return String(this->what()); };
  };
}

#endif // _EGGBEATER_SERIAL_H_
