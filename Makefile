CXX = g++
CXXFLAGS = -g -std=c++0x -Wall -W -Wno-sign-compare -pg

OBJ = fsm.o 
BIN = fsm-info fsm-copy
TEST_BIN = fsm-test

all: $(BIN) $(TEST_BIN)

test: $(TEST_BIN)
	@for x in $(TEST_BIN); do \
		printf "Running $$x ..."; \
		./$$x &> /dev/null; \
		if [ $$? -ne 0 ]; then \
            echo "... Fail $$x"; \
		else \
            echo "... Success $$x"; \
		fi \
	done

$(BIN): $(OBJ)

$(TEST_BIN): $(OBJ)

fsm.o: fsm.h utils.h

.PHONY: clean
clean:
	-rm -f *.exe *.o *.stackdump core.*
