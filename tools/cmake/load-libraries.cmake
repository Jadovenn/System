##
## load-libraries.cmake - Project libraries loader
## System sources under license MIT
##

function(load_if_arch_present  PATH)
    if (NOT EXISTS "${PATH}/dependency.config")
        message(FATAL_ERROR " -- ${PATH} has no dependency.config please!")
    endif()
    file(STRINGS ${PATH}/dependency.config ConfigContents)
    foreach(NameAndValue ${ConfigContents})
        string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
        string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
        if (${Name} STREQUAL "architecture ")
            string(REPLACE "${Name}=" "" Values ${NameAndValue})
            string(FIND ${Values} "${ARCH}" POSITION)
            if (NOT "${POSITION}" STREQUAL "-1")
                add_subdirectory(${PATH})
            endif()
        endif()
    endforeach()
endfunction(load_if_arch_present)

function(load_libraries PATH)
    FILE(GLOB DIRS
            LIST_DIRECTORIES true
            ${PATH}/*)
    FOREACH(SUB_DIR ${DIRS})
        if (IS_DIRECTORY ${SUB_DIR})
            load_if_arch_present(${SUB_DIR})
        endif()
    ENDFOREACH()
endfunction(load_libraries)
