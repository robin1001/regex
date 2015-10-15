CXX = g++
CXXFLAGS = -g -Wall -W -Wno-sign-compare

OBJ = fsm.o fsm-operation.o
BIN = fsm-info fsm-copy
TEST_BIN = fsm-test

all: $(BIN) 

test: $(TEST_BIN)
    
run_test: test
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
fsm-operation: fsm.o fsm-operatoin.h

.PHONY: clean
clean:
	-rm -f *.exe *.o *.stackdump core.*
