#include <stdio.h>

#include "utils.h"
#include "fsm.h"

int main(int argc, char *argv[]) {
    const char *usage = "Copy fsm model in file to out file, mainly for pipe usage, "
                        "if out_file is not provided, copy to the stdout stream\n"
                        "Usage: fsm-copy in_file [out_file]\n"
                        "eg: fsminfo in.fsm out.fsm\n";
    if (argc < 2 && argc > 3) {
        printf(usage);
        return -1;
    }

    Fsm fsm;
    fsm.read(argv[1]);
    if (argc == 2) {
        fsm.write("-");
    }
    else {
        fsm.write(argv[2]);
    }
    return 0;
}

