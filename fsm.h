/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */
#ifndef FSM_H_

#include <stdio.h>
#include <assert.h>

#include <vector>
#include <set>

#include "utils.h"


struct Arc {
    Arc(int ilabel_, int next_state_): ilabel(ilabel_), 
        next_state(next_state_) {}
    int ilabel;
    int next_state;
};

typedef std::vector<Arc> Arcs;
typedef std::vector<Arc>::iterator ArcIterator;

struct State {
    State() {}
    Arcs arcs;
	void add_arc(const Arc &arc) {
		arcs.push_back(arc);
	}
	int num_arcs() const {
		return arcs.size();
	}
};

// Finite State Machine
class Fsm {
    typedef std::set<int>::const_iterator SetIterator;
public:

    Fsm(): start_(0) {}
    Fsm(const char *topo_file);
    ~Fsm(); 
    void reset();
    int start() const { 
        return start_; 
    }
    void set_start(int id) {
		assert(id < states_.size());
        start_ = id;
    }
    void set_finish(int id) {
		assert(id < states_.size());
        finish_set_.insert(id);
    }
    int num_finish() const {
        return finish_set_.size();
    }
    int num_states() const {
        return states_.size();
    }
    int num_arcs() const {
		int count = 0;
		for (int i = 0; i < states_.size(); i++) {
			count += states_[i]->num_arcs();
		}
		return count;
    }
    int num_labels() const {
        return label_sets_.size();
    }
    int add_state();
    void add_arc(int id, const Arc &arc);
    void read_topo(const char *file);
	void read(const char *file); //read fsm from file
	void write(const char *file) const; // write fsm to file
    void fsm_info () const; 
    bool run_nfa(const std::vector<int> &input) const;
    void determine(Fsm *fsm_out) const; 
    void minimize(Fsm *fsm_out) const; 
    void epsilon_closure(const std::set<int> &in_set, 
                         std::set<int> *out_set) const; 
    void move(const std::set<int> &in_set, 
              int label, 
              std::set<int> *out_set) const; 
    bool is_finish(const std::set<int> &t) const;
    bool is_finish(int id) const;
protected:
    int start_;
    std::set<int> finish_set_;
    std::vector<State *> states_;
    std::set<int> label_sets_;
};

#endif
