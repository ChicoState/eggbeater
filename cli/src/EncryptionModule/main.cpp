#include <iostream>
#include <vector>
#include "enc_functions.h"

using namespace std;

int main(int argc, char* argv[])
{
	enc_functions e;
    string action = argv[1];

    if(argc == 6 && action == "-enc")
        e.encrypt_file(argv[2], argv[3], argv[4], argv[5]); //argv3 is the provided key

    else if(argc == 6 && action == "-dec")
        e.decrypt_file(argv[2], argv[3], argv[4], argv[5]);

    else
        cout<<"shit"<<endl;

    return 0;  

}