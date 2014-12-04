#ifndef _EGGBEATER_CRYPTO_H_
#define _EGGBEATER_CRYPTO_H_

#include <vector>
#include <stdint.h>

#include <eggbeater/Common.h>

#include <filters.h>
#include <files.h>

namespace EggBeater
{
  enum class CipherMode : uint32_t
  {
    None  = 0,
    CFB   = 1,
    OFB   = 2,
    GCM   = 3
  };
  
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
  ByteArray tau(const ByteArray& phase2Key, const ByteArray& plainText, CipherMode = CipherMode::CFB, const ByteArray& = {0});
  
  class Crypto
  {
  public:
    Crypto();
    ~Crypto();
    
    void setCipherMode(CipherMode);
    void setEncryptionKey(const ByteArray&);
    void setInitialVector(const ByteArray&);
    
    void encryptFile(const String&, const String&);
    void decryptFile(const String&, const String&);
  protected:
    // To facilitate testing
    void encrypt(CryptoPP::Source*, CryptoPP::Sink*);
    void decrypt(CryptoPP::Source*, CryptoPP::Sink*);
    //void decrypt(std::shared_ptr<CryptoPP::Source>, std::shared_ptr<CryptoPP::Sink>);
  
    CipherMode  cipherMode;
    ByteArray   iv;
    ByteArray   key;
  };
}

#endif // _EGGBEATER_CRYPTO_H_
