#include <stdio.h>

#include "utils.h"
#include "fsm.h"

int main(int argc, char *argv[]) {
    const char *usage = "Showing details text information of fsm format file\n"
                        "Usage: fsminfo fsm_file\n"
                        "eg: fsminfo re.fsm\n";
    if (argc != 2) {
        printf(usage);
        return -1;
    }
    Fsm fsm;
    fsm.read(argv[1]);
    fsm.fsm_info();
    return 0;
}

