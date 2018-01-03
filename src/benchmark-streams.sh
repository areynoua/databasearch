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

actions="input output"
impls="1 2 3 4"

format="\"%C\",%U,%S,%E,%R,%F,%w,%I,%O";

max_open_files=$(getconf OPEN_MAX);
if (( $max_open_files > 30 )) ; then
	max_open_files=30;
fi

file_counts="1 2 5 15 30"

file_len() {
	if [[ $1 == "1" ]]; then
		echo 2097152; # 8 MiB
	else
		echo 4194304; # 16 MiB
	fi
}

params() {
	if [[ $1 == "3" ]]; then
		echo "32 64 128 256 512 1024 4096 16384"
	else
		if [[ $1 == "4" ]]; then
			echo "1 4 16 64 256 1024 4096 8192"
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
date --rfc-3339=seconds >> "$1"

for impl in $impls; do
	echo "Implementation $impl";
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
						if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impl $file_count $param ; then
							exit 1 ;
						fi
					fi
				fi
			done
		done
	done
done

#
#echo
#echo Output
#echo
#
#
#for impln in 1 2; do
#	len=$(file_len $impln)
#	echo ;
#	echo Implementation $impln;
#	for fn in $(seq 2 6 $max_open_files); do
#		echo "$fn files"
#		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len ; then
#			exit 1 ;
#		fi
#	done
#done
#
#for impln in 3 4; do
#	len=$(file_len $impln)
#	echo ;
#	echo Implementation $impln;
#	for param in $(params $impln) ; do
#		for fn in $(seq 2 6 $max_open_files); do
#			echo "$fn files"
#			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len $param ; then
#				exit 1 ;
#			fi
#		done
#	done
#done
#
#if [[ $EUID -eq 0 ]]; then
#	sync
#	echo 3 > /proc/sys/vm/drop_caches
#fi
#
#echo
#echo Input
#echo
#
#for impln in 1 2; do
#	echo ;
#	echo Implementation $impln;
#	for fn in $(seq 2 6 $max_open_files); do
#		echo "$fn files"
#		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams input $impln $fn ; then
#			exit 1 ;
#		fi
#		if [[ $EUID -eq 0 ]]; then
#			sync
#			echo 3 > /proc/sys/vm/drop_caches
#		fi
#	done
#done
#
#for impln in 3 4; do
#	echo ;
#	echo Implementation $impln;
#	for param in $(params $impln) ; do
#		for fn in $(seq 2 6 $max_open_files); do
#			echo "$fn files"
#			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams input $impln $fn $param ; then
#				exit 1 ;
#			fi
#			if [[ $EUID -eq 0 ]]; then
#				sync
#				echo 3 > /proc/sys/vm/drop_caches
#			fi
#		done
#	done
#done
