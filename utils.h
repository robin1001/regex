/* Created on 2015-10-12
 * Author: zhangbinbin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <unordered_set>
#include <unordered_map>
#include <functional>

#define mini_log(fmt, args...) \
do { \
    fprintf(stderr, "LOG (%s:%s():%d)", __FILE__, __func__, __LINE__); \
    fprintf(stderr, fmt, ##args); \
    fprintf(stderr, "\n"); \
} while(0)


#define mini_error(fmt, args...) \
do { \
    fprintf(stderr, "ERROR (%s:%s():%d)", __FILE__, __func__, __LINE__); \
    fprintf(stderr, fmt, ##args); \
    fprintf(stderr, "\n"); \
    exit(-1); \
} while(0)



// Provide hash fuction for std::set<int>
//namespace std {
//template <>
//class hash<std::set<int> > {
//public:
//    size_t operator()(const std::set<int> &t) const {
//        typedef std::set<int>::const_iterator SetIterator;
//        size_t sum = 0;
//        for (SetIterator it = t.begin(); it != t.end(); it++) {
//            sum += *it * 131;
//        }
//        return sum;
//        //return reinterpret_cast<size_t>(&t);
//    }
//};
//}

typedef std::unordered_set<int> Set;
typedef Set::const_iterator SetIter;
//typedef std::set<int> Set;
//typedef Set::iterator SetIter;

class SetIntHash {
public:
    size_t operator()(const Set &t) const {
        size_t sum = 0;
        for (SetIter it = t.begin(); it != t.end(); it++) {
            sum += *it * 131;
        }
        return sum;
        //return reinterpret_cast<size_t>(&t);
    }
};

class SetIntEqual {
public:
    bool operator() (const Set &s1, const Set &s2) const {
        if (s1.size() != s2.size()) return false;
        SetIter iter1 = s1.begin(), iter2 = s2.begin();
        for (; iter1 != s1.end(); iter1++, iter2++) {
            if (*iter1 != *iter2) return false;
        }
        return true;
    }
};


typedef std::unordered_map<Set, int, SetIntHash, SetIntEqual> SetTable;
typedef SetTable::iterator SetTableIter;

typedef std::unordered_set<Set, SetIntHash, SetIntEqual> SetSet;
typedef SetSet::iterator SetSetIter;

#endif

