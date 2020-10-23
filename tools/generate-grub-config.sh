#!/bin/bash

##
## build-iso - generate an iso with grub2
## System source under license MIT
##

echo ""
echo "menuentry \"System $2 (v$3)\" {"
echo "    multiboot /boot/$1"
echo "    boot"
echo "}"
exit 0
