#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

rm -rf ../libpfm4/perf_examples/Instructions_counter_x86_64*

if chmod +x run.sh start_experiments.sh end_experiments.sh ; then
	echo "INFO: Execution permissions granted to run.sh, start_experiments.sh and end_experiments.sh successfully."
else
	echo "ERROR: Failed to give execution permissions to run.sh, start_experiments.sh and end_experiments.sh."
fi

if cp -rf run.sh ../ ; then
   echo "INFO: run.sh script copied to working dir."
else
   echo "ERROR: Failed to copy run.sh script."
fi

if cp -rf start_experiments.sh ../ ; then
   echo "INFO: start_experiments.sh script copied to working dir."
else
   echo "ERROR: Failed to copy start_experiments.sh script."
fi

if cp -rf end_experiments.sh ../ ; then
   echo "INFO: end_experiments.sh script copied to working dir."
else
   echo "ERROR: Failed to copy end_experiments.sh script."
fi

if cp src/Makefile src/Instructions_counter_x86_64.c ../libpfm4/perf_examples/ ; then
	if cd ../libpfm4/perf_examples/ ; then
		if make ; then
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

cd $HOME/working_dir/
OUTDIR=Instructions_counter_x86_64_res/
if mkdir $OUTDIR ; then
   echo "INFO: Directory for results created."
else
   echo "WARN: Failed to create directory for results. It may already exist."
fi