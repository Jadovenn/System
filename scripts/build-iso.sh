#!/bin/bash

## 
## build-iso - generate an iso with grub2
## System source under license MIT
##

cp system.kern sysroot/boot/system.kern
grub-mkrescue -o "$1" sysroot
exit 0

