/* 
 * Created on 2015-10-13
 * Author: zhangbinbin
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fsm.h"

Fsm::Fsm(const char *topo_file) {
    read_topo(topo_file);
}

void write_tmp_file(const char *file_name, const char *content) {
    FILE *fp = fopen(file_name, "w");
    fprintf(fp, "%s", content);
    fclose(fp);
}

void test_read_topo() {
    const char *topo = "0 1 0\n"
                 	   "0 2 1\n"
                 	   "1 3 2\n"
                 	   "2 3 3\n"
                 	   "3\n"
                 	   "2\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
	Fsm fsm(tmpfile);
	assert(fsm.start() == 0);
	assert(fsm.num_states() == 4);
	assert(fsm.num_finish() == 2);
	assert(fsm.num_arcs() == 4);
    assert(fsm.num_labels() == 3);
    //fsm.write("tmp.fsm");
}

void test_epsilon_closure() {
    const char *topo = "0 1 0\n"
                 	   "0 2 0\n"
                 	   "1 1 1\n"
                 	   "2 2 0\n"
                 	   "1 3 3\n"
                 	   "2 3 1\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
	Fsm fsm(tmpfile);
    std::set<int> in_set, out_set, ans_set;
    in_set.insert(0);
    ans_set.insert(0), ans_set.insert(1), ans_set.insert(2);
    fsm.epsilon_closure(in_set, &out_set);
    assert(out_set == ans_set);
}

void test_move() {
    const char *topo = "0 1 1\n"
                 	   "0 2 1\n"
                 	   "0 3 2\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
	Fsm fsm(tmpfile);
    std::set<int> in_set, out_set, ans_set;
    in_set.insert(0);
    ans_set.insert(1), ans_set.insert(2);
    fsm.move(in_set, 1, &out_set);
    assert(out_set == ans_set);
}

void test_run_nfa() {
    const char *topo = "0 1 0\n"
                 	   "0 2 0\n"
                 	   "1 1 1\n"
                 	   "2 2 1\n"
                 	   "1 3 3\n"
                 	   "2 3 0\n"
                 	   "3\n";
    const char *tmpfile = "tmp.topo";
	write_tmp_file(tmpfile, topo);
	Fsm fsm(tmpfile);
    int arr0[] = {3};
    std::vector<int> input0(arr0, arr0+1);
    assert(true == fsm.run_nfa(input0));
    int arr1[] = {1,3};
    std::vector<int> input1(arr1, arr1+2);
    assert(true == fsm.run_nfa(input1));
    int arr2[] = {1, 1, 3};
    std::vector<int> input2(arr2, arr2+3);
    assert(true == fsm.run_nfa(input2));
    int arr3[] = {1, 2, 3};
    std::vector<int> input3(arr3, arr3+3);
    assert(false == fsm.run_nfa(input3));
}

void test_determine() {
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
	Fsm fsm_in(tmpfile), fsm_out;
    fsm_in.determine(&fsm_out);
    assert(fsm_out.num_states() == 4);
    assert(fsm_out.num_arcs() == 8);
    assert(fsm_out.num_finish() == 1);
    //fsm_out.fsm_info();
}

int main() {
    // - Test read 
    test_read_topo();
    // - Test epsilon_closure
    test_epsilon_closure();
    // - Test move
    test_move(); 
    // - Test run_nfa
    test_run_nfa();
    // - Test determine
    test_determine();
}

