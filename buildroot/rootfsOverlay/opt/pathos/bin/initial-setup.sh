#!/bin/sh

SCRIPT=$(readlink -f $0)
SCRIPT_DIR=$(dirname $SCRIPT)

if [ $(id -u) -ne 0 ]; then
	echo "this script must be run as root"
	exit 1
fi

echo "remounting rootfs as rw"
mount / -o remount,rw

while true;
do
	echo "change root password:"
	passwd
	if [ $? -eq 0 ]; then
		break
	fi
done

while true;
do
	echo "change pathos password:"
	passwd pathos
	if [ $? -eq 0 ]; then
		break
	fi
done

echo "adding data partition to fstab"
grep /mnt/data /etc/fstab > /dev/null
if [ $? -ne 0 ]; then
        echo "/dev/mmcblk0p2    /mnt/data       ext4    defaults,noatime 0      2" >> /etc/fstab
	resize2fs /dev/mmcblk0p3
fi

echo "enabling sudo group"
sed -i 's/# %sudo/%sudo/' /etc/sudoers

echo "factory reset"
$SCRIPT_DIR/factory-reset.sh

