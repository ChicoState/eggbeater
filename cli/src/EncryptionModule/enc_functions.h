#ifndef _ENC_FUNCTIONS_
#define _ENC_FUNCTIONS_

#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h> 
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

class enc_functions
{
	public:
		int encrypt_file(std::string, std::string, std::vector<uint8_t>, std::vector<uint8_t>);
		int decrypt_file(std::string, std::string, std::vector<uint8_t>, std::vector<uint8_t>);

	private:
		
		byte key[CryptoPP::AES::MAX_KEYLENGTH];
		byte iv[ CryptoPP::AES::BLOCKSIZE ];

};

#endif
