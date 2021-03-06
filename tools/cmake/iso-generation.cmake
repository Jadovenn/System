##
## qemu-run.cmake - iso creation target's config
## System sources under license MIT
##

set(GRUB_CFG sysroot/boot/grub/grub.cfg)

file(GENERATE OUTPUT ${GRUB_CFG} CONTENT "
GRUB_TIMEOUT=0.1

multiboot /boot/${KERNEL_NAME}
boot

")

set(GRUB_MKRESCUE grub-mkrescue)
set(KERNEL_ISO ${KERNEL_NAME}.iso)

find_program(GRUB_MKRESCUE_FOUND ${GRUB_MKRESCUE})

if (GRUB_MKRESCUE_FOUND)

add_custom_target(${KERNEL_ISO}
        COMMAND
            ${GRUB_MKRESCUE} -o ${KERNEL_ISO} sysroot
        DEPENDS
            ${KERNEL_NAME}
        COMMENT
            "Generate ISO file")
else()
    message(STATUS "${GRUB_MKRESCUE} not found, iso generation disabled")
endif()
