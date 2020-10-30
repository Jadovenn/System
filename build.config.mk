##
## build.config - Build configuration
## System sources under license MIT
##

##############
##  NAMING  ##
##############

VERSION		= 0.0.3
RELEASE_NAME	= bare_bones

##############
##  MODES   ##
##############

MODE		= debug
ARCH		= i386

##############
##   QEMU   ##
##############

QEMU		= qemu-system-$(ARCH)
# Max without PAE on QEMU 3580M
# Min recommended 128M, consume only 1 page
PHYSICAL_MEM	= 128M