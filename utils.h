/* Created on 2015-10-12
 * Author: zhangbinbin
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>

#define mini_log(fmt, arg...) \
do { \
    fprintf(stdout, "LOG (%s:%s():%d)", __FILE__, __func__, __LINE__); \
    fprintf(stdout, fmt, ##arg); \
    fprintf(stdout, "\n"); \
} while(0)


#define mini_error(fmt, arg...) \
do { \
    fprintf(stderr, "ERROR (%s:%s():%d)", __FILE__, __func__, __LINE__); \
    fprintf(stderr, fmt, ##arg); \
    fprintf(stderr, "\n"); \
    exit(-1); \
} while(0)


#endif


