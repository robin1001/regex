/* 
 * Created on 2015-10-13
 * Author: zhangbinbin
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <set>
#include <unordered_set>

#include "fsm.h"

void write_tmp_file(const char *file_name, const char *content) {
    FILE *fp = fopen(file_name, "w");
    fprintf(fp, "%s", content);
    fclose(fp);
}


class FsmTest: public Fsm {
public:
    void test_read_topo(); 
    void test_move();
    void test_epsilon_closure(); 
    void test_run_nfa(); 
    void test_determine();
    void test_split_set_by_input();
    void test_minimize();
    void test_memory();
    void test_all();
};

void FsmTest::test_all() {
    test_read_topo();

}

void FsmTest::test_memory() {
    reset();
    int start = add_state();
    set_start(start);
    for (int i = 1; i < 4096; i++) {
        int next_state = add_state();
        add_arc(start, Arc(i, next_state));
    }
    Fsm fsm_out;
    determine(&fsm_out);
    //minimize(&fsm_out);
}

void FsmTest::test_read_topo() {
    const char *topo = "0 1 0\n"
                 	   "0 2 1\n"
                 	   "1 3 2\n"
                 	   "2 3 3\n"
                 	   "3\n"
                 	   "2\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
	assert(start() == 0);
	assert(num_states() == 4);
	assert(num_finish() == 2);
	assert(num_arcs() == 4);
    assert(num_labels() == 3);
    //fsm.write("tmp.fsm");
}

void FsmTest::test_epsilon_closure() {
    const char *topo = "0 1 0\n"
                 	   "0 2 0\n"
                 	   "1 1 1\n"
                 	   "2 2 0\n"
                 	   "1 3 3\n"
                 	   "2 3 1\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
    std::set<int> in_set, out_set, ans_set;
    in_set.insert(0);
    ans_set.insert(0), ans_set.insert(1), ans_set.insert(2);
    epsilon_closure(in_set, &out_set);
    assert(out_set == ans_set);
}

void FsmTest::test_move() {
    const char *topo = "0 1 1\n"
                 	   "0 2 1\n"
                 	   "0 3 2\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
    std::set<int> in_set, out_set, ans_set;
    in_set.insert(0);
    ans_set.insert(1), ans_set.insert(2);
    move(in_set, 1, &out_set);
    assert(out_set == ans_set);
}

void FsmTest::test_run_nfa() {
    const char *topo = "0 1 0\n"
                 	   "0 2 0\n"
                 	   "1 1 1\n"
                 	   "2 2 1\n"
                 	   "1 3 3\n"
                 	   "2 3 0\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
    int arr0[] = {3};
    std::vector<int> input0(arr0, arr0+1);
    assert(true == run_nfa(input0));
    int arr1[] = {1,3};
    std::vector<int> input1(arr1, arr1+2);
    assert(true == run_nfa(input1));
    int arr2[] = {1, 1, 3};
    std::vector<int> input2(arr2, arr2+3);
    assert(true == run_nfa(input2));
    int arr3[] = {1, 2, 3};
    std::vector<int> input3(arr3, arr3+3);
    assert(false == run_nfa(input3));
}

void FsmTest::test_determine() {
    const char *topo = "0 1 0\n"
                 	   "0 7 0\n"
                 	   "1 2 0\n"
                 	   "1 4 0\n"
                 	   "2 3 1\n"
                 	   "3 6 0\n"
                 	   "4 5 2\n"
                 	   "5 6 0\n"
                 	   "6 1 0\n"
                 	   "6 7 0\n"
                 	   "7 8 1\n"
                 	   "8 9 2\n"
                 	   "9\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
	Fsm fsm_out;
    determine(&fsm_out);
    //std::cerr << fsm_out.num_states() << "\n";
    assert(fsm_out.num_states() == 4);
    assert(fsm_out.num_arcs() == 8);
    assert(fsm_out.num_finish() == 1);
    //fsm_out.fsm_info();
}

void FsmTest::test_split_set_by_input() {
    const char *topo = "0 1 1\n"
                 	   "0 2 2\n"
                 	   "1 3 3\n"
                 	   "2 3 3\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
    std::set<int> in_set;
    std::unordered_set<std::set<int> > out_sets; 
    in_set.insert(1), in_set.insert(2), in_set.insert(3);
    split_set_by_input(in_set, 1, &out_sets);
    assert(out_sets.size() == 1);
    out_sets.clear();
    split_set_by_input(in_set, 3, &out_sets);
    assert(out_sets.size() == 2);
}

void FsmTest::test_minimize() {
    const char *topo = "0 1 1\n"
                 	   "0 2 2\n"
                 	   "1 3 3\n"
                 	   "2 3 3\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
    read_topo(tmpfile);
	Fsm fsm_out;
    this->minimize(&fsm_out);
    //std::cerr << fsm_out.num_states() << " " << fsm_out.num_arcs() << "\n";
    assert(fsm_out.num_states() == 3);
    assert(fsm_out.num_arcs() == 3);
}

int main() {
    // - Test read 
    FsmTest fsm_test;
    fsm_test.test_read_topo();
    // - Test epsilon_closure
    fsm_test.test_epsilon_closure();
    // - Test move
    fsm_test.test_move(); 
    // - Test run_nfa
    fsm_test.test_run_nfa();
    // - Test determine
    fsm_test.test_determine();
    // - Test split by input
    fsm_test.test_split_set_by_input();
    // - Test fsm Minimize()
    fsm_test.test_minimize();
    // - Test fsm memory if stack overflow on big state size
    fsm_test.test_memory();

    return 0;
}

