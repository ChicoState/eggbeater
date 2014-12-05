#include <iostream>
#include <vector>
#include "enc_functions.h"
#include <stdint.h>

using namespace std;

int main(int argc, char* argv[])
{
	enc_functions e;
    string action = argv[1];

    vector<uint8_t> k;
    vector<uint8_t> v;

    for (int i=0; i<64; i++)
    	k.push_back(0);
    for (int i=0; i<32; i++)
    	v.push_back(0);

    if(argc == 6 && action == "-enc")
        e.encrypt_file(argv[2], argv[3], k, v); //argv3 is the provided key

    else if(argc == 6 && action == "-dec")
        e.decrypt_file(argv[2], argv[3], k, v);

    else
        cout<<"shit"<<endl;

    return 0;  

}