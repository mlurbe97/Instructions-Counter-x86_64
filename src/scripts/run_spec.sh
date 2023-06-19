#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./run.sh -k core_type [0->P_core 1->E_core] -t time [Number in seconds] -w workloadArray [a->SPEC 2006 b->SPEC 2017 c->SPEC-ALL d->custom_workload] -c num_cores -f cpu_freq [default=2.50 GHz] s-> selected_workload [0-50]"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "k:t:w:c:f:s:" opt
do
   case "$opt" in
      k ) CORE_TYPE="$OPTARG" ;;
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

if [ -z "$CORE_TYPE" ]
then
    CORE_TYPE="0"
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
   NUM_CORES=$(nproc)
   echo "WARN: Using default NUM_CORES=" $NUM_CORES;
fi

# Select the array of benckmarks to be executed
case "$ARRAY" in 

    #case 1 
    "a") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25" ;; ## SPEC 2006.
    
    #case 2 
    "c") workloadArray="26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50" ;; ## SPEC 2017.
    
    #case 3
    "d") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50" ;; ## SPEC 2006 & 2017.

    #case 4
    "f") workloadArray=$CUSTOM_APP ;;

    #case 5
    *) echo "ERROR: Invalid entry for array of benckmarks."
      helpFunction;; 
esac

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/speccpu/

if mkdir $OUTDIR ; then
   echo "INFO: Directory for speccpu created."
else
   echo "WARN: Failed to create directory for speccpu. It may already exist."
fi

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/speccpu/$CORE_TYPE/

if mkdir $OUTDIR ; then
   echo "INFO: Directory for core type created."
else
   echo "WARN: Failed to create directory for core type. It may already exist."
fi

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/speccpu/instructions[${TIME}]

if mkdir $OUTDIR ; then
   echo "INFO: Directory for new results created."
else
   echo "WARN: Failed to create directory for results. It may already exist."
fi 

# Begin script in case all parameters are correct
{ sudo ./start_experiments.sh -c $NUM_CORES -f $CPU_FREQ; }

export SPECPERLLIB=/home/malursem/spec2017/bin/lib:/home/malursem/spec2017/bin

for workload in $workloadArray
do
	sudo ./Instructions_counter_x86_64 -k $CORE_TYPE -t $TIME -A $workload 2>> $OUTDIR/Instructions[${workload}].txt
done;

{ sudo ./end_experiments.sh -c $NUM_CORES; }