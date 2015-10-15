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
public:
    Fsm(): start_(0), end_(0) {}
    ~Fsm(); 
    void reset();
    int start() const { 
        return start_; 
    }
	int end() const {
		return end_;
	}
    void set_start(int id) {
		assert(id < states_.size());
        start_ = id;
    }
    void set_end(int id) {
		assert(id < states_.size());
        end_ = id;
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
    int add_state();
    void add_arc(int id, const Arc &arc);
    void read_topo(const char *file);
	void read(const char *file); //read fsm from file
	void write(const char *file) const; // write fsm to file
    void fsm_info () const; 
    bool run_nfa(std::vector<int> &input) const;
protected:
    void step_epsilon(State *state, std::set<int> *list) const; 
protected:
    int start_;
    int end_;
    std::vector<State *> states_;
};

#endif
