#include <eggbeater/Crypto.h>
/* Crypto++ includes */
#include <sha3.h>
#include <gcm.h>
#include <aes.h>
#include <modes.h>
#include <files.h>
#include <filters.h>

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
  
  Crypto::Crypto() : cipherMode(CipherMode::GCM)
  {
  }
  
  Crypto::~Crypto()
  {
    // Clear the key and IV
    memset(key.data(), 0, key.size());
    memset(iv.data(), 0, iv.size());
  }
  
  void Crypto::setCipherMode(CipherMode mode)
  {
    this->cipherMode = mode;
  }
  
  void Crypto::setEncryptionKey(const ByteArray& key)
  {
    this->key = key;
  }
  
  void Crypto::setInitialVector(const ByteArray& iv)
  {
    this->iv = iv;
  }
  
  void Crypto::encryptFile(const String& inputFile, const String& outputFile)
  {
    Sink* sink = new CryptoPP::FileSink(outputFile.c_str());
    std::shared_ptr<CryptoPP::Source> source(new CryptoPP::FileSource(inputFile.c_str(), true));
  
    encrypt(source.get(), sink);
  }
  
  void Crypto::decryptFile(const String& inputFile, const String& outputFile)
  {
    Sink* sink = new CryptoPP::FileSink(outputFile.c_str());
    std::shared_ptr<CryptoPP::Source> source(new CryptoPP::FileSource(inputFile.c_str(), true));
  
    decrypt(source.get(), sink);
  }
  
  void Crypto::encrypt(CryptoPP::Source* source, CryptoPP::Sink* sink)
  {
    MeterFilter sinkMeter(sink, true);
    std::function<void(const ByteArray& key, const ByteArray& iv, CryptoPP::Source* source, CryptoPP::Sink* sink)> cipherFunction;
    
    // Because Crypto++ doesn't fucking take pointers for StreamTransformationFilter
    switch (this->cipherMode)
    {
      case CipherMode::CFB:
        cipherFunction = [](const ByteArray& key, const ByteArray& iv, CryptoPP::Source* source, CryptoPP::Sink* sink)
        {
          CFB_Mode<AES>::Encryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new StreamTransformationFilter(enc, sink));
          
          source->PumpAll();
        };
        break;
      
      case CipherMode::OFB:
        cipherFunction = [](const ByteArray& key, const ByteArray& iv, CryptoPP::Source* source, CryptoPP::Sink* sink)
        {
          OFB_Mode<AES>::Encryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new StreamTransformationFilter(enc, sink));
          
          source->PumpAll();
        };
        break;
      default:
      case CipherMode::GCM:
        cipherFunction = [](const ByteArray& key, const ByteArray& iv, CryptoPP::Source* source, CryptoPP::Sink* sink)
        {
          GCM<AES>::Encryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new AuthenticatedEncryptionFilter(enc, sink));
          
          source->PumpAll();
        };
        break;
    }
    
    if (!cipherFunction)
      return;
    
    cipherFunction(key, iv, source, new Redirector(sinkMeter));
    
    #if _EGGBEATER_TESTS_
    std::cout << "encrypt #sink bytes: " << sinkMeter.GetTotalBytes() << std::endl;
    #endif
  }
  
  void Crypto::decrypt(CryptoPP::Source* source, CryptoPP::Sink* sink)
  {
    MeterFilter meter(sink, true);
    std::function<void(const ByteArray& key, const ByteArray& iv)> cipherFunction;
    
    // Because Crypto++ doesn't fucking take pointers for StreamTransformationFilter
    switch (this->cipherMode)
    {
      case CipherMode::CFB:
        cipherFunction = [source, &meter](const ByteArray& key, const ByteArray& iv)
        {
          CFB_Mode<AES>::Decryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new StreamTransformationFilter(enc, new Redirector(meter)));
          
          source->PumpAll();
        };
        break;
      
      case CipherMode::OFB:
        cipherFunction = [source, &meter](const ByteArray& key, const ByteArray& iv)
        {
          OFB_Mode<AES>::Decryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new StreamTransformationFilter(enc, new Redirector(meter)));
          
          source->PumpAll();
        };
        break;
      default:
      case CipherMode::GCM:
        cipherFunction = [source, &meter](const ByteArray& key, const ByteArray& iv)
        {
          GCM<AES>::Decryption enc;
          
          enc.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
          
          source->Attach(new AuthenticatedDecryptionFilter(enc, new Redirector(meter)));
          
          source->PumpAll();
        };
        break;
    }
    
    if (!cipherFunction)
      return;
    
    cipherFunction(key, iv);
    
    #if _EGGBEATER_TESTS_
    std::cout << "decrypt #sink bytes: " << meter.GetTotalBytes() << std::endl;
    #endif
  }
  
  #if 0
  void Crypto::decryptFile(const String& inputFile, const String& outputFile)
  {
    CryptoPP::Sink* sink = new FileSink(outputFile);
  }
  
  void Crypto::decrypt(std::shared_ptr<CryptoPP::Source> source, std::shared_ptr<CryptoPP::Sink> sink)
  {
    std::shared_ptr<StreamTransformation> decrypt;
    std::shared_ptr<CryptoPP::Filter> filter;
    
    switch (this->cipherMode)
    {
      default:
        decrypt = nullptr;
        break;
      
      case CipherMode::CFB:
        decrypt = new CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption(sink);
        break;
      case CipherMode::OFB:
        decrypt = new CryptoPP::OFB_Mode<CryptoPP::AES>::Decryption(sink);
        break;
      case CipherMode::GCM:
        decrypt = new CryptoPP::GCM<CryptoPP::AES>::Decryption(sink);
        break;
    }
    
    if (!decrypt)
      return;
    
    decrypt.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());
    
    filter = new Filter(decrypt);
    
    source.attach(new CryptoPP::Filter(decrypt));
  }
  
  #endif
}