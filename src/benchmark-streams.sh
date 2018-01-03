#!/bin/bash

#   8 MiB   2097152 int32
#  16 MiB   4194304 int32
#  32 MiB   8388608 int32
#  64 MiB  16777216 int32
# 128 MiB  33554432 int32
# 256 MiB  67108864 int32
# 512 MiB 134217728 int32

alias time=/usr/bin/time

if [[ $EUID -ne 0 ]]; then
	echo "This script should be run as root to enable cache clearing" 1>&2
	echo
fi

format="\"%C\",%U,%S,%E,%R,%F,%w,%I,%O";

filen=$(getconf OPEN_MAX);
if (( $filen > 30 )) ; then
	filen=30;
fi

filelen() {
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
		echo "1 4 16 64 256 1024 4096 16384"
	fi
}

if [[ $EUID -eq 0 ]]; then
	sync
	echo 3 > /proc/sys/vm/drop_caches
fi

echo
echo Output
echo

date --rfc-3339=seconds >> "$1"

#for impln in 1 2; do
#	len=$(filelen $impln)
#	echo ;
#	echo Implementation $impln;
#	for fn in $(seq 2 6 $filen); do
#		echo "$fn files"
#		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len ; then
#			exit 1 ;
#		fi
#	done
#done
#
#for impln in 3 4; do
#	len=$(filelen $impln)
#	echo ;
#	echo Implementation $impln;
#	for param in $(params $impln) ; do
#		for fn in $(seq 2 6 $filen); do
#			echo "$fn files"
#			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len $param ; then
#				exit 1 ;
#			fi
#		done
#	done
#done

if [[ $EUID -eq 0 ]]; then
	sync
	echo 3 > /proc/sys/vm/drop_caches
fi

echo
echo Input
echo

for impln in 1 2; do
	echo ;
	echo Implementation $impln;
	for fn in $(seq 2 6 $filen); do
		echo "$fn files"
		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams input $impln $fn ; then
			exit 1 ;
		fi
		if [[ $EUID -eq 0 ]]; then
			sync
			echo 3 > /proc/sys/vm/drop_caches
		fi
	done
done

for impln in 3 4; do
	echo ;
	echo Implementation $impln;
	for param in $(params $impln) ; do
		for fn in $(seq 2 6 $filen); do
			echo "$fn files"
			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams input $impln $fn $param ; then
				exit 1 ;
			fi
			if [[ $EUID -eq 0 ]]; then
				sync
				echo 3 > /proc/sys/vm/drop_caches
			fi
		done
	done
done
