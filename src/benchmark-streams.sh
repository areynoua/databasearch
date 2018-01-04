#!/bin/bash

#   8 MiB   2097152 int32
#  16 MiB   4194304 int32
#  32 MiB   8388608 int32
#  64 MiB  16777216 int32
# 128 MiB  33554432 int32
# 256 MiB  67108864 int32
# 512 MiB 134217728 int32

if [[ $EUID -ne 0 ]]; then
	echo "This script should be run as root to enable cache clearing" 1>&2
	echo
fi

actions="output input"

# If 1 is too slow on your computer you can remove it below or
# descrease the size of files of this impl in file_len()
impls="1 2 3 4"

format="\"%C\",%U,%S,%E,%R,%F,%w,%I,%O";

max_open_files=$(getconf OPEN_MAX);
if (( $max_open_files > 30 )) ; then
	max_open_files=30;
fi

file_counts="1 2 10 30"

file_len() {
	if [[ $1 == "1" ]]; then
		echo 2097152; # 8 MiB
	else
		echo 4194304; # 16 MiB
	fi
}

params() {
	if [[ $1 == "3" ]]; then
		echo "44 64 88 128 180 256 360 512 724 1024 1448 2048 2896 4096 5792 8192 11584 16384 23168 32768"
	else
		if [[ $1 == "4" ]]; then
			echo "1 2 4 8 16 32 64 128 256 512 1024 2048"
		else
			echo "1"
		fi
	fi
}

clear_cache() {
if [[ $EUID -eq 0 ]]; then
	sync
	echo 3 > /proc/sys/vm/drop_caches
fi
}

clear_cache
#date --rfc-3339=seconds >> "$1"

for impl in $impls; do
	for action in $actions; do
		echo $action;
		for param in $(params $impl); do
			echo "Implementation $impl ($param)";
			for file_count in $file_counts; do
				if [ $file_count -le $max_open_files ] ; then
					echo "$file_count files"
					if [ $action = output ] ; then
						if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impl $file_count $(file_len $impl) $param ; then
							exit 1 ;
						fi
					else
						clear_cache
						if !  /usr/bin/time -a -o "$1" -f "$format" ./streams input $impl $file_count $param ; then
							exit 1 ;
						fi
					fi
				fi
			done
		done
	done
done

