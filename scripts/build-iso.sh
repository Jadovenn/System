#!/bin/bash

## 
## build-iso - generate an iso with grub2
## System source under license MIT
##

mkdir -p isodir/boot/grub
cp system isodir/boot/system
cp scripts/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o "$1" isodir
rm -rf isodir
exit 0

