##
## build.config.cmake - Build configuration
## System sources under license MIT
##

############################
## Build config variables ##
############################

set(KERNEL_VERSION 0.0.3)
set(RELEASE_NAME BareBones)

set(ARCH i386)
set(BOARD Generic)

set(KERNEL_NAME System_${KERNEL_VERSION}-${RELEASE_NAME}-${ARCH}_${BOARD})

############################
## qemu run config        ##
############################

set(QEMU qemu-system-${ARCH})
set(QEMU_PHYSICAL_MEM 128M)
## CPU qemu
##set(QEMU_ARGS -accel tcg -cpu base)
## CPU intel
set(QEMU_ARGS -accel tcg -cpu coreduo,+sse)
## CPU AMD
##set(QEMU_ARGS -accel tcg -cpu Opteron_G1,+sse)
