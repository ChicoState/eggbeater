#include <stdio.h>
#include <tchar.h>
#include "Serial.cpp"	// Library described above
#include <string>
#include <iostream>
using namespace std;
// application reads from the specified serial port and reports the collected data
int _tmain(int argc, TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("Port_#0002.Hub_#0006");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 256;
	int readResult = 0;

	while(SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData,dataLength);
		printf("Bytes read: (-1 means no data available) %i\n",readResult);

		std::string test(incomingData);

		printf("%s",incomingData);
		test = "";

		Sleep(500);
	}
	return 0;
}