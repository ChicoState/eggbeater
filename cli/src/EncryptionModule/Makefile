CC= g++ -DDEBUG=1 -g3 -O0 -Wall -Wextra 

all: enc_functions.h enc_functions.cpp main.cpp
	$(CC) -o enc enc_functions.h enc_functions.cpp main.cpp -lcryptopp 
