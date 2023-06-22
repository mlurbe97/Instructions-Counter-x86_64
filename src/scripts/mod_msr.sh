#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./mod_msr.sh -c msr_prefetch_conf [default->0xF] -n num_cpus [default->nproc]"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "c:n:" opt
do
   case "$opt" in
	  c ) CONFIG="$OPTARG" ;;
     n ) NUM_CORES="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

# Print helpFunction in case parameters are empty
if [ -z "$CONFIG" ]
then
   CONFIG="0xF"
fi

if [ -z "$NUM_CORES" ]
then
   NUM_CORES=$(nproc)
   echo "WARN: Using default NUM_CORES=" $NUM_CORES;
fi
ONE=1
let num_procesors=$NUM_CORES-$ONE

echo "Available cpu cores" $NUM_CORES
echo "Using this configuration" $CONFIG

sudo modprobe msr

for cpu in $(seq 0 $num_procesors);
do
	sudo wrmsr -p $cpu 0x1a4 $CONFIG
	echo "Core" $cpu "ready..."
done