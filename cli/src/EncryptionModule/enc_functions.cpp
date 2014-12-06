
#include "enc_functions.h"

// crypto libaries found below
// the "using" includes allows the code
// to look a little readable.

using namespace std;

int enc_functions::encrypt_file(string encMode, string oFile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)
{
    //Allows encryption if input Vectors are of the required size
    if( (pwordKey.size() == CryptoPP::AES::MAX_KEYLENGTH) && (ivec.size() == CryptoPP::AES::BLOCKSIZE) )
    {
        //pulls vectors into byte and initilization vectors
        copy(pwordKey.begin(), pwordKey.end(), key);
        copy(ivec.begin(), ivec.end(), iv);

        //Adds .egg to siginifiy that the file is encrypted by eggbeater
        string ofilename = oFile;
        string outFile = oFile + ".egg";
        string efilename = outFile;   

        // creates a copy of the input file
        // and converts it to binary for encryption.
        // Helps avoid the creation of corrupted files
        // durring decryption process
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
            cerr << "[ERROR] Invalid Mode" <<endl;
            return 1;
        }

        return 0;
    }
    else
    {
        cerr<<"[ERROR] Key size is insufficient"<<endl;
        return 1;
    }
}

int enc_functions::decrypt_file(string decMode, string efile, vector<uint8_t> pwordKey, vector<uint8_t> ivec)  //keep hash
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
           return 1;
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
            return 1;
        }

        return 0;
    }
    else
    {
        cerr<<" [ERROR] Key size is insufficient"<<endl;
        return 1;
    }
}