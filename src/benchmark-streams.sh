#!/bin/bash

alias time=/usr/bin/time

if [[ $EUID -ne 0 ]]; then
	echo "This script should be run as root to enable cache clearing" 1>&2
	echo
fi

format="%C\n%U,%S,%R,%F,%w,%I,%O";

filen=$(getconf OPEN_MAX);
if (( $filen > 30 )) ; then
	filen=30;
fi

filelen() {
	if [[ $1 == "1" ]]; then
		echo 33554432; # 128 MiB
	else
		if [[ $1 == "2" ]]; then
			echo 134217728; # 512 MiB
		else
			echo 268435456; # 1GiB
		fi
	fi
}


echo
echo Output
echo

if [[ $EUID -eq 0 ]]; then
	sync
	echo 3 > /proc/sys/vm/drop_caches
fi

date --rfc-3339=seconds >> "$1"

for impln in 1 2; do
	len=$(filelen $impln)
	echo ;
	echo Implementation $impln;
	for fn in $(seq 2 4 $filen); do
		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len ; then
			exit 1 ;
		fi
		if [[ $EUID -eq 0 ]]; then
			sync
			echo 3 > /proc/sys/vm/drop_caches
		fi
	done
done

for impln in 3 4; do
	len=$(filelen $impln)
	echo ;
	echo Implementation $impln;
	for param in 2 8 32 128 256 512 1024 4096 16384 ; do
		for fn in $(seq 2 4 $filen); do
			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $len $param ; then
				exit 1 ;
			fi
			if [[ $EUID -eq 0 ]]; then
				sync
				echo 3 > /proc/sys/vm/drop_caches
			fi
		done
	done
done

sync
echo 3 > /proc/sys/vm/drop_caches

echo
echo Input
echo

for impln in 1 2; do
	echo ;
	echo Implementation $impln;
	for fn in $(seq 2 4 $filen); do
		if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn ; then
			exit 1 ;
		fi
	done
done

for impln in 3 4; do
	echo ;
	echo Implementation $impln;
	for param in 2 8 32 128 256 512 1024 4096 16384 ; do
		for fn in $(seq 2 4 $filen); do
			if !  /usr/bin/time -a -o "$1" -f "$format" ./streams output $impln $fn $param ; then
				exit 1 ;
			fi
		done
	done
done
