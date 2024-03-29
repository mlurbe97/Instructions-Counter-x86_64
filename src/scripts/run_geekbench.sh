#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./run.sh -k core_type [0->P_core 1->E_core] -t time [Number in seconds] -w workloadArray [a->all benches b->custom_workload] -f cpu_freq [default=3000000] s-> selected_workload [0-20]"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "k:t:w:f:s:" opt
do
   case "$opt" in
      k ) CORE_TYPE="$OPTARG" ;;
      t ) TIME="$OPTARG" ;;
      w ) ARRAY="$OPTARG" ;;
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
    echo "WARN: Using default CORE_TYPE=P_core."
fi

if [ -z "$CPU_FREQ" ]
then
    CPU_FREQ="3000000"
    echo "WARN: Using default CPU_FREQ=" $CPU_FREQ;
fi

if [ -z "$CUSTOM_APP" ]
then
    CUSTOM_APP="0"
fi

# Select the array of benckmarks to be executed
case "$ARRAY" in 

    #case 1 
    "a") workloadArray="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20" ;; ## Geekbench5.

    #case 2
    "b") workloadArray=$CUSTOM_APP ;;

    #case 3
    *) echo "ERROR: Invalid entry for array of benckmarks."
      helpFunction;; 
esac

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/geekbench5/

if mkdir $OUTDIR ; then
   echo "INFO: Directory for geekbench created."
else
   echo "WARN: Failed to create directory for geekbench. It may already exist."
fi

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/geekbench5/$CORE_TYPE/

if mkdir $OUTDIR ; then
   echo "INFO: Directory for core type created."
else
   echo "WARN: Failed to create directory for core type. It may already exist."
fi

OUTDIR=/home/malursem/working_dir/Instructions_counter_x86_64_res/geekbench5/$CORE_TYPE/instructions[${TIME}]

if mkdir $OUTDIR ; then
   echo "INFO: Directory for new results created."
else
   echo "WARN: Failed to create directory for results. It may already exist."
fi 

# Begin script in case all parameters are correct
sudo ./start_experiments.sh -f $CPU_FREQ

for workload in $workloadArray
do
   echo "Launching workload number: " $workload
	sudo ./Instructions_counter_x86_64 -k $CORE_TYPE -t $TIME -A $workload 2>> $OUTDIR/Instructions[${workload}].txt
done;

sudo ./end_experiments.sh