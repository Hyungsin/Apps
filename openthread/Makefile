APPLICATION = openthread

# If no BOARD is found in the environment, use this default:
BOARD ?= hamilton

# These are the boards that OpenThread stack has been tested on
BOARD_WHITELIST := hamilton samr21-xpro iotlab-m3 fox iotlab-a8-m3

# This has to be the absolute path to the RIOT base directory:
RIOTBASE = /home/hskim/Desktop/RIOT-OS

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#CFLAGS += -DDEVELHELP -Wall

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEPKG += openthread
OPENTHREAD_TYPE ?= ftd
ifeq ($(OPENTHREAD_TYPE),mtd)
    # MTD: A Minimal Thread Device does not have router functionality
    # compiled in. As a result, it is not necessary to configure the
    # routerrole on an MTD. At the same time, an MTD may or may not be sleepy.
    USEMODULE += openthread-mtd
    USEMODULE += openthread-cli-mtd
else
    # ftd: A Full Thread Device has router functionality compiled in
    USEMODULE += openthread-ftd
    USEMODULE += openthread-cli-ftd
endif

#Define PANID, CHANNEL and UART_BAUDRATE used by default
OPENTHREAD_PANID ?= 0xbeef
OPENTHREAD_CHANNEL ?= 26
OPENTHREAD_UART_BAUDRATE ?= 115200

CFLAGS += -DOPENTHREAD_PANID=${OPENTHREAD_PANID}
CFLAGS += -DOPENTHREAD_CHANNEL=${OPENTHREAD_CHANNEL}
CFLAGS += -DOPENTHREAD_UART_BAUDRATE=${OPENTHREAD_UART_BAUDRATE}

ifneq (,$(filter hamilton samr21-xpro,$(BOARD)))
  DRIVER := at86rf233
endif
ifneq (,$(filter iotlab-m3 fox iotlab-a8-m3,$(BOARD)))
  DRIVER := at86rf231
endif

ifneq (,$(filter at86rf2%,$(DRIVER)))
  FEATURES_REQUIRED += periph_spi periph_gpio
endif

USEMODULE += $(DRIVER)

# CPU clock speed
CFLAGS    += -DCLOCK_USE_OSCULP32_DFLL=1

# Sensors
USEMODULE += saul_reg        # SAUL: sensor/actuator abstraction layer 
USEMODULE += auto_init_saul  
USEMODULE += tmp006          # ambient temperature
USEMODULE += hdc1000         # humidity and temperature
USEMODULE += fxos8700        # acceleration and magnetic field
USEMODULE += apds9007        # illumination
USEMODULE += ekmb1101111     # pir-based occupancy
USEMODULE += push_button     # simple button

USEMODULE += xtimer
USEMODULE += random
USEMODULE += ps

USEMODULE += rtt_stdio
CFLAGS    += -DRTT_STDIO_DISABLE_STDIN
CFLAGS    += -DTHREAD_STACKSIZE_DEFAULT=2048

#required for C++ compiling
CXXEXFLAGS += -fno-rtti
USEMODULE += cpp11-compat

include $(RIOTBASE)/Makefile.include
