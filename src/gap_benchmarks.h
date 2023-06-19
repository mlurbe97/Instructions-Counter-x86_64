/*
## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
*/

#ifndef gap_benchmarks  /* Include guard */
#define gap_benchmarks

// Intel Hybrid
#include "Intel_Core.h"

// System libs
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/poll.h>
#include <sched.h>
#include <omp.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>
#include <dlfcn.h>

/*************************************************************
 **                   Defines                               **
 *************************************************************/

#define NUM_BENCHMARKS 16 // Number of available benchmarks.

/*************************************************************
 **                   Benchmarks SPEC2006-2017              **
 *************************************************************/

char *benchmarks[NUM_BENCHMARKS][200] = {
    //* GAP *//
    // 0 -> bfs -g 24
    {"benchmarks/gapbs/bfs", "-g", "24", NULL},
    // 1 -> bfs -u 24
    {"benchmarks/gapbs/bfs", "-u", "24", NULL},
    // 2 -> bc -g 24
    {"benchmarks/gapbs/bc", "-g", "24", NULL},
    // 3 -> bc -u 24
    {"benchmarks/gapbs/bc", "-u", "24", NULL},
    // 4 -> cc -g 24
    {"benchmarks/gapbs/cc", "-g", "24", NULL},
    // 5 -> cc -u 24
    {"benchmarks/gapbs/cc", "-u", "24", NULL},
    // 6 -> pr -g 24
    {"benchmarks/gapbs/pr", "-g", "24", NULL},
    // 7 -> pr -u 24
    {"benchmarks/gapbs/pr", "-u", "24", NULL},
    // 8 -> sssp -g 24
    {"benchmarks/gapbs/sssp", "-g", "24", NULL},
    // 9 -> sssp -u 24
    {"benchmarks/gapbs/sssp", "-u", "24", NULL},
    // 10 -> tc -g 24
    {"benchmarks/gapbs/tc", "-g", "24", NULL},
    // 11 -> tc -u 24
    {"benchmarks/gapbs/tc", "-u", "24", NULL},
    // 12 -> cc_sv -g 24
    {"benchmarks/gapbs/cc_sv", "-g", "24", NULL},
    // 13 -> cc_sv -u 24
    {"benchmarks/gapbs/cc_sv", "-u", "24", NULL},
    // 14 -> pr_spmv -g 24
    {"benchmarks/gapbs/pr_spmv", "-g", "24", NULL},
    // 15  -> pr_spmv -u 24
    {"benchmarks/gapbs/pr_spmv", "-u", "24", NULL},
};

/*************************************************************
 **                   Name of the benchmarks	            **
 *************************************************************/

char *bench_Names [NUM_BENCHMARKS] = {
	"bfs_g_24","bfs_u_24","bc_g_24","bc_u_24","cc_g_24","cc_u_24","pr_g_24","pr_u_24",//0--7
    "sssp_g_24","sssp_u_24","tc_g_24","tc_u_24","cc_sv_g_24","cc_sv_u_24","pr_spmv_g_24","pr_spmv_u_24"//8--15
};

/*************************************************************
 **                 search_benchmark                        **
 *************************************************************/

int search_benchmark(int prg,FILE **in_file) {
    int res = 0;
    close(1);
    *in_file = fopen("rundir/sal.log", "w");
    close(2);
    *in_file = fopen("rundir/err.log", "w");
    return res;
}

#endif