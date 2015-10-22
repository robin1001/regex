/* Created on 2015-10-12
 * Author: zhangbinbin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

#include <set>
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


class SetIntHash {
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

class SetIntEqual {
public:
    bool operator() (const std::set<int> &s1, const std::set<int> &s2) const {
        typedef std::set<int>::const_iterator SetIterator;
        if (s1.size() != s2.size()) return false;
        SetIterator iter1 = s1.begin(), iter2 = s2.begin();
        for (; iter1 != s1.end(); iter1++, iter2++) {
            if (*iter1 != *iter2) return false;
        }
        return true;
    }
};

#endif


