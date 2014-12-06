#need to create checks to avoid stack overflows in enc and dec

CC= g++ -DDEBUG=1 -g3 -O0 -Wall -Wextra -fno-stack-protector

all: enc_functions.h enc_functions.cpp main.cpp
	$(CC) -o enc enc_functions.h enc_functions.cpp main.cpp -lcryptopp 
