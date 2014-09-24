#include <EggBeater/Crypto.h>
#include <cryptopp/sha3.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#ifndef EB_DEBUG
#define EB_DEBUG 0
#endif

#if EB_DEBUG == 1
  #define EB_DEBUG_OUTPUT(expr) { expr; }
#else
  #define EB_DEBUG_OUTPUT(...)
#endif

#include <iostream>

using namespace CryptoPP;

namespace EggBeater
{
  ByteArray delta(const ByteArray& phase1Password)
  {
    ByteArray ret;
    SHA3_256 hash;
    
    hash.Update(phase1Password.data(), phase1Password.size());
    
    ret.resize(256 / 8);
    hash.TruncatedFinal(ret.data(), ret.capacity());
    
    return ret;
  }

  ByteArray gamma(const ByteArray& phase2Passcode)
  {
    return delta(phase2Passcode);
    //ByteArray ret;
    //return ret;
  }

  ByteArray tau(const ByteArray& phase2Key, const ByteArray& plainText)
  {
    uint8_t iv[16] = {0};
    ByteArray ret;
    
    if (phase2Key.size() < (256 / 8))
    {
      throw std::runtime_error("tau: phase 2 key must be at least 32 bytes in length");
    }
    
    AES::Encryption cipher(phase2Key.data(), 256/8);
    
    // Future version should use an encryption mode. One with feedback
    ECB_Mode_ExternalCipher::Encryption cfbCipher(cipher, iv);
    
    EB_DEBUG_OUTPUT(std::cout << cipher.StaticAlgorithmName() << std::endl);
    
    ret.resize(plainText.size());
    
    EB_DEBUG_OUTPUT(std::cout << "ret.size=" << ret.size() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "ret.cap=" << ret.capacity() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "plainText.size=" << plainText.size() << std::endl);
    
    cfbCipher.ProcessData(ret.data(), plainText.data(), plainText.size());
    
    EB_DEBUG_OUTPUT(std::cout << "ret.size=" << ret.size() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "ret.cap=" << ret.capacity() << std::endl);
    
    // Create AES object
    // Pass plainText into AES object
    // Which cipher mode do we want?
    
    //! @TODO This is where SamW's encryption code should go
    
    return ret;
  }
}