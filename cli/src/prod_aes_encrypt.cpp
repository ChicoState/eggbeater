/* A quick run at encryption using AES and crypto++ library
	later versions will provide option to chose files and only allow
	use of this program after verification from an ARM board.


   I can not get the "Crypto.cpp" file to compile. my guess is that i need cigwin. I will try that route.
   This test does not have a fixed key and the block mode should be changed to GCM preferabaly. Should work that in soon.
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

//Crypto++ lib files
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "cryptopp/osrng.h"
using namespace std;

void encrypt_file(string);
void decrypt_file();


byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ]; // cout this to see what it is.
byte iv[ CryptoPP::AES::BLOCKSIZE ];

int main()
{
   //if credentials form ARM board are valid ...
   int usr_choice;
   bool Exit_test = false;
   while(Exit_test != true)
   {
	   cout<<"Encrypt[1]\nDecrypt[2]\nQuit[0]";
	   cin>>usr_choice;

	   if(usr_choice == 1)
	   {
	   	string provided_data;  // change to int
	   	cout<<"Enter data: ";
	   	cin>>provided_data;
	   	encrypt_file(provided_data);
	   }
	   else if(usr_choice == 2)
	   {
	   	cout<<"decrypting file....."<<endl;
	   	decrypt_file();
	   }
	   else if(usr_choice == 0)
	   {
	   	Exit_test = true;
	   }
   }
	return 0;
}

void encrypt_file(string data)
{
	string plaintext = data;
	string ciphertext;
	ofstream aes_file;

	memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
	memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
	stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
	stfEncryptor.MessageEnd();

	aes_file.open("Locked_file.txt");
	aes_file << ciphertext;
	aes_file.close();

}

void decrypt_file()
{
	string ciphertex;
	string decryptedtext;

	ifstream created_file("Locked_file.txt");
	ofstream unlocked_file;

	created_file.is_open();
	getline(created_file, ciphertex);
	created_file.close();

	cout<<decryptedtext<<endl;
	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
	stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertex.c_str() ), ciphertex.size() );
	stfDecryptor.MessageEnd();

	unlocked_file.open("Locked_file.txt");
	unlocked_file << decryptedtext;
	unlocked_file.close();
}
