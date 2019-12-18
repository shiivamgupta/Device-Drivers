# execute in root mode at same folder

#!/bin/bash

rmmod proc_seq_example1.ko

echo 0  > /sys/kernel/debug/tracing/tracing_on 

echo function >  /sys/kernel/debug/tracing/current_tracer

insmod proc_seq_example1.ko

echo '*:mod:proc_seq_example1' >> /sys/kernel/debug/tracing/set_ftrace_filter 

echo proc_reg_*  >> /sys/kernel/debug/tracing/set_ftrace_filter

#echo do_dentry_open  >> /sys/kernel/debug/tracing/set_ftrace_filter


# for live tracing 
#cat /sys/kernel/debug/tracing/trace_pipe


cat  /sys/kernel/debug/tracing/set_ftrace_filter 

gcc proc_seq_test.c

echo 1  > /sys/kernel/debug/tracing/tracing_on 

./a.out

echo 0  > /sys/kernel/debug/tracing/tracing_on

cat /sys/kernel/debug/tracing/trace | less



