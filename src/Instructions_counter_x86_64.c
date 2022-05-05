/*
## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
*/

/*************************************************************
 **                  Includes                               **
 *************************************************************/

#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/poll.h>
#include <sched.h>
#include "perf_util.h"

/*************************************************************
 **                   Structs                               **
 *************************************************************/
typedef struct {
  char *events;
  int delay;
  int pinned;
  int group;
  int verbose;
} options_t;

/*************************************************************
 **                   Global Variables                      **
 *************************************************************/

static options_t options;
uint64_t pmu_counters[7];
char *events[7];

/*************************************************************
 **                   Benchmarks SPEC2006-2017              **
 *************************************************************/

char *benchmarks[][200] = {
  //* SPEC CPU 2006 *//
  // 0 -> perlbench checkspam
  {"spec2006-x86-bin/400.perlbench/perlbench_base.i386", "-ICPU2006/400.perlbench/data/all/input/lib", "CPU2006/400.perlbench/data/ref/input/checkspam.pl", "2500", "5", "11", "150", "1", "1", "1", "1", ">", "CPU2006/400.perlbench/data/ref/output/checkspam.2500.5.25.11.150.1.1.1.1.out", NULL},
  // 1 -> bzip2
  {"spec2006-x86-bin/401.bzip2/bzip2_base.i386", "CPU2006/401.bzip2/data/all/input/input.combined", "200", NULL},
  // 2 -> gcc
  {"spec2006-x86-bin/403.gcc/gcc_base.i386", "CPU2006/403.gcc/data/ref/input/scilab.i", "-o", "scilab.s", NULL},
  // 3 -> mcf
  {"spec2006-x86-bin/429.mcf/mcf_base.i386", "CPU2006/429.mcf/data/ref/input/inp.in", NULL},
  // 4 -> gobmk
  {"spec2006-x86-bin/445.gobmk/gobmk_base.i386", "--quiet", "--mode", "gtp", NULL},
  // 5 -> hmmer
  {"spec2006-x86-bin/456.hmmer/hmmer_base.i386", "--fixed", "0", "--mean", "500", "--num", "500000", "--sd", "350", "--seed", "0", "CPU2006/456.hmmer/data/ref/input/retro.hmm", NULL},
  // 6 -> sjeng
  {"spec2006-x86-bin/458.sjeng/sjeng_base.i386", "CPU2006/458.sjeng/data/ref/input/ref.txt", NULL},
  // 7 -> libquantum
  {"spec2006-x86-bin/462.libquantum/libquantum_base.i386", "1397", "8", NULL},
  // 8 -> h264ref
  {"spec2006-x86-bin/464.h264ref/h264ref_base.i386", "-d", "CPU2006/464.h264ref/data/ref/input/foreman_ref_encoder_baseline.cfg", NULL},
  // 9 -> omnetpp
  {"spec2006-x86-bin/471.omnetpp/omnetpp_base.i386", "CPU2006/471.omnetpp/data/ref/input/omnetpp.ini", NULL},
  // 10 -> astar
  {"spec2006-x86-bin/473.astar/astar_base.i386", "CPU2006/473.astar/data/ref/input/BigLakes2048.cfg", NULL},
  // 11 -> xalancbmk
  {"spec2006-x86-bin/483.xalancbmk/xalancbmk_base.i386", "-v", "CPU2006/483.xalancbmk/data/ref/input/t5.xml", "CPU2006/483.xalancbmk/data/ref/input/xalanc.xsl", NULL},
  // 12 -> bwaves
  {"spec2006-x86-bin/410.bwaves/bwaves_base.i386", NULL},
  // 13 -> gamess
  {"spec2006-x86-bin/416.gamess/gamess_base.i386", NULL},
  // 14 -> milc
  {"spec2006-x86-bin/433.milc/milc_base.i386", NULL},
  // 15 -> zeusmp
  {"spec2006-x86-bin/434.zeusmp/zeusmp_base.i386", NULL},
  // 16 -> gromacs
  {"spec2006-x86-bin/435.gromacs/gromacs_base.i386", "-silent", "-deffnm", "CPU2006/435.gromacs/data/ref/input/gromacs", "-nice", "0", NULL},
  // 17 -> cactusADM
  {"spec2006-x86-bin/436.cactusADM/cactusADM_base.i386", "CPU2006/436.cactusADM/data/ref/input/benchADM.par", NULL},
  // 18 -> leslie3d
  {"spec2006-x86-bin/437.leslie3d/leslie3d_base.i386", NULL},
  // 19 -> namd
  {"spec2006-x86-bin/444.namd/namd_base.i386", "--input", "CPU2006/444.namd/data/all/input/namd.input", "--iterations", "38", "--output", "namd.out", NULL},
  // 20 -> microbench
  {"microbench", "100", "0", "1024", "0"},
  // 21 -> soplex
  {"spec2006-x86-bin/450.soplex/soplex_base.i386", "-s1", "-e","-m45000", "CPU2006/450.soplex/data/ref/input/pds-50.mps", NULL},
  //22 -> povray
  {"spec2006-x86-bin/453.povray/povray_base.i386", "CPU2006/453.povray/data/ref/input/SPEC-benchmark-ref.ini", NULL},
  // 23 -> GemsFDTD
  {"spec2006-x86-bin/459.GemsFDTD/GemsFDTD_base.i386", NULL},
  // 24 -> lbm
  {"spec2006-x86-bin/470.lbm/lbm_base.i386", "300", "reference.dat", "0", "1", "CPU2006/470.lbm/data/ref/input/100_100_130_ldc.of", NULL},
  // 25 -> perlbench diffmail
  {"spec2006-x86-bin/400.perlbench/perlbench_base.i386", "-ICPU2006/400.perlbench/data/all/input/lib", "CPU2006/400.perlbench/data/all/input/diffmail.pl", "4", "800", "10", "17", "19", "300", ">", "CPU2006/400.perlbench/data/ref/output/diffmail.4.800.10.17.19.300.out", NULL},
  // 26 -> calculix
  {"spec2006-x86-bin/454.calculix/calculix_base.i386", "-i", "CPU2006/454.calculix/data/ref/input/hyperviscoplastic", NULL},
  //* Geekbench 5 *//
  // 27 -> geekbench5 AES-XTS
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "101", "--no-upload", NULL},
  // 28 -> geekbench5 Text Compression
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "201", "--no-upload", NULL},
  // 29 -> geekbench5 Image Compression
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "202", "--no-upload", NULL},
  // 30 -> geekbench5 Navigation
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "203", "--no-upload", NULL},
  // 31 -> geekbench5 HTML5
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "204", "--no-upload", NULL},
  // 32 -> geekbench5 SQLite
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "205", "--no-upload", NULL},
  // 33 -> geekbench5 PDF Rendering
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "206", "--no-upload", NULL},
  // 34 -> geekbench5 Text Rendering
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "207", "--no-upload", NULL},
  // 35 -> geekbench5 Clang
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "208", "--no-upload", NULL},
  // 36 -> geekbench5 Camera
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "209", "--no-upload", NULL},
  // 37 -> geekbench5 N-Body Physics
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "301", "--no-upload", NULL},
  // 38 -> geekbench5 Rigid Body Physics
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "302", "--no-upload", NULL},
  // 39 -> geekbench5 Gaussian Blur
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "303", "--no-upload", NULL},
  // 40 -> geekbench5 Face Detection
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "305", "--no-upload", NULL},
  // 41 -> geekbench5 Horizon Detection
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "306", "--no-upload", NULL},
  // 42 -> geekbench5 Image Inpainting
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "307", "--no-upload", NULL},
  // 43 -> geekbench5 HDR
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "308", "--no-upload", NULL},
  // 44 -> geekbench5 Ray Tracing
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "309", "--no-upload", NULL},
  // 45 -> geekbench5 Structure from Motion
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "310", "--no-upload", NULL},
  // 46 -> geekbench5 Speech Recognition
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "312", "--no-upload", NULL},
  // 47 -> geekbench5 Machine Learning
  {"Geekbench-5.4.4-Linux/geekbench5", "--arch", "64bit", "--section", "1", "--workload", "313", "--no-upload", NULL},
};

/*************************************************************
 **                  get_counts                             **
 *************************************************************/

static void get_counts(perf_event_desc_t *fds, int num) {
  ssize_t ret;
  int i;

  for(i=0; i < num; i++) {
    uint64_t val;
    ret = read(fds[i].fd, fds[i].values, sizeof(fds[i].values));
    if (ret < (ssize_t)sizeof(fds[i].values)) {
      if (ret == -1){
        fprintf(stderr, "ERROR: cannot read values event %s\n", fds[i].name);
      } else{
        fprintf(stderr,"ERROR: could not read event %d\n", i);
      }
    }
    val = fds[i].values[0];
    pmu_counters[i] += val;
  }
}

/*************************************************************
 **                 measure                                 **
 *************************************************************/

int measure(pid_t *pid) {
  perf_event_desc_t *fds = NULL;
  int i, j, ret, num_fds = 0;
  int status;

  ret = perf_setup_list_events(options.events, &fds, &num_fds);
  if (ret || (num_fds == 0)) {
    exit(1);
  }
  fds[0].fd = -1;
  for(j=0; j < num_fds; j++) {
    fds[j].hw.disabled = 0; /* start immediately */
    /* request timing information necessary for scaling counts */
    fds[j].hw.read_format = PERF_FORMAT_SCALE;
    fds[j].hw.pinned = !j && options.pinned;
    fds[j].fd = perf_event_open(&fds[j].hw, *pid, -1, (options.group? fds[j].fd : -1), 0);
    if (fds[j].fd == -1) {
      fprintf(stderr, "ERROR: cannot attach event %s\n", fds[j].name);
      exit(1);
    }
  }
  // Release the processes.
  kill(*pid, 18);
  waitpid(*pid, &status, WCONTINUED);
  if (WIFEXITED(status)) {
     fprintf(stderr,"ERROR: command process %d exited too early with status %d\n", *pid, WEXITSTATUS(status));
  }
  usleep(options.delay*1000);
  // Block processes
  ret = 0;
  kill(*pid, 19);
  waitpid(*pid, &status, WUNTRACED);
  if (WIFEXITED(status)) {
    fprintf(stderr,"INFO: Process %d finished with status %d\n", *pid, WEXITSTATUS(status));
    ret = 1;
  }
  get_counts(fds, num_fds);
  for(i=0; i < num_fds; i++) {
    close(fds[i].fd);
  }
  perf_free_fds(fds, num_fds);
  return ret;
}

/*************************************************************
 **                      initialize_events                       **
 *************************************************************/

int initialize_events() {
  perf_event_desc_t *fds = NULL;
  int i, ret, num_fds = 0;

  ret = perf_setup_list_events(options.events, &fds, &num_fds);
  if (ret || (num_fds == 0)) {
    exit(1);
  }
  for (i=0; i < num_fds; i++) {
    events[i] = strdup(fds[i].name);
    pmu_counters[i] = 0;
  }
  for(i=0; i < num_fds; i++) {
    close(fds[i].fd);
  }
  perf_free_fds(fds, num_fds);
  return num_fds;
}

/*************************************************************
 **                          usage                          **
 *************************************************************/

static void usage(void) {
  fprintf(stdout,"usage: [PROGRAM] [-h] [-P] [-g] [-t time (in seconds)] [-d delay (in milliseconds)] [-e cycles,instructions,event2,...] A prgA\n");
}

/*************************************************************
 **                     MAIN PROGRAM                        **
 *************************************************************/

int main(int argc, char **argv) {

  printf("\nInstructions Counter x86_64.\nAuthor: Manel Lurbe Sempere\ne-mail: malursem@gap.upv.es\nYear: 2022\n");

  int c, ret, temps=-1, q=0, i;
  pid_t pid;
  int prg;
  cpu_set_t mask;
  int status, num_fds;
  FILE *fitxer;
  int max_quantums = -1;
  int nucli = -1;

  prg = -1;
  options.verbose = 0;
  options.delay = 0;

  while ((c=getopt(argc, argv,"he:d:t:pvgPA:Q:C:")) != -1) {
    switch(c) {
    case 'e':
      options.events = optarg;
      break;
    case 'P':
      options.pinned = 1;
      break;
    case 'g':
      options.group = 1;
      break;
    case 'd':
      options.delay = atoi(optarg);
      break;
    case 'h':
      usage();
      exit(0);
    case 'A':
      prg = atoi(optarg);
      break;
    case 't':
      temps = atoi(optarg);
      break;
    case 'v':
      options.verbose = 1;
      break;
    case 'Q':
      max_quantums = atoi(optarg);
      break;
    case 'C':
        nucli = atoi(optarg);
        break;
    default:
      fprintf(stderr, "ERROR: Unknown error\n");
    }
  }
  setenv("OMP_NUM_THREADS", "1", 1);

  if (!options.events) {
    options.events = strdup("cycles,instructions");
  }

  if (options.delay < 1) {
    options.delay = 100;
  }

  if (prg < 0) {
    fprintf(stderr, "ERROR: The process needs to be specified.\n");
    return -1;
  }

  if (max_quantums < 0) {
    if (temps < 0) {
      fprintf(stderr, "ERROR: Max time or quanta should be especified.\n");
      exit (-1);
    }
    fprintf(stderr, "INFO: Time: %d, Delay: %d\n", temps, options.delay);
    max_quantums = temps * 1000 / options.delay;
    fprintf(stderr, "INFO: Number of quanta set to %d.\n", max_quantums);
  } else {
    fprintf(stderr, "INFO: Number of quanta defined to %d.\n", max_quantums);
  }

  if (nucli == -1) {
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
  } else {
    CPU_ZERO(&mask);
    CPU_SET(nucli, &mask);
  }

  if (pfm_initialize() != PFM_SUCCESS) {
    fprintf(stderr,"ERROR: libpfm initialization failed\n");
    exit (-1);
  }
  // Preparation
  num_fds = initialize_events();
  fprintf(stderr,"INFO: Number of events configured num_fds=%d\n",num_fds);
  do {
    pid = fork();
    switch (pid) {
      case -1: // ERROR
        fprintf(stderr,"ERROR: Couldn't create the child.\n");
        return -1;

      case 0: // Child
        // Descriptors for those who have input for standard input
        switch(prg) {
          case 4: // [Doesn't work]
            close(0);
            fitxer = fopen("CPU2006/445.gobmk/data/ref/input/13x13.tst", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: CPU2006/445.gobmk/data/ref/input/13x13.tst.\n");
              return -1;
            }
            break;

          case 9:
            system("cp omnetpp_2006.ini omnetpp.ini  >/dev/null 2>&1");
            break;

          case 13:
            close(0);
            fitxer = fopen("CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config.\n");
              return -1;
            }
            break;

          case 14:
            close(0);
            fitxer = fopen("CPU2006/433.milc/data/ref/input/su3imp.in", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: CPU2006/433.milc/data/ref/input/su3imp.in.\n");
              return -1;
            }
            break;

          case 18:
            close(0);
            fitxer = fopen("CPU2006/437.leslie3d/data/ref/input/leslie3d.in", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: CPU2006/437.leslie3d/data/ref/input/leslie3d.in.\n");
              return -1;
            }
            break;

          case 22:
            close(2);
            fitxer = fopen("povray.sal", "w");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR: The file could not be opened: povray.sal\n");
              return -1;
            }
            break;
        }
        close(1);
        fitxer = fopen("rundir/sal.log", "w");
        close(2);
        fitxer = fopen("rundir/err.log", "w");
        execv(benchmarks[prg][0], benchmarks[prg]);
        fprintf(stderr,"ERROR: Couldn't launch the program %d.\n", prg);
        return -1;

      default:  // Parent                                                                               
        usleep (200000);  // Wait 200 ms
        // Assign the process to a core
        if (sched_setaffinity(pid, sizeof(mask), &mask) != 0) { // Instead of sizeof putting 1 was fine
          fprintf(stderr,"ERROR: Sched_setaffinity %d.\n", errno);
          exit(1);
        }
        // We stop the process
        kill (pid, 19);  
        waitpid(pid, &status, WUNTRACED);
        if (WIFEXITED(status)) {
          fprintf(stderr,"ERROR: command process %d exited too early with status %d\n", pid, WEXITSTATUS(status));
        }
    }
    for (; q<max_quantums; q++) {
      ret = measure(&pid);
      if (ret) {
        fprintf(stderr, "INFO: The process has completed all quantums.\n");
        break;
      }
    }
  } while (q < max_quantums-1);
  kill(pid, 9);
  // Print values for cycles and instructions executed before exiting the scheduler.
  fprintf(stderr, "PMU_COUNTS:");
  for (i=0; i<num_fds; i++) {
    fprintf(stderr, "\t%"PRIu64"", pmu_counters[i]);
  }
  fprintf(stderr, "\n");  
  // Free libpfm resources cleanly
  pfm_terminate();
  return 0;
}
