CXX = g++
CXXFLAGS = -g -Wall -W -Wno-sign-compare

BIN = fsminfo
TEST_BIN = fsm_test

all: $(BIN) test

compile_test: $(TEST_BIN)

test:compile_test

fsm_test: fsm_test.cc fsm.o
	$(CXX) -o $@ $< fsm.o $(CXXFLAGS)

fsminfo: fsminfo.cc fsm.o
	$(CXX) -o $@ $< fsm.o $(CXXFLAGS)


fsm.o:fsm.h utils.h


.PHONY: clean
clean:
	-rm -f *.exe *.o *.stackdump core.*
