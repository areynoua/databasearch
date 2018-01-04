#!/bin/bash

./streams output 3 1 2097152 512

format="\"%C\",%U,%S,%E,%R,%F,%w,%I,%O";

clear_cache() {
if [[ $EUID -eq 0 ]]; then
	sync
	echo 3 > /proc/sys/vm/drop_caches
fi
}

#date --rfc-3339=seconds >> "$1"

<<<<<<< HEAD
for M in 262144 524288 1048576 2097152 4194304; do
	for d in 2 4 8 16 ; do
		clear_cache
		echo ./mergesort \"output data/od.18\" $M $d
		#if !  /usr/bin/time -a -o "$1" -f "$format" ./mergesort "output data/od.0" $M $d ; then
		if !  /usr/bin/time -p ./mergesort "output data/od.0" $M $d ; then
=======
for M in 262144 524288 1048576 2097152 ; do
	for d in 2 4 8 16 ; do
		clear_cache
		echo ./mergesort \"output data/od.0\" $M $d
		if !  /usr/bin/time -a -o "$1" -f "$format" ./mergesort "output data/od.0" $M $d ; then
>>>>>>> 57172922fa557ae99a80c8227833112eafea7e69
			exit 1 ;
		fi
	done
done
