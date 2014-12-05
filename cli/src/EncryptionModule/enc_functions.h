#ifndef _ENC_FUNCTIONS_
#define _ENC_FUNCTIONS_

#include <string>
#include <cryptopp/aes.h>
#include <vector>
#include <stdint.h>

class enc_functions
{
	public:
		void encrypt_file(std::string, std::string, std::string, std::string);
		void decrypt_file(std::string, std::string, std::string, std::string);

	private:
		
		byte key[CryptoPP::AES::MAX_KEYLENGTH];
		byte iv[ CryptoPP::AES::BLOCKSIZE ];

};

#endif
