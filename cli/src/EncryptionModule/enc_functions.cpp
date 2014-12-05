#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
 
#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
using CryptoPP::PK_Encryptor;
using CryptoPP::PK_Decryptor;
 
#include <cryptopp/sha.h>
using CryptoPP::SHA256;
 
#include <cryptopp/secblock.h>
using CryptoPP::SecByteBlock;
 
#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;
 
#include <cryptopp/queue.h>
using CryptoPP::ByteQueue;
 
#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder; 
 
#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::FileSink;
using CryptoPP::FileSource;
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::PK_DecryptorFilter;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;
 
#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;
 
#include <cryptopp/dh.h>
using CryptoPP::DH;
 
#include <cryptopp/modes.h>
using CryptoPP::CBC_Mode;
 
#include <cryptopp/gcm.h>
using CryptoPP::GCM;
 
#include <cryptopp/tea.h>
using CryptoPP::TEA;
 
#include <cryptopp/aes.h>
using CryptoPP::AES;
 
#include <cryptopp/pssr.h>
using CryptoPP::PSS;
 
#include <cryptopp/rsa.h>
using CryptoPP::RSA;
using CryptoPP::RSASS;
 
#include <cryptopp/nbtheory.h>
using CryptoPP::ModularExponentiation;
 
#include <cryptopp/eccrypto.h>
using CryptoPP::ECDSA;
using CryptoPP::ECP;
 
#include <cryptopp/oids.h>
using CryptoPP::ASN1::secp521r1;
 
#include <cryptopp/modes.h>
using CryptoPP::OFB_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::Exception;

#include "enc_functions.h"

using namespace std;

void enc_functions::encrypt_file(string encMode, string oFile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)
{

   copy(pwordKey.begin(), pwordKey.end()-1, key);
   copy(ivec.begin(), ivec.end()-1, iv);

    string ofilename = oFile;
    string outFile = oFile + ".egg";
    string efilename = outFile;   
    
    ifstream ifile(oFile.c_str(), ios::binary);
    ifstream::pos_type size = ifile.seekg(0, ios_base::end).tellg();
    ifile.seekg(0, ios_base::beg);

    string temp;
    temp.resize(size);
    ifile.read((char*)temp.data(), temp.size());
   
    if(encMode == "OFB")
    {
        OFB_Mode< AES >::Encryption e1;
        e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
       
        StringSource( temp, true,
                        new StreamTransformationFilter( e1,
                        new FileSink( efilename.c_str() ))); 
    }
    else if(encMode == "CFB")
    {
        CFB_Mode< AES >::Encryption e1;
        e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );

        StringSource( temp, true,
                        new StreamTransformationFilter( e1,
                        new FileSink( efilename.c_str() ))); 
    }
    else if(encMode == "GCM")
    {
        GCM< AES >::Encryption e1;
        e1.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
       
        StringSource ss1( temp, true,
                        new AuthenticatedEncryptionFilter( e1,
                        new FileSink( efilename.c_str() )));
    }
    else
    {
        cerr << "Invalid Mode" <<endl;
    }
}

void enc_functions::decrypt_file(string decMode, string efile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)  //keep hash
{   

    string efilename = efile;
    efile.erase(efile.end()-4, efile.end());
    string rfilename = efile;

    copy(pwordKey.begin(), pwordKey.end()-1, key);
    copy(ivec.begin(), ivec.end()-1, iv);

    if(decMode == "OFB")
    {
        OFB_Mode< AES >::Decryption d2;
        d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
       
        FileSource( efilename.c_str(), true,
                        new StreamTransformationFilter( d2,
                        new FileSink( rfilename.c_str() ))); 
    }
    else if(decMode == "CFB")
    {
        CFB_Mode< AES >::Decryption d2;
        d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
       
        FileSource( efilename.c_str(), true,
                        new StreamTransformationFilter( d2,
                        new FileSink( rfilename.c_str() )));
    }
    else if(decMode == "GCM")
    {
        GCM< AES >::Decryption d2;
        d2.SetKeyWithIV( key, sizeof(key), iv, sizeof(iv) );
       
        FileSource fs2( efilename.c_str(), true,
                        new AuthenticatedDecryptionFilter( d2,
                        new FileSink( rfilename.c_str() ),
                        AuthenticatedDecryptionFilter::THROW_EXCEPTION));
    }
    else
    {
        cerr << "Decryption Error" <<endl;
    }
}