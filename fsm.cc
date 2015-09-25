/* 
 * Created on 2015-09-25
 * Author: zhangbinbin
 */

#include "fsm.h"


Fsm::~Fsm() {
    for (size_t i = 0; i < states_.size(); i++) {
        if (states_[i] != NULL) {
            delete states_[i];
        }
    }
}

StateId Fsm::add_state() {
    StateId id = states_.size();
    states_.push_back(new State());
    return id;
}



