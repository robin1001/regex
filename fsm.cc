/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */
#include <string.h>

#include <iostream>
#include <queue>
#include <unordered_map>

#include "fsm.h"

Fsm::~Fsm() {
    reset();
}

void Fsm::reset() {
    start_ = 0;
    for (int i = 0; i < states_.size(); i++) {
        if (states_[i] != NULL) {
            delete states_[i];
        }
    }
    states_.clear();
    label_sets_.clear();
    finish_set_.clear();
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
            set_finish(src);
        }
        else {
            break;
            // mini_error("wrong line, expected src, dest, label"); 
        }
    }
    fclose(fp);
}


/* 	fsm read write file format 
  	num_states(int) num_arcs(int)
	start_(int) 
    finish_set_(int)
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
    int states_num, arcs_num, finish_num;
    int ilabel, next_state;
    fread(&states_num, sizeof(int), 1, fp);
    fread(&arcs_num, sizeof(int), 1, fp);
    fread(&start_, sizeof(int), 1, fp);
    fread(&finish_num, sizeof(int), 1, fp);
    //std::cerr << finish_num;
    for (int i = 0; i < states_num; i++) {
        states_.push_back(new State());
    }
    for (int i = 0; i < finish_num; i++) {
        fread(&next_state, sizeof(int), 1, fp);
        finish_set_.insert(next_state);
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
    int finish_num = num_finish();
    fwrite(&states_num, sizeof(int), 1, fp);
    fwrite(&arcs_num, sizeof(int), 1, fp);
    fwrite(&start_, sizeof(int), 1, fp);
    fwrite(&finish_num, sizeof(int), 1, fp);
    for (SetIterator it = finish_set_.begin(); it != finish_set_.end(); it++) {
        fwrite(&(*it), sizeof(int), 1, fp);
    }
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
    fprintf(stderr, "finish set:\t%d { ", num_finish()); 
    for (SetIterator it = finish_set_.begin(); it != finish_set_.end(); it++) {
        fprintf(stderr, "%d ", *it); 
    }
    fprintf(stderr, "}\n");
    for (int i = 0; i < states_.size(); i++) {
        fprintf(stderr, "state %d arcs %d: { ", i, states_[i]->num_arcs());
        for (int j = 0; j < states_[i]->num_arcs(); j++) {
            fprintf(stderr, "[%d, %d] ", states_[i]->arcs[j].ilabel, \
                                states_[i]->arcs[j].next_state);
        }
        fprintf(stderr, "}\n");
    }
}
// if in_set contains any finish state
bool Fsm::is_finish(const std::set<int> &in_set) const {
    for (SetIterator it = in_set.begin(); it != in_set.end(); it++) {
        if (is_finish(*it)) {
            return true;
        }
    }
    return false;
}

bool Fsm::is_finish(int id) const {
    return (finish_set_.find(id) != finish_set_.end());
}

void Fsm::epsilon_closure(const std::set<int> &in_set, 
                          std::set<int> *out_set) const {
    std::queue<int> q;
    for (SetIterator it = in_set.begin(); it != in_set.end(); it++) {
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

void Fsm::move(const std::set<int> &in_set, 
               int label, 
               std::set<int> *out_set) const {
    assert(out_set != NULL);
    for (SetIterator it = in_set.begin(); it != in_set.end(); it++) {
        State *state = states_[*it];
        for (int j = 0; j < state->num_arcs(); j++) {
            if (label == state->arcs[j].ilabel) {
                out_set->insert(state->arcs[j].next_state);
            }
        }
    }
}


bool Fsm::run_nfa(const std::vector<int> &input) const {
    std::set<int> current_set, tmp_set, next_set;
    tmp_set.insert(start_);
    // Step epsilon, add it in current list
    epsilon_closure(tmp_set, &current_set);
    tmp_set.clear();

    // Simulate nfa
    for (int i = 0; i < input.size(); i++) { 
        // Loop all current state with input[i]
        move(current_set, input[i], &tmp_set);
        /* next_set is epsilon the epsilon_closure of tmp_set
         * It can be implimented with recurrsive calls
         * We now impliment it with width first search for efficiency 
         */
        epsilon_closure(tmp_set, &next_set);
        //std::cerr << "next state set ";
        //for (SetIterator it = next_set.begin(); it != next_set.end(); it++) {
        //    std::cerr << *it << " ";
        //}
        //std::cerr << "\n";

        // Exchange current and next list
        current_set.swap(next_set);
        next_set.clear();
        tmp_set.clear();
        if (current_set.empty()) break;
    }
    // Judge whether end in the final current set
    return is_finish(current_set);
}

// Provide hash fuction for std::set<int>
namespace std {
template <>
class hash<std::set<int> > {
public:
    size_t operator()(const std::set<int> &t) const {
        typedef std::set<int>::const_iterator SetIterator;
        size_t sum = 0;
        for (SetIterator it = t.begin(); it != t.end(); it++) {
            sum += *it * 131;
        }
        return sum;
        //return reinterpret_cast<size_t>(&t);
    }
};

}

void Fsm::determine(Fsm *fsm_out) const {
    assert(fsm_out != NULL);
    fsm_out->reset();
    //typedef std::unordered_map<std::set<int>, int>::const_iterator TableIterator;
    std::unordered_map<std::set<int>, int> table;
    std::set<int> current_set, tmp_set, next_set;
    tmp_set.insert(start_);
    epsilon_closure(tmp_set, &current_set);
    int start = fsm_out->add_state();
    fsm_out->set_start(start);
    if (is_finish(current_set)) fsm_out->set_finish(start);
    table.insert(std::make_pair(current_set, start));
    std::queue<std::set<int> > q;
    q.push(current_set);

    while (!q.empty()) {
        current_set = q.front();
        int src_state = table[current_set];
        q.pop();
        for (SetIterator it = label_sets_.begin(); it != label_sets_.end(); it++) {
            tmp_set.clear();
            next_set.clear();
            // Move
            move(current_set, *it, &tmp_set);
            // Epsilon closure
            epsilon_closure(tmp_set, &next_set);
            if (!next_set.empty()) {
                int dest_state = 0;
                // New state
                if (table.find(next_set) == table.end()) {
                    dest_state = fsm_out->add_state();
                    if (is_finish(next_set)) fsm_out->set_finish(dest_state);
                    table.insert(std::make_pair(next_set, dest_state));
                    q.push(next_set);
                } else { // Not new, find it in table
                    dest_state = table[next_set];
                }
                fsm_out->add_arc(src_state, Arc(*it, dest_state));
            }
        } // for iterator
    }
}

void minimize(Fsm *fsm_out) const {
}

