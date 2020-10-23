##
## dependencies.config - dependencies declaration
## System sources under license MIT
##

INCLUDE_DEPENDENCIES	=	lib/libc/dependency.mk

build-dependencies:
	@$(ECHO) $(COLOR_YELLOW) "*** GENERATING" $(COLOR_TEAL) lib/libc
	@$(MAKE) -C lib/libc

clean-dependencies:
	@$(ECHO) $(COLOR_YELLOW) "x CLEANING" $(COLOR_TEAL) lib/libc
	@$(MAKE) -C lib/libc clean
