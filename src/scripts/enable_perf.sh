#!/bin/bash

## Instructions Counter x86_64.
## Author: Manel Lurbe Sempere
## e-mail: malursem@gap.upv.es
## Year: 2022

sudo sh -c 'echo -1 > /proc/sys/kernel/perf_event_paranoid' .

sudo modprobe msr