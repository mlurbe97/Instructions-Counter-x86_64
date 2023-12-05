#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./start_experiments.sh -f cpu_freq"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "f:" opt
do
   case "$opt" in
      f ) CPU_FREQ="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

# Print helpFunction in case parameters are empty
if [ -z "$CPU_FREQ" ]
then
   echo "Missing argument CPU_FREQ.";
   helpFunction
fi

sudo ./enable_perf.sh
sudo ./mod_msr.sh

echo performance | tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor > /dev/null

echo $CPU_FREQ | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_min_freq > /dev/null

echo $CPU_FREQ | tee /sys/devices/system/cpu/cpufreq/policy*/scaling_max_freq > /dev/null 

echo "CPU Ready..."