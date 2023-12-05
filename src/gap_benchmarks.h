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

#define NUM_BENCHMARKS 46 // Number of available benchmarks.

/*************************************************************
 **                   Benchmarks SPEC2006-2017              **
 *************************************************************/

char *benchmarks[NUM_BENCHMARKS][200] = {
    //* GAP Sintetics *//

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

    //* GAP Non-Sintetics *//
    
    // 16 -> bfs twitter
    {"benchmarks/gapbs/bfs", "-f", "benchmarks/gapbs/benchmark/graphs/twitter.sg", "-n64", ">", "benchmarks/gapbs/benchmark/out/bfs-twitter.out", NULL},
    // 17 -> pr twitter
    {"benchmarks/gapbs/pr", "-f", "benchmarks/gapbs/benchmark/graphs/twitter.sg", "-i1000", "-t1e-4", "-n16", ">", "benchmarks/gapbs/benchmark/out/pr-twitter.out", NULL},
    // 18 -> cc twitter
    {"benchmarks/gapbs/cc", "-f", "benchmarks/gapbs/benchmark/graphs/twitter.sg", "-n16", ">", "benchmarks/gapbs/benchmark/out/cc-twitter.out", NULL},
    // 19 -> bc twitter
    {"benchmarks/gapbs/bc", "-f", "benchmarks/gapbs/benchmark/graphs/twitter.sg", "-i4", "-n16", ">", "benchmarks/gapbs/benchmark/out/bc-twitter.out",NULL},
    
    // 20 -> bfs web
    {"benchmarks/gapbs/bfs", "-f", "benchmarks/gapbs/benchmark/graphs/web.sg", "-n64", ">", "benchmarks/gapbs/benchmark/out/bfs-web.out", NULL},
    // 21 -> pr web
    {"benchmarks/gapbs/pr", "-f", "benchmarks/gapbs/benchmark/graphs/web.sg", "-i1000", "-t1e-4", "-n16", ">", "benchmarks/gapbs/benchmark/out/pr-web.out", NULL},
    // 22 -> cc web
    {"benchmarks/gapbs/cc", "-f", "benchmarks/gapbs/benchmark/graphs/web.sg", "-n16", ">", "benchmarks/gapbs/benchmark/out/cc-web.out", NULL},
    // 23 -> bc web
    {"benchmarks/gapbs/bc", "-f", "benchmarks/gapbs/benchmark/graphs/web.sg", "-i4", "-n16", ">", "benchmarks/gapbs/benchmark/out/bc-web.out",NULL},
    
    // 24 -> bfs road
    {"benchmarks/gapbs/bfs", "-f", "benchmarks/gapbs/benchmark/graphs/road.sg", "-n64", ">", "benchmarks/gapbs/benchmark/out/bfs-road.out", NULL},
    // 25 -> pr road
    {"benchmarks/gapbs/pr", "-f", "benchmarks/gapbs/benchmark/graphs/road.sg", "-i1000", "-t1e-4", "-n16", ">", "benchmarks/gapbs/benchmark/out/pr-road.out", NULL},
    // 26 -> cc road
    {"benchmarks/gapbs/cc", "-f", "benchmarks/gapbs/benchmark/graphs/road.sg", "-n16", ">", "benchmarks/gapbs/benchmark/out/cc-road.out", NULL},
    // 27 -> bc road
    {"benchmarks/gapbs/bc", "-f", "benchmarks/gapbs/benchmark/graphs/road.sg", "-i4", "-n16", ">", "benchmarks/gapbs/benchmark/out/bc-road.out",NULL},

    // 28 -> bfs kron
    {"benchmarks/gapbs/bfs", "-f", "benchmarks/gapbs/benchmark/graphs/kron.sg", "-n64", ">", "benchmarks/gapbs/benchmark/out/bfs-kron.out", NULL},
    // 29 -> pr kron
    {"benchmarks/gapbs/pr", "-f", "benchmarks/gapbs/benchmark/graphs/kron.sg", "-i1000", "-t1e-4", "-n16", ">", "benchmarks/gapbs/benchmark/out/pr-kron.out", NULL},
    // 30 -> cc kron
    {"benchmarks/gapbs/cc", "-f", "benchmarks/gapbs/benchmark/graphs/kron.sg", "-n16", ">", "benchmarks/gapbs/benchmark/out/cc-kron.out", NULL},
    // 31 -> bc kron
    {"benchmarks/gapbs/bc", "-f", "benchmarks/gapbs/benchmark/graphs/kron.sg", "-i4", "-n16", ">", "benchmarks/gapbs/benchmark/out/bc-kron.out",NULL},

    // 32 -> bfs urand
    {"benchmarks/gapbs/bfs", "-f", "benchmarks/gapbs/benchmark/graphs/urand.sg", "-n64", ">", "benchmarks/gapbs/benchmark/out/bfs-urand.out", NULL},
    // 33 -> pr urand
    {"benchmarks/gapbs/pr", "-f", "benchmarks/gapbs/benchmark/graphs/urand.sg", "-i1000", "-t1e-4", "-n16", ">", "benchmarks/gapbs/benchmark/out/pr-urand.out", NULL},
    // 34 -> cc urand
    {"benchmarks/gapbs/cc", "-f", "benchmarks/gapbs/benchmark/graphs/urand.sg", "-n16", ">", "benchmarks/gapbs/benchmark/out/cc-urand.out", NULL},
    // 35 -> bc urand
    {"benchmarks/gapbs/bc", "-f", "benchmarks/gapbs/benchmark/graphs/urand.sg", "-i4", "-n16", ">", "benchmarks/gapbs/benchmark/out/bc-urand.out",NULL},

    // 36 -> tc kron
    {"benchmarks/gapbs/tc", "-f", "benchmarks/gapbs/benchmark/graphs/kronU.sg", "-n3", ">", "benchmarks/gapbs/benchmark/out/tc-kron.out",NULL},
    // 37 -> tc urand
    {"benchmarks/gapbs/tc", "-f", "benchmarks/gapbs/benchmark/graphs/urandU.sg", "-n3", ">", "benchmarks/gapbs/benchmark/out/tc-urand.out",NULL},
    // 38 -> tc twitter
    {"benchmarks/gapbs/tc", "-f", "benchmarks/gapbs/benchmark/graphs/twitterU.sg", "-n3", ">", "benchmarks/gapbs/benchmark/out/tc-twitter.out",NULL},
    // 39 -> tc web
    {"benchmarks/gapbs/tc", "-f", "benchmarks/gapbs/benchmark/graphs/webU.sg", "-n3", ">", "benchmarks/gapbs/benchmark/out/tc-web.out",NULL},
    // 40 -> tc road
    {"benchmarks/gapbs/tc", "-f", "benchmarks/gapbs/benchmark/graphs/roadU.sg", "-n3", ">", "benchmarks/gapbs/benchmark/out/tc-road.out",NULL},

    // 41 -> sssp kron
    {"benchmarks/gapbs/sssp", "-f", "benchmarks/gapbs/benchmark/graphs/kron.wsg", "-n64", "-d2", ">", "benchmarks/gapbs/benchmark/out/sssp-kron.out",NULL},
    // 42 -> sssp urand
    {"benchmarks/gapbs/sssp", "-f", "benchmarks/gapbs/benchmark/graphs/urand.wsg", "-n64", "-d2", ">", "benchmarks/gapbs/benchmark/out/sssp-urand.out",NULL},
    // 43 -> sssp twitter
    {"benchmarks/gapbs/sssp", "-f", "benchmarks/gapbs/benchmark/graphs/twitter.wsg", "-n64", "-d2", ">", "benchmarks/gapbs/benchmark/out/sssp-twitter.out",NULL},
    // 44 -> sssp web
    {"benchmarks/gapbs/sssp", "-f", "benchmarks/gapbs/benchmark/graphs/web.wsg", "-n64", "-d2", ">", "benchmarks/gapbs/benchmark/out/sssp-web.out",NULL},
    // 45 -> sssp road
    {"benchmarks/gapbs/sssp", "-f", "benchmarks/gapbs/benchmark/graphs/road.wsg", "-n64", "-d50000", ">", "benchmarks/gapbs/benchmark/out/sssp-road.out",NULL},
};  

/*************************************************************
 **                   Name of the benchmarks	            **
 *************************************************************/

char *bench_Names [NUM_BENCHMARKS] = {
	"bfs","bfs","bc","bc","cc","cc","pr","pr",//0--7
    "sssp","sssp","tc","tc","cc_sv","cc_sv","pr_spmv","pr_spmv",//8--15
    "bfs","pr","cc","bc","bfs","pr","cc","bc",//16--23
    "bfs","pr","cc","bc","bfs","pr","cc","bc",//24--31
    "bfs","pr","cc","bc","tc","tc","tc","tc","tc",//32--40
    "sssp","sssp","sssp","sssp","sssp"//41--45
};

char *bench_Names_complete [NUM_BENCHMARKS] = {
	"bfs_g_24","bfs_u_24","bc_g_24","bc_u_24","cc_g_24","cc_u_24","pr_g_24","pr_u_24",//0--7
    "sssp_g_24","sssp_u_24","tc_g_24","tc_u_24","cc_sv_g_24","cc_sv_u_24","pr_spmv_g_24","pr_spmv_u_24",//8--15
    "bfs-twitter","pr-twitter","cc-twitter","bc-twitter","bfs-web","pr-web","cc-web","bc-web",//16--23
    "bfs-road","pr-road","cc-road","bc-road","bfs-kron","pr-kron","cc-kron","bc-kron",//24--31
    "bfs-urand","pr-urand","cc-urand","bc-urand","tc-kron","tc-urand","tc-twitter","tc-web","tc-road",//32--39
    "sssp-kron","sssp-urand","sssp-twitter","sssp-web","sssp-road"//41--45
};

/*************************************************************
 **           Instrucciones for the benchmarks	            **
 *************************************************************/

unsigned long int bench_Instructions [NUM_BENCHMARKS] = {
        223030519099,92129370138,218961549501,81274477281,222406002284,89132503770,218846766374,82829436069,//0--7
        223103812333,107057190320,221974004596,82038751242,222440048395,90553369490,218857412520,80821985448,//8--15
        160296512643,95183161779,185712564811,131916126004,475082751896,384980974792,419129969118,372051027770,//16--23
        174683365095,243602631150,190433168136,110271444711,155150018076,89009462507,127457991159,144166826650,//24--31
        141167241918,77428841125,118405803496,96800997392,152613202132,101431611260,128032150759,770322654090,//32--39
        146221601301,207064728411,152864043360,163146916272,205447073773,149498879036//41--45
};

/*************************************************************
 **                 search_benchmark                        **
 *************************************************************/

int search_benchmark(int benchmark,FILE **in_file) {
    int res = 0;
    close(1);
    *in_file = fopen("rundir/sal.log", "w");
    close(2);
    *in_file = fopen("rundir/err.log", "w");
    return res;
}

#endif