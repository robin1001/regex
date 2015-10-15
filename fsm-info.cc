#include <stdio.h>

#include "utils.h"
#include "fsm.h"

int main(int argc, char *argv[]) {
    const char *usage = "Showing details text information of fsm format file," 
                        "if in file not provided, read from stdin stream\n"
                        "Usage: fsm-info [fsm_file]\n"
                        "eg: fsm-info in.fsm\n";
    if (argc < 1 || argc > 2) {
        printf(usage);
        return -1;
    }

    Fsm fsm;
    if (argc == 1) {
        fsm.read("-");
    }
    else {
        fsm.read(argv[1]);
    }
    fsm.fsm_info();
    return 0;
}

