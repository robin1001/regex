/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */
#ifndef FSM_H_
#include <stdio.h>
#include <vector>
#include <assert.h>

typedef char Label;
typedef size_t StateId;

struct Arc {
    Arc(Label ilabel_, StateId next_state_): ilabel(ilabel_), 
        next_state(next_state_) {}
    Label ilabel;
    StateId next_state;
};

typedef std::vector<Arc> Arcs;
typedef std::vector<Arc>::iterator ArcIterator;

struct State {
    State() {}
    size_t num_arcs() const{ return arcs.size(); }
    Arcs arcs;
};

//Finite State Machine
class Fsm {
public:
    Fsm(): start_(0) {}
    ~Fsm(); 
    StateId start() const { 
        return start_; 
    }
    void set_start(StateId start) {
        start_ = start;
    }
    size_t num_states() const {
        return states_.size();
    }
    StateId add_state();
private:
    StateId start_;
    std::vector<State *> states_;
};

#endif
