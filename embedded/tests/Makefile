GTEST_DIR = ../../cli/deps/gtest-1.7.0

CFLAGS   += -O0                       \
            -D_EGGBEATER_TESTS_       \
            -g                        \
						"-I$(GTEST_DIR)"          \
						"-I$(GTEST_DIR)/include"  \
						"-I../inc"                \
						-ffunction-sections       \
						-fdata-sections
LDFLAGS  += -Wl,-gc-sections          \
            -lpthread

SOURCES   = ../src/Control.o

TESTS     = control_tests.elf

all: $(SOURCES) $(TESTS)

%.c.o: %.c
	$(CC) -c $(CFLAGS) $+ -o $@

%.cpp.o: %.cpp
	$(CXX) -c $(CFLAGS) $+ -o $@
	
%.elf: %.cpp.o $(SOURCES)
	$(CXX) $(LDFLAGS) $+ -o $@ "$(GTEST_DIR)/make/gtest.a"

.PHONY: clean
clean:
	rm -rf $(shell find ../src -type f -name "*.o") $(shell find . -type f -name "*.o") $(shell find . -type f -name "*.elf")
