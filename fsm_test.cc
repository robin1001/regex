/* 
 * Created on 2015-10-13
 * Author: zhangbinbin
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fsm.h"

void write_tmp_file(const char *file_name, const char *content) {
    FILE *fp = fopen(file_name, "w");
    fprintf(fp, "%s", content);
    fclose(fp);
}

void test_read_topo() {
    const char *topo = "0 1 1\n"
                 	   "0 2 1\n"
                 	   "1 3 2\n"
                 	   "2 3 3\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
	Fsm fsm;
	fsm.read_topo(tmpfile);
	assert(fsm.start() == 0);
	assert(fsm.num_states() == 4);
	assert(fsm.end() == 3);
	assert(fsm.num_arcs() == 4);
    //fsm.write("tmp.fsm");
}


int main() {
    test_read_topo();
}

