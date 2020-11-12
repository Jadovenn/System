##
## build-header.cmake - fancy useless build header
## System sources under license MIT
##

add_custom_target(build-header ALL
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "      ___                       ___                         ___           ___ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "     /\\__\\                     /\\__\\                       /\\__\\         /\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "    /:/ _/_         ___       /:/ _/_         ___         /:/ _/_       |::\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "   /:/ /\\  \\       /|  |     /:/ /\\  \\       /\\__\\       /:/ /\\__\\      |:|:\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "  /:/ /::\\  \\     |:|  |    /:/ /::\\  \\     /:/  /      /:/ /:/ _/_   __|:|\\:\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta " /:/_/:/\\:\\__\\    |:|  |   /:/_/:/\\:\\__\\   /:/__/      /:/_/:/ /\\__\\ /::::|_\\:\\__\\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta " \\:\\/:/ /:/  /  __|:|__|   \\:\\/:/ /:/  /  /::\\  \\      \\:\\/:/ /:/  / \\:\\~~\\  \\/__/ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "  \\::/ /:/  /  /::::\\  \\    \\::/ /:/  /  /:/\\:\\  \\      \\::/_/:/  /   \\:\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "   \\/_/:/  /   ~~~~\\:\\  \\    \\/_/:/  /   \\/__\\:\\  \\      \\:\\/:/  /     \\:\\  \\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "     /:/  /         \\:\\__\\     /:/  /         \\:\\__\\      \\::/  /       \\:\\__\\ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta "     \\/__/           \\/__/     \\/__/           \\/__/       \\/__/         \\/__/ "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta " "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "           VERSION: ${KERNEL_VERSION}\t\tRELEASE NAME: ${RELEASE_NAME}"
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "      ARCHITECTURE: ${ARCH}\t\t       BOARD: ${BOARD}"
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --magenta " "
        VERBATIM)
