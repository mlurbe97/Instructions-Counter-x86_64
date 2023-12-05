/*
## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
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
  {"benchmarks/spec2006-x86-bin/400.perlbench/perlbench_base.i386", "-ICPU2006/400.perlbench/data/all/input/lib", "benchmarks/CPU2006/400.perlbench/data/ref/input/checkspam.pl", "2500", "5", "11", "150", "1", "1", "1", "1", ">", "benchmarks/CPU2006/400.perlbench/data/ref/output/checkspam.2500.5.25.11.150.1.1.1.1.out", NULL},
  // 1 -> bzip2
  {"benchmarks/spec2006-x86-bin/401.bzip2/bzip2_base.i386", "benchmarks/CPU2006/401.bzip2/data/all/input/input.combined", "200", NULL},
  // 2 -> gcc
  {"benchmarks/spec2006-x86-bin/403.gcc/gcc_base.i386", "benchmarks/CPU2006/403.gcc/data/ref/input/scilab.i", "-o", "scilab.s", NULL},
  // 3 -> mcf
  {"benchmarks/spec2006-x86-bin/429.mcf/mcf_base.i386", "benchmarks/CPU2006/429.mcf/data/ref/input/inp.in", NULL},
  // 4 -> gobmk
  {"benchmarks/spec2006-x86-bin/445.gobmk/gobmk_base.i386", "--quiet", "--mode", "gtp", NULL},
  // 5 -> hmmer
  {"benchmarks/spec2006-x86-bin/456.hmmer/hmmer_base.i386", "--fixed", "0", "--mean", "500", "--num", "500000", "--sd", "350", "--seed", "0", "benchmarks/CPU2006/456.hmmer/data/ref/input/retro.hmm", NULL},
  // 6 -> sjeng
  {"benchmarks/spec2006-x86-bin/458.sjeng/sjeng_base.i386", "benchmarks/CPU2006/458.sjeng/data/ref/input/ref.txt", NULL},
  // 7 -> libquantum
  {"benchmarks/spec2006-x86-bin/462.libquantum/libquantum_base.i386", "1397", "8", NULL},
  // 8 -> h264ref
  {"benchmarks/spec2006-x86-bin/464.h264ref/h264ref_base.i386", "-d", "benchmarks/CPU2006/464.h264ref/data/ref/input/foreman_ref_encoder_baseline.cfg", NULL},
  // 9 -> omnetpp
  {"benchmarks/spec2006-x86-bin/471.omnetpp/omnetpp_base.i386", "benchmarks/CPU2006/471.omnetpp/data/ref/input/omnetpp.ini", NULL},
  // 10 -> astar
  {"benchmarks/spec2006-x86-bin/473.astar/astar_base.i386", "benchmarks/CPU2006/473.astar/data/ref/input/BigLakes2048.cfg", NULL},
  // 11 -> xalancbmk
  {"benchmarks/spec2006-x86-bin/483.xalancbmk/xalancbmk_base.i386", "-v", "benchmarks/CPU2006/483.xalancbmk/data/ref/input/t5.xml", "benchmarks/CPU2006/483.xalancbmk/data/ref/input/xalanc.xsl", NULL},
  // 12 -> bwaves
  {"benchmarks/spec2006-x86-bin/410.bwaves/bwaves_base.i386", NULL},
  // 13 -> gamess
  {"benchmarks/spec2006-x86-bin/416.gamess/gamess_base.i386", NULL},
  // 14 -> milc
  {"benchmarks/spec2006-x86-bin/433.milc/milc_base.i386", NULL},
  // 15 -> zeusmp
  {"benchmarks/spec2006-x86-bin/434.zeusmp/zeusmp_base.i386", NULL},
  // 16 -> gromacs
  {"benchmarks/spec2006-x86-bin/435.gromacs/gromacs_base.i386", "-silent", "-deffnm", "benchmarks/CPU2006/435.gromacs/data/ref/input/gromacs", "-nice", "0", NULL},
  // 17 -> cactusADM
  {"benchmarks/spec2006-x86-bin/436.cactusADM/cactusADM_base.i386", "benchmarks/CPU2006/436.cactusADM/data/ref/input/benchADM.par", NULL},
  // 18 -> leslie3d
  {"benchmarks/spec2006-x86-bin/437.leslie3d/leslie3d_base.i386", NULL},
  // 19 -> namd
  {"benchmarks/spec2006-x86-bin/444.namd/namd_base.i386", "--input", "benchmarks/CPU2006/444.namd/data/all/input/namd.input", "--iterations", "38", "--output", "namd.out", NULL},
  // 20 -> soplex
  {"benchmarks/spec2006-x86-bin/450.soplex/soplex_base.i386", "-s1", "-e","-m45000", "benchmarks/CPU2006/450.soplex/data/ref/input/pds-50.mps", NULL},
  // 21 -> povray
  {"benchmarks/spec2006-x86-bin/453.povray/povray_base.i386", "benchmarks/CPU2006/453.povray/data/ref/input/SPEC-benchmark-ref.ini", NULL},
  // 22 -> GemsFDTD
  {"benchmarks/spec2006-x86-bin/459.GemsFDTD/GemsFDTD_base.i386", NULL},
  // 23 -> lbm
  {"benchmarks/spec2006-x86-bin/470.lbm/lbm_base.i386", "300", "reference.dat", "0", "1", "benchmarks/CPU2006/470.lbm/data/ref/input/100_100_130_ldc.of", NULL},
  // 24 -> perlbench diffmail
  {"benchmarks/spec2006-x86-bin/400.perlbench/perlbench_base.i386", "-ICPU2006/400.perlbench/data/all/input/lib", "benchmarks/CPU2006/400.perlbench/data/all/input/diffmail.pl", "4", "800", "10", "17", "19", "300", ">", "benchmarks/CPU2006/400.perlbench/data/ref/output/diffmail.4.800.10.17.19.300.out", NULL},
  // 25 -> calculix
  {"benchmarks/spec2006-x86-bin/454.calculix/calculix_base.i386", "-i", "benchmarks/CPU2006/454.calculix/data/ref/input/hyperviscoplastic", NULL},
  //* SPEC CPU 2017 *//
	// 26 -> perlbench_r checkspam
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/500.perlbench_r/exe/perlbench_r_base.InitialTest-m64", "-Ispec2017-x86-bin/bin/lib", "benchmarks/spec2017-x86-bin/benchspec/CPU/500.perlbench_r/data/all/input/checkspam.pl", "2500", "5", "25", "11", "150", "1", "1", "1", "1", NULL},
	// 27 -> gcc_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/502.gcc_r/exe/cpugcc_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/502.gcc_r/data/refrate/input/gcc-smaller.c", "-O3", "-fipa-pta", "-o", "gcc-smaller.opts-O3_-fipa-pta.s", NULL},
	// 28 -> mcf_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/505.mcf_r/exe/mcf_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/605.mcf_s/data/refrate/input/inp.in", NULL},
	// 29 -> omnetpp_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/520.omnetpp_r/exe/omnetpp_r_base.InitialTest-m64", "-c", "General", "-r", "0", NULL},
	// 30 -> xalancbmk_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/523.xalancbmk_r/exe/cpuxalan_r_base.InitialTest-m64", "-v", "benchmarks/spec2017-x86-bin/benchspec/CPU/623.xalancbmk_s/data/refrate/input/t5.xml", "benchmarks/spec2017-x86-bin/benchspec/CPU/623.xalancbmk_s/data/refrate/input/xalanc.xsl", NULL},
	// 31 -> x264_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/525.x264_r/exe/x264_r_base.InitialTest-m64", "--pass", "1", "--stats", "x264_stats.log", "--bitrate", "1000", "--frames", "1000", "-o", "benchmarks/spec2017-x86-bin/benchspec/CPU/625.x264_s/BuckBunny_New.264", "benchmarks/spec2017-x86-bin/benchspec/CPU/625.x264_s/BuckBunny.yuv", "1280x720", NULL},
	// 32 -> deepsjeng_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/538.imagick_r/exe/imagick_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/531.deepsjeng_r/ref.txt", NULL},
	// 33 -> leela_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/541.leela_r/exe/leela_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/641.leela_s/ref.sgf", NULL},
	// 34 -> exchange2_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/548.exchange2_r/exe/exchange2_r_base.InitialTest-m64", "6", NULL},
	// 35 -> xz_r 1
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/exe/xz_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/cld.tar.xz", "160", "19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474", "59796407", "61004416", "6", NULL},
	// 36 -> bwaves_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/503.bwaves_r/exe/bwaves_r_base.InitialTest-m64", NULL},
	// 37 -> cactuBSSN_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/507.cactuBSSN_r/exe/cactusBSSN_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/507.cactuBSSN_r/spec_ref.par", NULL},
	// 38 -> lbm_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/519.lbm_r/exe/lbm_r_base.InitialTest-m64", "3000", "reference.dat", "0", "0", "benchmarks/spec2017-x86-bin/benchspec/CPU/519.lbm_r/100_100_130_ldc.of", NULL},
	// 39 -> wrf_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/521.wrf_r/exe/wrf_r_base.InitialTest-m64", NULL},
	// 40 -> imagick_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/538.imagick_r/exe/imagick_r_base.InitialTest-m64", "-limit", "disk", "0", "benchmarks/spec2017-x86-bin/benchspec/CPU/538.imagick_r/refrate_input.tga", "-edge", "41", "-resample", "181%", "-emboss", "31", "-colorspace", "YUV", "-mean-shift", "19x19+15%", "-resize", "30%", "refrate_output.tga", NULL},
	// 41 -> nab_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/544.nab_r/exe/nab_r_base.InitialTest-m64", "3j1n", "20140317", "220", NULL},
	// 42 -> fotonik3d_r
	{"benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/549.fotonik3d_r/exe/fotonik3d_r_base.InitialTest-m64", NULL},
	// 43 -> roms_r
	{"benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/554.roms_r/exe/roms_r_base.InitialTest-m64", NULL},
	// 44 -> namd_r
	{"benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/508.namd_r/exe/namd_r_base.InitialTest-m64", "--input", "benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/508.namd_r/apoa1.input", "--output", "apoa1.ref.output", "--iterations", "65", NULL},
	// 45 -> parest_r
	{"benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/510.parest_r/exe/parest_r_base.InitialTest-m64", "benchmarks/benchmarks/spec2017-x86-bin/benchspec/CPU/510.parest_r/ref.prm", NULL},
	// 46 -> povray_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/511.povray_r/exe/povray_r_base.InitialTest-m64", "/home/malursem//spec2017/benchspec/CPU/511.povray_r/SPEC-benchmark-ref.ini", NULL},
	// 47 -> xz_r 2
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/exe/xz_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/cpu2006docs.tar.xz", "250", "055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae", "23047774", "23513385", "6e", NULL },
	// 48 -> xz_r 3
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/exe/xz_r_base.InitialTest-m64", "benchmarks/spec2017-x86-bin/benchspec/CPU/557.xz_r/input.combined.xz", "250", "a841f68f38572a49d86226b7ff5baeb31bd19dc637a922a972b2e6d1257a890f6a544ecab967c313e370478c74f760eb229d4eef8a8d2836d233d3e9dd1430bf", "40401484", "41217675", "7", NULL },
	// 49 -> exchange2_r
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/548.exchange2_r/exe/exchange2_r_base.InitialTest-m64","6",NULL},
	// 50 -> perlbench_r diffmail
	{"benchmarks/spec2017-x86-bin/benchspec/CPU/500.perlbench_r/exe/perlbench_r_base.InitialTest-m64", "-Ispec2017-x86-bin/benchspec/CPU/500.perlbench_r/lib", "benchmarks/spec2017-x86-bin/benchspec/CPU/500.perlbench_r/diffmail.pl", "4", "800", "10", "17", "19", "300", NULL},
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
 **           Instrucciones for the benchmarks	            **
 *************************************************************/

unsigned long int bench_Instructions [NUM_BENCHMARKS] = {
        0,0,0,0

};

/*************************************************************
 **                 search_benchmark                        **
 *************************************************************/

int search_benchmark(int benchmark,FILE **in_file) {
    int res = 0;
    // Descriptors for those who have input for standard input
        switch(benchmark) {
          case 4: // [Doesn't work]
            close(0);
            *in_file = fopen("benchmarks/CPU2006/445.gobmk/data/ref/input/13x13.tst", "r");
            if (*in_file == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: benchmarks/CPU2006/445.gobmk/data/ref/input/13x13.tst.\n");
            res = -1;
            }
            break;

          case 9:
            system("cp omnetpp_2006.ini omnetpp.ini  >/dev/null 2>&1");
            break;

          case 13:
            close(0);
            *in_file = fopen("benchmarks/CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config", "r");
            if (*in_file == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: benchmarks/CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config.\n");
              res = -1;
            }
            break;

          case 14:
            close(0);
            *in_file = fopen("benchmarks/CPU2006/433.milc/data/ref/input/su3imp.in", "r");
            if (*in_file == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: benchmarks/CPU2006/433.milc/data/ref/input/su3imp.in.\n");
              res = -1;
            }
            break;

          case 18:
            close(0);
            *in_file = fopen("benchmarks/CPU2006/437.leslie3d/data/ref/input/leslie3d.in", "r");
            if (*in_file == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: benchmarks/CPU2006/437.leslie3d/data/ref/input/leslie3d.in.\n");
              res = -1;
            }
            break;

          case 21:
            close(2);
            *in_file = fopen("povray.sal", "w");
            if (*in_file == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: povray.sal\n");
              res = -1;
            }
            break;
          case 36:
            close(0);
            *in_file = fopen("benchmarks/spec2017-x86-bin/benchspec/CPU/503.bwaves_r/bwaves_1.in", "r");
            if(*in_file == NULL) {
            fprintf(stderr,"ERROR. The file could not be opened: benchmarks/spec2017-x86-bin/benchspec/CPU/503.bwaves_r/bwaves_1.in.\n");
            res = -1;
            }
            break;

          case 29:
            system("cp omnetpp_2017.ini omnetpp.ini  >/dev/null 2>&1");
            break;

          case 43:
            close(0);
            *in_file = fopen("benchmarks/spec2017-x86-bin/benchspec/CPU/554.roms_r/ocean_benchmark2.in.x", "r");
            if(*in_file == NULL) {
            fprintf(stderr,"ERROR. The file could not be opened: benchmarks/spec2017-x86-bin/benchspec/CPU/554.roms_r/ocean_benchmark2.in.x.\n");
            res = -1;
            }
            break;
        }
        close(1);
        *in_file = fopen("rundir/sal.log", "w");
        close(2);
        *in_file = fopen("rundir/err.log", "w");
        return res;
}

#endif