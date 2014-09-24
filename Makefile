
GTEST_DIR     = deps/gtest
CRYPTOPP_DIR  = deps/cryptopp

CXXFLAGS += -O0 -std=gnu++11 "-I$(GTEST_DIR)" "-I$(GTEST_DIR)/include" "-I$(CRYPTOPP_DIR)" "-Iinclude" "-Ideps"
LDFLAGS  += "-Llib"

all: lib/libcryptopp.a lib/gtest.a testsuite

src/Crypto.o: src/Crypto.cpp
	$(CXX) -c -o $@ $+ $(CXXFLAGS)

lib/.d:
	test -d lib || mkdir lib
	#mkdir lib
	touch lib/.d

lib/libcryptopp.a: lib/.d
	make "-C$(CRYPTOPP_DIR)" libcryptopp.a
	cp "$(CRYPTOPP_DIR)/libcryptopp.a" lib/libcryptopp.a
	
lib/gtest.a: lib/.d
	make "-C$(GTEST_DIR)/make" gtest.a
	cp "$(GTEST_DIR)/make/gtest.a" lib/gtest.a

testsuite: tests/Crypto_tests

tests/Crypto_tests.o: tests/Crypto_tests.cpp
	$(CXX) -c -o $@ $+ $(CXXFLAGS)

tests/Crypto_tests: tests/Crypto_tests.o src/Crypto.o lib/libcryptopp.a lib/gtest.a
	$(CXX) -o $@ $+ $(LDFLAGS)

clean:
	rm -rf lib src/*.o tests/*.o tests/*.exe