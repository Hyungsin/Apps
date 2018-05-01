APPLICATION = openthread_border_router

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# These are the boards that OpenThread stack has been tested on
BOARD_WHITELIST := samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /root/RIOT-OS

# This is to set the proper programmer for board
PROGRAMMER ?= jlink

USEMODULE += rtt_stdio
CFLAGS    += -DRTT_STDIO_DISABLE_STDIN

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#CFLAGS += -DDEVELHELP -Wall

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# NCP: A Network Co-Processor is used with wpantund software. wpantund is a user-space network
# interface driver/daemon that provides a native IPv6 network interface to a low-power wireless Network Co-Processor
# FTD (see openthread_test_ftd): NCP needs Full Thread Device to act as a router in the network
USEPKG += openthread
USEMODULE += openthread-ftd
USEMODULE += openthread-ncp-ftd

CFLAGS += -DHIGH_DATA_RATE=1
CFLAGS += -DTHREAD_STACKSIZE_DEFAULT=2048

# Define UART_BAUDRATE, PANID, CHANNEL, and TXPOWER used by default
# These parameters can be modified by wpantund commands
CFLAGS += -DOPENTHREAD_UART_BAUDRATE=1000000
CFLAGS += -DOPENTHREAD_PANID=0xbeef
CFLAGS += -DOPENTHREAD_CHANNEL=26
CFLAGS += -DOPENTHREAD_TXPOWER=4

FEATURES_REQUIRED += periph_spi periph_gpio
USEMODULE += at86rf233
USEMODULE += xtimer
USEMODULE += random
USEMODULE += ps
USEMODULE += schedstatistics
CFLAGS += -DAUTO_CSMA_EN=1

#required for C++ compiling
CXXEXFLAGS += -fno-rtti
USEMODULE += cpp11-compat

include $(RIOTBASE)/Makefile.include
