// g++ -DDEBUG=1 -g3 -O0 -Wall -Wextra v4_CLI_fileEncrypt.cpp -o crypt -lcryptopp
// ./crypt -dec OFB tacos.jpg.egg 00000000000000sam000000000000001
 

/**
    All Modes currently working.........

    To Do's
    - how to recive and sink the key.
    - remove unnescarry crap
    - document / comment
    - take everything out of main
    - error checking
    - delete the original file / encrypted file on action?
    - hash the key?
**/
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

using namespace std;
 
 
void encrypt_file(string, string, string);
void decrypt_file(string, string, string);

byte key[AES::MAX_KEYLENGTH];
byte iv[ AES::BLOCKSIZE ];

int main(int argc, char* argv[])
{
    string action = argv[1];

    if(argc == 5 && action == "-enc")
        encrypt_file(argv[2], argv[3], argv[4]); //argv3 is the provided key

    else if(argc == 5 && action == "-dec")
        decrypt_file(argv[2], argv[3], argv[4]);

    else
        cout<<"shit"<<endl;

    return 0;  

}
void encrypt_file(string encMode, string oFile, string pwordKey)
{
    // SecByteBlock key(AES::MAX_KEYLENGTH);
    // byte iv[ AES::BLOCKSIZE ];

    for (int i = 0; i < pwordKey.size(); i++)
        key[i] = pwordKey[i];
   
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

void decrypt_file(string decMode, string efile, string pwordKey2)  //keep hash
{   

    string efilename = efile;
    efile.erase(efile.end()-4, efile.end());
    string rfilename = efile;

    //SecByteBlock key(AES::MAX_KEYLENGTH);
    //byte iv[ AES::BLOCKSIZE ];
    for (int i = 0; i < pwordKey2.size(); i++)
        key[i] = pwordKey2[i];

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