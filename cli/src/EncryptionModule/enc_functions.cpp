#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include "enc_functions.h"

// crypto libaries found below
// the "using" includes allows the code
// to look a little readable.
#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;
 
#include <cryptopp/files.h>
using CryptoPP::FileSink;
using CryptoPP::FileSource;
 
#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder; 
 
#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::FileSink;
using CryptoPP::FileSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;
 
#include <cryptopp/modes.h>
using CryptoPP::CBC_Mode;
 
#include <cryptopp/gcm.h>
using CryptoPP::GCM;
 
#include <cryptopp/aes.h>
using CryptoPP::AES;
 
#include <cryptopp/modes.h>
using CryptoPP::OFB_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::Exception;

using namespace std;

void enc_functions::encrypt_file(string encMode, string oFile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)
{
    //Allows encryption if input Vectors are of the required size
    if( (pwordKey.size() == CryptoPP::AES::MAX_KEYLENGTH) && (ivec.size() == CryptoPP::AES::BLOCKSIZE) )
    {
        // creates a copy of the input file
        // and converts it to binary for encryption.
        // Helps avoid the creation of corrupted files
        // durring decryption process
        // checks to make sure the file exists
        ifstream ifile(oFile.c_str(), ios::binary);
        if(!ifile)
        {
            cerr<<"[ERROR] File not Found"<<endl;
            return;
        }

        ifstream::pos_type size = ifile.seekg(0, ios_base::end).tellg();
        ifile.seekg(0, ios_base::beg);

        //pulls vectors into byte and initilization vectors
        copy(pwordKey.begin(), pwordKey.end(), key);
        copy(ivec.begin(), ivec.end(), iv);

        //Adds .egg to siginifiy that the file is encrypted by eggbeater
        string ofilename = oFile;
        string outFile = oFile + ".egg";
        string efilename = outFile;   

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
            cerr << "[ERROR] Invalid Mode" <<endl;
        }
    }
    else
    {
        cerr<<"[ERROR] Key size is insufficient"<<endl;
    }
}

void enc_functions::decrypt_file(string decMode, string efile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)  //keep hash
{   
    //Allows encryption if input Vectors are of the required size
    if( (pwordKey.size() == CryptoPP::AES::MAX_KEYLENGTH) && (ivec.size() == CryptoPP::AES::BLOCKSIZE) )
    {
        //Checks if the ending of the provided filename
        //is an eggbeater encrypted file, otherwise decryption
        //will halt.
        if(!efile.compare(efile.size() - 4, 4, ".egg") == 0)
        {
           cerr<<" [ERROR] The File provided is not an Eggbeater encrypted file \n";
           cerr<<"         Please choose a .egg file to decrypt \n";
           return;
        }

        //checks to make sure the file exists
        ifstream ifile(efile.c_str(), ios::binary);
        if(!ifile)
        {
            cerr<<"[ERROR] File not Found"<<endl;
            return;
        }

        //Removes the .egg from the file type provided
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
            cerr << " [ERROR] Decryption Error" <<endl;
        }
    }
    else
    {
        cerr<<" [ERROR] Key size is insufficient"<<endl;
    }
}