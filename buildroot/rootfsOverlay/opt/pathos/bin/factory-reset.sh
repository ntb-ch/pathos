#!/bin/sh

if [ $(id -u) -ne 0 ]; then
	echo "this script must be run as root"
	exit 1
fi

echo "unmounting data partition"
umount /dev/mmcblk0p2

echo "fromating data partition"
yes | mkfs -t ext4 -q /dev/mmcblk0p2
if [ $? -ne 0 ]; then
	echo "cannot format data partition"
	exit 1
fi

echo "mounting all partitions"
mount -a

chown pathos:users /mnt/data
chmod a+rw /mnt/data

reboot

