/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */
#include <string.h>

#include "fsm.h"

Fsm::~Fsm() {
    reset();
}

void Fsm::reset() {
    start_ = end_ = 0;
    for (int i = 0; i < states_.size(); i++) {
        if (states_[i] != NULL) {
            delete states_[i];
        }
    }
    states_.clear();
}

int Fsm::add_state() {
    int id = states_.size();
    states_.push_back(new State());
    return id;
}

void Fsm::add_arc(int id, const Arc &arc) {
    int max = id > arc.next_state ? id : arc.next_state;
	int old_size = states_.size();
    if (max >= old_size) {
		for (int i = old_size; i < max + 1; i++) {
			states_.push_back(new State());
		}
    }
	assert(id < states_.size());
	states_[id]->add_arc(arc);
}


/* read fsm from text file, eg:
   src  dest    input //src_state dest_state
   0    1       1
   ...
   n
   attention: first line src must be start state,
   only one src in the last line as the final state
*/
void Fsm::read_topo(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        mini_error("file %s not exist", file);
    }
    char buffer[1024];
    int src, dest, label;
    bool first_line = true;
    while(fgets(buffer, 1024, fp)) {
        int num = sscanf(buffer, "%d %d %d", &src, &dest, &label);
		//mini_log("read arc %d %d %d", src, dest, label);
        if (num == 3) {
			add_arc(src, Arc(label, dest));	
            if (first_line) {
                set_start(src);
                first_line = false; 
            }
        }
        else if (num == 1) {//final state
            set_end(src);
            break;
        }
        else {
            mini_error("wrong line, expected src, dest, label"); 
        }
    }
    fclose(fp);
}


/* 	fsm read write file format 
  	num_states(int) num_arcs(int)
	start_(int) end_(int)
	every state's arcs info: num_arcs(int) {ilabel(int) next_state(int) ...}
*/
void Fsm::read(const char *file) {
    reset();
    FILE *fp = NULL;
    if (strcmp(file, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(file, "rb");
    }
    if (!fp) {
        mini_error("file %s not exist", file);
    }
    int states_num, arcs_num;
    int ilabel, next_state;
    fread(&states_num, sizeof(int), 1, fp);
    fread(&arcs_num, sizeof(int), 1, fp);
    fread(&start_, sizeof(int), 1, fp);
    fread(&end_, sizeof(int), 1, fp);
    for (int i = 0; i < states_num; i++) {
        states_.push_back(new State());
    }
    for (int i = 0; i < states_num; i++) {
        int state_arcs = 0;
        fread(&state_arcs, sizeof(int), 1, fp);
        for (int j = 0; j < state_arcs; j++) {
            fread(&ilabel, sizeof(int), 1, fp);
            fread(&next_state, sizeof(int), 1, fp);
            states_[i]->add_arc(Arc(ilabel, next_state));
        }
    }
    assert(arcs_num == num_arcs());
    if (fp != stdin) {
        fclose(fp);
    }
}

void Fsm::write(const char *file) const { // write fsm to file
    FILE *fp = NULL;
    if (strcmp(file, "-") == 0) {
        fp = stdout;
    }
    else {
        fp = fopen(file, "wb");
    }
    int states_num = num_states(), arcs_num = num_arcs();
    fwrite(&states_num, sizeof(int), 1, fp);
    fwrite(&arcs_num, sizeof(int), 1, fp);
    fwrite(&start_, sizeof(int), 1, fp);
    fwrite(&end_, sizeof(int), 1, fp);
    for (int i = 0; i < states_num; i++) {
        int state_arcs = states_[i]->num_arcs();
        fwrite(&state_arcs, sizeof(int), 1, fp);
        for (int j = 0; j < state_arcs; j++) {
            fwrite(&states_[i]->arcs[j].ilabel, sizeof(int), 1, fp);
            fwrite(&states_[i]->arcs[j].next_state, sizeof(int), 1, fp);
        }
    }
    if (fp != stdout) {
        fclose(fp);
    }
}

// show the text format fsm info 
void Fsm::fsm_info() const {
    fprintf(stderr, "fsm info table\n");
    fprintf(stderr, "num_states:\t%d\n", num_states()); 
    fprintf(stderr, "num_arcs:\t%d\n", num_states()); 
    fprintf(stderr, "start id:\t%d\n", start_); 
    fprintf(stderr, "end id:\t%d\n", end_); 
    for (int i = 0; i < states_.size(); i++) {
        fprintf(stderr, "state %d arcs %d: { ", i, states_[i]->num_arcs());
        for (int j = 0; j < states_[i]->num_arcs(); j++) {
            fprintf(stderr, "[%d, %d] ", states_[i]->arcs[j].ilabel, \
                                states_[i]->arcs[j].next_state);
        }
        fprintf(stderr, "}\n");
    }
}
