#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "This script must be run as root" 1>&2
	echo
	exit 1
fi

read -p "Enter your disk name (ex: sda) " disk

echo "Write disk information to $1 (please wait 10 secs)"

echo "Result of timings on disk $disk" > "$1"
( hdparm -t "/dev/$disk" | tail -n 1 ) >> "$1"
( hdparm -T "/dev/$disk" | tail -n 1 ) >> "$1"

echo "Write cpu information to $1"
echo >> $1
echo "CPUs" >> $1
lscpu >> $1

echo "Write detailed disk information to $1"
echo >> $1
echo "disk $disk" >> $1
hdparm -I "/dev/$disk" >> $1
