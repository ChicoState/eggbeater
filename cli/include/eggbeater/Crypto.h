#ifndef _EGGBEATER_CRYPTO_H_
#define _EGGBEATER_CRYPTO_H_

#include <vector>
#include <stdint.h>

namespace EggBeater
{
  typedef std::vector<uint8_t> ByteArray;
  
  /*
    phase 1 password = password input from LCD on STM32F429
    phase 1 key      = cryptographic hash of phase 1 password (256-bit in length)
    
    phase 2 passcode = fingerprint template from GT511C1R
    phase 2 key      = cryptographic hash of phase 2 passcode (256-bit in length)
    
    delta[phase 1 password]       = phase 1 key
    gamma[phase 2 passcode]       = phase 2 key
    tau[phase 2 key, plain text]  = cipher text
  */
  
  ByteArray delta(const ByteArray& phase1Password);
  ByteArray gamma(const ByteArray& phase2Passcode);
  ByteArray tau(const ByteArray& phase2Key, const ByteArray& plainText);
}

#endif // _EGGBEATER_CRYPTO_H_
