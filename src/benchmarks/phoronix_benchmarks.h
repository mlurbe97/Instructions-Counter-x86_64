/*
## Manager-IPMAN1.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2023
*/

#ifndef spec_benchmarks  /* Include guard */
#define spec_benchmarks

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

#define NUM_BENCHMARKS 51 // Number of available benchmarks. 0-50

/*************************************************************
 **                   Benchmarks SPEC2006-2017              **
 *************************************************************/

char *benchmarks[NUM_BENCHMARKS][200] = {
  //* SPEC CPU 2006 *//
  // 0 -> perlbench checkspam
  {"benchmarks/spec2006-x86-bin/400.perlbench/perlbench_base.i386", "", NULL},
  };

/*************************************************************
 **                   Name of the benchmarks	            **
 *************************************************************/

char *bench_Names [NUM_BENCHMARKS] = {
	"perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum",//0--7
	"h264ref","omnetpp","astar","xalancbmk","bwaves","gamess","milc","zeusmp",//8--15
	"gromacs","cactusADM","leslie3d","namd","soplex","povray","GemsFDTD",//16--22
	"lbm","perlbench diffmail","calculix","perlbench_r checkspam","gcc_r",//23--27
  "mcf_r","omnetpp_r","xalancbmk_r","x264_r","deepsjeng_r","leela_r",//28--33
  "exchange2_r","xz_r 1","bwaves_r","cactuBSSN_r","lbm_r","wrf_r","imagick_r",//34--40
  "nab_r","fotonik3d_r","roms_r","namd_r","parest_r","povray_r","xz_r 2",//41--47
  "xz_r 3","exchange2_r","perlbench_r diffmail"//48--50
};

char *bench_Names_complete [NUM_BENCHMARKS] = {
	"perlbench checkspam","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum",//0--7
	"h264ref","omnetpp","astar","xalancbmk","bwaves","gamess","milc","zeusmp",//8--15
	"gromacs","cactusADM","leslie3d","namd","soplex","povray","GemsFDTD",//16--22
	"lbm","perlbench diffmail","calculix","perlbench_r checkspam","gcc_r",//23--27
  "mcf_r","omnetpp_r","xalancbmk_r","x264_r","deepsjeng_r","leela_r",//28--33
  "exchange2_r","xz_r 1","bwaves_r","cactuBSSN_r","lbm_r","wrf_r","imagick_r",//34--40
  "nab_r","fotonik3d_r","roms_r","namd_r","parest_r","povray_r","xz_r 2",//41--47
  "xz_r 3","exchange2_r","perlbench_r diffmail"//48--50
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