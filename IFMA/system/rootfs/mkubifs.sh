#!/bin/sh
mkdir rootfs
sudo tar xvf rootfs.tar -C rootfs
echo "start make ubifs images"
sudo mkfs.ubifs -d rootfs -e 0x1f800 -c 512 -m 0x800 -x lzo -o rootfs.ubifs 

