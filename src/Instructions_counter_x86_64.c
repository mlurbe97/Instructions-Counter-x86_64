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

//Select only one include for benchmarks to compile with
//#include "spec_benchmarks.h"
//#include "geekbench_benchmarks.h"
#include "gap_benchmarks.h"

// Intel Hybrid
#include "Intel_Core.h"

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
  fprintf(stdout,"usage: [PROGRAM] [-h] [-P] [-g] [-k core_type] [-t time (in seconds)] [-d delay (in milliseconds)] [-e cycles,instructions,event2,...] A prgA\n");
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
  FILE *in_file;
  int max_quantums = -1;
  int nucli = -1;

  prg = -1;
  options.verbose = 0;
  options.delay = 0;

  while ((c=getopt(argc, argv,"he:d:t:pvgPA:Q:k:")) != -1) {
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
    case 'k':
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

  switch (nucli)
  {
  case 0://P-core
    CPU_ZERO(&mask);
    CPU_SET(bigCoresSMT2[0], &mask);
    break;
  case 1://E-core
    CPU_ZERO(&mask);
    CPU_SET(smallCores[0], &mask);
    break;
  default://P-core
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    break;
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
        if(search_benchmark(prg,&in_file) == -1)
        {
          return -1;
        }
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
