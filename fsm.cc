/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */
#include <string.h>

#include <iostream>
#include <queue>

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
    label_sets_.clear();
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
    if (arc.ilabel != 0) label_sets_.insert(arc.ilabel);
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
            if (label != 0) label_sets_.insert(label);
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
    label_set_ num_labels(int) [label1, label2, ...] 
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
            if (ilabel != 0) label_sets_.insert(ilabel);
            states_[i]->add_arc(Arc(ilabel, next_state));
        }
    }
    assert(arcs_num == num_arcs());
    if (fp != stdin) {
        fclose(fp);
    }
}

// Write fsm to file
void Fsm::write(const char *file) const { 
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

// Show the text format fsm info 
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

void Fsm::step_epsilon(State *state, std::set<int> *list) const {
    for (int i = 0; i < state->num_arcs(); i++) {
        if (0 == state->arcs[i].ilabel) {
            int state_id = state->arcs[i].next_state;
            assert(state_id < states_.size());
            if (list->find(state_id) == list->end()) {
                list->insert(state_id);
                step_epsilon(states_[state_id], list);
            }
        }
    }
}

void Fsm::epsilon_closure(std::set<int> &in_set, std::set<int> *out_set) const {
    typedef std::set<int>::const_iterator Iterator;
    out_set->clear();
    std::queue<int> q;
    for (Iterator it = in_set.begin(); it != in_set.end(); it++) {
        q.push(*it);
        out_set->insert(*it);
    }
    while (!q.empty()) {
        int id = q.front();
        q.pop();
        assert(id < states_.size());
        for (int i = 0; i < states_[id]->num_arcs(); i++) {
            if (0 == states_[id]->arcs[i].ilabel) {
                int next_state = states_[id]->arcs[i].next_state;
                if (out_set->find(next_state) == out_set->end()) {
                    out_set->insert(next_state);
                    q.push(next_state);
                }
            }
        }
    }
}

bool Fsm::run_nfa(std::vector<int> &input) const {
    typedef std::set<int>::const_iterator Iterator;
    std::set<int> current_set, tmp_set, next_set;
    tmp_set.insert(start_);
    // Step epsilon, add it in current list
    epsilon_closure(tmp_set, &current_set);
    tmp_set.clear();

    // Simulate nfa
    for (int i = 0; i < input.size(); i++) { 
        // Loop all current state with input[i]
        for (Iterator it = current_set.begin(); it != current_set.end(); it++) {
            State *state = states_[*it];
            for (int j = 0; j < state->num_arcs(); j++) {
                if (input[i] == state->arcs[j].ilabel) {
                    tmp_set.insert(state->arcs[j].next_state);
                }
            }
        }
        /* next_set is epsilon the epsilon_closure of tmp_set
         * It can be implimented with recurrsive calls
         * We now impliment it with width first search for efficiency 
         */
        epsilon_closure(tmp_set, &next_set);

        std::cerr << "next state set ";
        for (Iterator it = next_set.begin(); it != next_set.end(); it++) {
            std::cerr << *it << " ";
        }
        std::cerr << "\n";

        // Exchange current and next list
        current_set.swap(next_set);
        next_set.clear();
        tmp_set.clear();
        if (current_set.empty()) break;
    }
    // Judge whether end in the final current set
    Iterator it = current_set.find(end_);
    if (it != current_set.end()) return true;
    else return false;
}


void Fsm::determine(Fsm *fsm_out) const {
    assert(fsm_out != NULL);
    fsm_out->reset();
}
