CXX = g++
CXXFLAGS = -g -Wall -W -Wno-sign-compare

OBJ = fsm.o
BIN = fsm-info fsm-copy
TEST_BIN = fsm-test

all: $(BIN) 

test_compile: $(TEST_BIN)
    
test: test_compile
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
