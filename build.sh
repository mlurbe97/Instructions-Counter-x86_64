#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

helpFunction()
{
   echo ""
   echo "usage: sudo ./build.sh -b benches [GAP_BENCHMARKS, GEEK_BENCHMARKS or SPEC_BENCHMARKS]"
   exit 1 # Exit script after printing help
}

# Get the arguments
while getopts "b:" opt
do
   case "$opt" in
      b ) BENCHES="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

# Print helpFunction in case parameters are empty
if [ -z "$BENCHES" ]
then
   echo "ERROR: Missing arguments.";
   helpFunction
fi

rm -rf ../libpfm4/perf_examples/Instructions_counter_x86_64*
dos2unix src/scripts/* src/python/instructions_to_C_array.py
chmod +x src/scripts/* src/python/instructions_to_C_array.py
cp -rf *src/scripts/* src/python/instructions_to_C_array.py ../

if cp src/*.h src/benchmarks/*.h src/Makefile src/Instructions_counter_x86_64.c ../libpfm4/perf_examples/ ; then
	if cd ../libpfm4/perf_examples/ ; then
		if make BENCHES=$BENCHES; then
			if cp -rf Instructions_counter_x86_64 ../../ ; then
				echo "INFO: Process finished successfully."
			else
				echo "ERROR: Failed to copy programs."
			fi
		else
			echo "ERROR: Failed to compile the program."
		fi
	else
		echo "ERROR: Failed to change the directory."
	fi
else
	echo "ERROR: Failed to copy files."
fi

cd /home/malursem/working_dir/
OUTDIR=Instructions_counter_x86_64_res/
if mkdir $OUTDIR ; then
   echo "INFO: Directory for results created."
else
   echo "WARN: Failed to create directory for results. It may already exist."
fi