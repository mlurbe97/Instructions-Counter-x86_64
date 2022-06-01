#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./run.sh -t time [Number in seconds] -w workloadArray [a->SPEC 2006 & microbenchmark b->Geekbench5 c->SPEC 2017 d->SPEC-ALL & microbenchmark e->All f->custom_workload] -c num_cores -f cpu_freq [default=2.50 GHz] s-> selected_workload [0-26:30-55]"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "t:w:c:f:s:" opt
do
   case "$opt" in
      t ) TIME="$OPTARG" ;;
      w ) ARRAY="$OPTARG" ;;
      c ) NUM_CORES="$OPTARG" ;;
      f ) CPU_FREQ="$OPTARG" ;;
      s ) CUSTOM_APP="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

# Print helpFunction in case parameters are empty
if [ -z "$TIME" ] || [ -z "$ARRAY" ]
then
   echo "ERROR: Missing arguments.";
   helpFunction
fi

if [ -z "$CPU_FREQ" ]
then
    CPU_FREQ="2.50 GHz"
    echo "WARN: Using default CPU_FREQ=" $CPU_FREQ;
fi

if [ -z "$CUSTOM_APP" ]
then
    CUSTOM_APP="20"
fi

if [ -z "$NUM_CORES" ]
then
    NUM_CORES=8
    echo "WARN: Using default NUM_CORES=" $NUM_CORES;
fi

# Select the array of benckmarks to be executed
case "$ARRAY" in 

    #case 1 
    "a") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26" ;;## SPEC 2006 & microbenchmark.

    #case 2 
    "b") workloadArray="27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47" ;;# Geekbench5.
    
    #case 3 
    "c") workloadArray="48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72" ;;# SPEC 2017.
    
    #case 4
    "d") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72" ;;# SPEC 2006 & 2017 & microbenchmark.
    
    #case 5
    "e") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72" ;;# All.
    
    #case 6
    "f") workloadArray=$CUSTOM_APP ;;

    #case 7
    *) echo "ERROR: Invalid entry for array of benckmarks."
      helpFunction;; 
esac

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/instructions[${TIME}]

# if rm -rf $OUTDIR ; then
#    echo "INFO: Directory for old results deleted."
# else
#    echo "WARN: Failed to delete directory for results. It may no exist."
# fi 

if mkdir $OUTDIR ; then
   echo "INFO: Directory for new results created."
else
   echo "WARN: Failed to create directory for results. It may already exist."
fi 

# Begin script in case all parameters are correct
{ sudo ./start_experiments.sh -c $NUM_CORES -f $CPU_FREQ; }

for workload in $workloadArray
do
	sudo ./Instructions_counter_x86_64 -t $TIME -A $workload 2>> $OUTDIR/Instructions[${workload}].txt
done;

{ sudo ./end_experiments.sh -c $NUM_CORES; }