##
## qemu-run.cmake - run target's config
## System sources under license MIT
##

set(KERNEL_SYM_FILE ${KERNEL_NAME}.sym)
set(KERNEL_SYM_FILE_STRIP ${KERNEL_NAME}.txt.sym)

find_program(QEMU_FOUND ${QEMU})

if (QEMU_FOUND)

    add_custom_target(qemu-run
            COMMAND
                ${QEMU} -m ${QEMU_PHYSICAL_MEM} -drive file=${KERNEL_ISO},format=raw
            DEPENDS
                ${KERNEL_ISO}
            COMMENT "Launching qemu")

    add_custom_target(${KERNEL_SYM_FILE}
            COMMAND
                ${CMAKE_OBJCOPY} --only-keep-debug sysroot/boot/${KERNEL_NAME} ${KERNEL_SYM_FILE}
            DEPENDS
                ${KERNEL_NAME}
            COMMENT "Generate kernel symbols file")

    add_custom_target(${KERNEL_SYM_FILE_STRIP}
            COMMAND
                ${CMAKE_NM} ${KERNEL_SYM_FILE} | grep "T" | ${CMAKE_AWK} '{ print $$1\" \"$$3 }' > ${KERNEL_SYM_FILE_STRIP}
            DEPENDS
                ${KERNEL_SYM_FILE}
            COMMENT "Generate kernel symbols file")

    add_custom_target(qemu-run-with-gdb-cli
            COMMAND
                ${QEMU} -m ${QEMU_PHYSICAL_MEM} -s -S -drive file=${KERNEL_ISO},format=raw&
            COMMAND
                ${CMAKE_GDB} -ex \"target remote localhost:1234\" -ex \"symbol-file sysroot/boot/${KERNEL_NAME}\"
            DEPENDS
                ${KERNEL_ISO} ${KERNEL_SYM_FILE}
            COMMENT "Launching qemu with gdb cli")

    add_custom_target(qemu-run-with-gdb-server
            COMMAND
                ${QEMU} -m ${QEMU_PHYSICAL_MEM} -s -S -drive file=${KERNEL_ISO},format=raw&
            DEPENDS
                ${KERNEL_ISO}
            COMMENT "Launching qemu with gdb server localhost:1234")

else()
    message(STATUS "qemu not found run custom command disabled")
endif()
