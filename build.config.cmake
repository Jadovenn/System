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

if (APPLE)
    set(QEMU_ACCEL tcg)
elseif(UNIX AND NOT APPLE)
    set(QEMU_ACCEL kvm)
endif()

## CPU unknown, cool to check with absolutely no features
##set(QEMU_ARGS -accel ${QEMU_ACCEL} -cpu base,+sse)
## CPU qemu
##set(QEMU_ARGS -accel ${QEMU_ACCEL} -cpu qemu32,+sse)
## CPU intel
set(QEMU_ARGS -accel ${QEMU_ACCEL} -cpu coreduo)
## CPU AMD
##set(QEMU_ARGS -accel ${QEMU_ACCEL} -cpu Opteron_G1)
