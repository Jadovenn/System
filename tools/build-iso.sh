#!/bin/bash

## 
## build-iso - generate an iso with grub2
## System source under license MIT
##

## $1 kernel bin file
## $2 release name
## $3 version

if test -f "$1.iso"; then
  exit 0
fi

rm -rf sysroot/boot/
mkdir -p sysroot/boot/grub
cp "$1" sysroot/boot/"$1"
./tools/generate-grub-config.sh $1 $2 $3 > sysroot/boot/grub/grub.cfg
grub-mkrescue -o "$1.iso" sysroot

