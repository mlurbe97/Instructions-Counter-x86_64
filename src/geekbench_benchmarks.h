/*
## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
*/

#ifndef geekbench_benchmarks  /* Include guard */
#define geekbench_benchmarks

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

#define NUM_BENCHMARKS 21 // Number of available benchmarks. 0-20

/*************************************************************
 **                   Benchmarks SPEC2006-2017              **
 *************************************************************/

char *benchmarks[NUM_BENCHMARKS][200] = {
  //* Geekbench 5 *//
  // 0 -> geekbench5 AES-XTS
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "101", "--no-upload", NULL},
  // 1 -> geekbench5 Text Compression
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "201", "--no-upload", NULL},
  // 2 -> geekbench5 Image Compression
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "202", "--no-upload", NULL},
  // 3 -> geekbench5 Navigation
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "203", "--no-upload", NULL},
  // 4 -> geekbench5 HTML5
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "204", "--no-upload", NULL},
  // 5 -> geekbench5 SQLite
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "205", "--no-upload", NULL},
  // 6 -> geekbench5 PDF Rendering
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "206", "--no-upload", NULL},
  // 7 -> geekbench5 Text Rendering
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "207", "--no-upload", NULL},
  // 8 -> geekbench5 Clang
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "208", "--no-upload", NULL},
  // 9 -> geekbench5 Camera
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "209", "--no-upload", NULL},
  // 10 -> geekbench5 N-Body Physics
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "301", "--no-upload", NULL},
  // 11 -> geekbench5 Rigid Body Physics
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "302", "--no-upload", NULL},
  // 12 -> geekbench5 Gaussian Blur
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "303", "--no-upload", NULL},
  // 13 -> geekbench5 Face Detection
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "305", "--no-upload", NULL},
  // 14 -> geekbench5 Horizon Detection
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "306", "--no-upload", NULL},
  // 15 -> geekbench5 Image Inpainting
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "307", "--no-upload", NULL},
  // 16 -> geekbench5 HDR
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "308", "--no-upload", NULL},
  // 17 -> geekbench5 Ray Tracing
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "309", "--no-upload", NULL},
  // 18 -> geekbench5 Structure from Motion
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "310", "--no-upload", NULL},
  // 19 -> geekbench5 Speech Recognition
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "312", "--no-upload", NULL},
  // 20 -> geekbench5 Machine Learning
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "313", "--no-upload", NULL},
 };

/*************************************************************
 **                   Name of the benchmarks	            **
 *************************************************************/

char *bench_Names [NUM_BENCHMARKS] = {
	"AES-XTS","Text Compression","Image Compression","Navigation","HTML5",//0--4
	"SQLite","PDF Rendering","Text Rendering","Clang","Camera","N-Body Physics","Rigid Body Physics","Gaussian Blur",//5--12
	"Face Detection","Horizon Detection","Image Inpainting","HDR","Ray Tracing","Structure from Motion","Speech Recognition","Machine Learning"//13--20
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