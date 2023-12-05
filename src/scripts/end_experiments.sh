#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./end_experiments.sh"
   exit 1 # Exit script after printing help
}

NUM_CORES=$(nproc)

ONE=1
let num_procesors=$NUM_CORES-$ONE
echo "Available cpu cores" $NUM_CORES
for cpu in $(seq 0 $num_procesors);
do
	{ sudo cpufreq-set -g ondemand -c $cpu; }
done;

echo "CPU Free..."