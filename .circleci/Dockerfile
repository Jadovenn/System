##
## Dockerfile to create a container with all the tool
## required to build System
##
## This script expect to have the tar archive of the toolchains
## in the current directory
##

FROM debian:stable-slim

RUN apt update
RUN apt install build-essential ncurses-bin curl nasm cmake make tcl tar git qemu --yes

COPY System-toolchain-Linux_x86-11.0.0-i386.tar.gz .
RUN mkdir -p /opt/System
RUN tar xvf System-toolchain-Linux_x86-11.0.0-i386.tar.gz -C /opt/System
