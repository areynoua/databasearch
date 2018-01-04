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

for M in 262144 524288 1048576 2097152 ; do
	for d in 2 4 8 16 ; do
		clear_cache
		echo ./mergesort \"output data/od.0\" $M $d
		if !  /usr/bin/time -a -o "$1" -f "$format" ./mergesort "output data/od.0" $M $d ; then
			exit 1 ;
		fi
	done
done
