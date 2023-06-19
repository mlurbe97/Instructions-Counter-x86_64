/*
## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022
*/

#ifndef Intel_Core  /* Include guard */
#define Intel_Core

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

#define NUM_CPUS 32 // Number of processor cores counting SMT logical cores.
#define CPU_FREQ_BIG 3000 // Big-cores (P-cores) base frequency.
#define CPU_FREQ_SMALL 2200 // Small-cores (E-cores) base frequency.
#define CPU_FREQ 3000 // All cores.
#define NUM_PMU_COUNTERS 12
#define P_CORES 0
#define E_CORES 1
#define NUM_BIG_CORES 8
#define NUM_BIG_CORES_SMT2 16
#define NUM_SMALL_CORES 16

/*************************************************************
 **                 cpu_node_struct                         **
 *************************************************************/

typedef struct {//Defines the variables of an application or process. Represents a core of the processor.
	pid_t pid;//Process id
	int benchmark;//Application to run
	int finished;//Have you completed the target instructions?
	int status;
	int thread_id;	// Core where it runs
	uint64_t counters[NUM_PMU_COUNTERS];
    uint64_t acumulated_counters[NUM_PMU_COUNTERS];
	cpu_set_t mask;
	char *core_out;
    int P_or_E_core;
    // Processor thread childrens.
    pid_t *children;
    int num_children;
    int num_quantums;
	/*
		To save the information of each application in each quantum.
	*/
	uint64_t total_instructions;//Total instructions executed by the application.
	uint64_t total_cycles;//Total cycles executed by the application.
	uint64_t instructions;//Instructions executed in the last quantum.
	uint64_t cycles;//Cycles executed in the last quantum.
	int actual_MSR;//Current msr configuration.
	float actual_IPC;//IPC of the measurement interval.
	float actual_BW;

	//For the previous intervals we keep the CPI reached.
	float last_IPC_1;
	float last_IPC_2;
	float last_IPC_3;

	//For the previous intervals we save the consumed BW
	float last_BW_1;
	float last_BW_2;
	float last_BW_3;
} cpu_core;

cpu_core cpus_list[NUM_CPUS];//Queue where all running applications are stored. They are the cores of the processor.

/*************************************************************
 **         Intel Big and Small Cores                   **
 *************************************************************/
/*
Cache topology
L3 Shared among all the cores (P-cores and E-cores) 36MB
L2 Private for each P-Core 2048KB and Shared each 4 E-cores 4096KB
L1i Private P-cores 32KB and E-cores 64KB
L1d Private P-cores 48KB and E-cores 32KB
*/
int bigCoresThreads1[] = {0,2,4,6,8,10,12,14};
int bigCoresThreads2[] = {1,3,5,7,9,11,13,15};
int bigCoresSMT2[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int smallCores[] = {16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

/*************************************************************
 **         Intel Prefetch Configurations                   **
 *************************************************************/

// Intel Prefetch register MSR prefetch configurations.
const int msr_prefetch_reg = 0x1a4;
// 0 All prefetchers enabled.
// 1 L2 Hardware prefetcher.
// 2 L2 adjacent cache line prefetcher.
// 3 DCU prefetcher.
// 4 DCU IP prefetcher.
// 5 All prefetchers disabled.
const int msr_stats [] = {0x0,0xE,0xD,0xB,0x7,0xF};

// Length of regvals.
const int prefetch_configs [] = {0,1,2,3,4,5};
const int num_prefetch_configs = 6;

/*************************************************************
 **                 wrmsr_on_cpu                            **
 *************************************************************/

void wrmsr_on_cpu(int msr_state, int cpu, int verbose) {
    uint64_t data;
    int fd;
    char msr_file_name[64];
    if(verbose) {
        fprintf(stdout,"INFO: Configuring the MSR=%d, MSR_value=%d on CPU=%d, Register=%d\n",msr_state,msr_stats[msr_state],cpu,msr_prefetch_reg);
    }
    sprintf(msr_file_name, "/dev/cpu/%d/msr", cpu);
    fd = open(msr_file_name, O_WRONLY);
    if (fd < 0) {
        if (errno == ENXIO) {
            fprintf(stderr,"ERROR: No CPU %d found!\n", cpu);
            exit(2);
        } else if (errno == EIO) {
            fprintf(stderr,"ERROR: CPU %d doesn't support MSRs\n", cpu);
            exit(3);
        } else {
            fprintf(stderr,"ERROR: wrmsr open");
            exit(127);
        }
    }
	data = msr_stats[msr_state];
    if (pwrite(fd, &data, sizeof(data), msr_prefetch_reg) != sizeof(data)) {
        if (errno == EIO) {
            fprintf(stderr,"ERROR: CPU %d cannot set MSR ""0x%08"PRIx32" to 0x%016"PRIx64"\n", cpu, msr_prefetch_reg, data);
            exit(4);
        } else {
            fprintf(stderr,"ERROR: pwrite");
            exit(127);
        }
    }
    close(fd);
    if(verbose) {
        fprintf(stdout,"INFO: Successfully configured the MSR=%d, MSR_value=%d on CPU=%d, Register=%d\n",msr_state,msr_stats[msr_state],cpu,msr_prefetch_reg);
    }
}

#endif