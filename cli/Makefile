GTEST_DIR     = deps/gtest-1.7.0
CRYPTOPP_DIR  = deps/cryptopp562

CXXFLAGS += -O0                      \
            -g                       \
            -std=gnu++11             \
            "-I$(GTEST_DIR)"         \
            "-I$(GTEST_DIR)/include" \
            "-I$(CRYPTOPP_DIR)"      \
            "-Iinclude"              \
            "-Ideps"                 \
            -ffunction-sections      \
            -fdata-sections
LDFLAGS  += "-Llib" -Wl,-gc-sections -lpthread

SOURCES   = src/Crypto.o                      \
            src/Discovery.o                   \
            src/SerialCommunication.o         \
            src/Options.o                     \
            src/packet.o

TESTS     = tests/Crypto_tests.elf            \
            tests/Crypto_CipherMode_tests.elf \
            tests/Discovery_test_present.elf  \
            tests/Discovery_test_absent.elf   \
            tests/Discovery_tests.elf         \
            tests/SerialComm_tests.elf        \
            tests/SerialComm_test_port.elf    \
            tests/Options_tests.elf           \
            tests/Packet_tests.elf

all: lib/libcryptopp.a lib/gtest.a testsuite $(SOURCES)

%.o: %.cpp
	$(CXX) -c -o $@ $+ $(CXXFLAGS)

#lib/.d:
#	test -d lib || mkdir lib
#	touch lib/.d

%/.d:
	test -d $(dir $@) || mkdir $(dir $@)
	touch $(dir $@)/.d

lib/libcryptopp.a: lib/.d
	$(MAKE) "-C$(CRYPTOPP_DIR)" libcryptopp.a
	cp "$(CRYPTOPP_DIR)/libcryptopp.a" lib/libcryptopp.a
	
lib/gtest.a: lib/.d
	$(MAKE) "-C$(GTEST_DIR)/make" gtest.a
	cp "$(GTEST_DIR)/make/gtest.a" lib/gtest.a

.PHONY: testsuite
testsuite: $(TESTS)

%.elf: %.o $(SOURCES) lib/libcryptopp.a lib/gtest.a
	$(CXX) -o $@ $+ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf lib                                \
	$(shell find src -type f -name "*.o")     \
	$(shell find tests -type f -name "*.o")   \
	$(shell find tests -type f -name "*.elf")