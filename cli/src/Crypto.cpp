#include <eggbeater/Crypto.h>
/* Crypto++ includes */
#include <sha3.h>
#include <gcm.h>
#include <aes.h>
#include <modes.h>

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

  ByteArray tau(const ByteArray& phase2Key, const ByteArray& plainText, CipherMode mode, const ByteArray& iv)
  {
    ByteArray cipherText;
    std::function<void(void)> cipherFunction;
    //auto cipherFunction;
    
    if (phase2Key.size() < (256 / 8))
    {
      throw std::runtime_error("tau: phase 2 key must be at least 32 bytes in length");
    }
    
    AES::Encryption cipher(phase2Key.data(), 256/8);
    
    switch (mode)
    {
      case CipherMode::CFB:
        cipherFunction = [&iv, &phase2Key, &plainText, &cipherText]()
        {
          CFB_Mode<AES>::Encryption enc;
          
          enc.SetKeyWithIV(phase2Key.data(), phase2Key.size(), iv.data());
          
          cipherText.resize(plainText.size());
          
          enc.ProcessData(cipherText.data(), plainText.data(), plainText.size());
        };
        break;
      
      case CipherMode::OFB:
        cipherFunction = [&iv, &phase2Key, &plainText, &cipherText]()
        {
          OFB_Mode<AES>::Encryption enc;
          
          enc.SetKeyWithIV(phase2Key.data(), phase2Key.size(), iv.data());
          
          cipherText.resize(plainText.size());
          
          enc.ProcessData(cipherText.data(), plainText.data(), plainText.size());
        };
        break;
      default:
      case CipherMode::GCM:
        cipherFunction = [&iv, &phase2Key, &plainText, &cipherText]()
        {       
          GCM<AES>::Encryption enc;
          
          enc.SetKeyWithIV(phase2Key.data(), phase2Key.size(), iv.data(), iv.size());
          
          cipherText.resize(plainText.size());
          
          enc.ProcessData(cipherText.data(), plainText.data(), plainText.size());
        };
        break;
    }
    
    if (cipherFunction)
      cipherFunction();
    
    
    #if 0
    // Future version should use an encryption mode. One with feedback
    ECB_Mode_ExternalCipher::Encryption cfbCipher(cipher, iv.data());
    
    EB_DEBUG_OUTPUT(std::cout << cipher.StaticAlgorithmName() << std::endl);
    
    cipherText.resize(plainText.size());
    
    EB_DEBUG_OUTPUT(std::cout << "ret.size=" << cipherText.size() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "ret.cap=" << cipherText.capacity() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "plainText.size=" << plainText.size() << std::endl);
    
    cfbCipher.ProcessData(cipherText.data(), plainText.data(), plainText.size());
    
    EB_DEBUG_OUTPUT(std::cout << "ret.size=" << cipherText.size() << std::endl);
    EB_DEBUG_OUTPUT(std::cout << "ret.cap=" << cipherText.capacity() << std::endl);
    #endif
    
    // Create AES object
    // Pass plainText into AES object
    // Which cipher mode do we want?
    
    //! @TODO This is where SamW's encryption code should go
    
    return cipherText;
  }
}