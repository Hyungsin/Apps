APPLICATION = hamilton_easyapp
RIOTBASE = /home/hskim/Desktop/RIOT-OS
BOARD = hamilton

# System functions
USEMODULE += random
USEMODULE += xtimer
USEMODULE += rtt_stdio
CFLAGS    += -DRTT_STDIO_DISABLE_STDIN
CFLAGS    += -DTHREAD_STACKSIZE_DEFAULT=2048

# Sensors
USEMODULE += saul_reg        # SAUL: sensor/actuator abstraction layer 
USEMODULE += auto_init_saul  
USEMODULE += tmp006          # ambient temperature
USEMODULE += hdc1000         # humidity and temperature
USEMODULE += fxos8700        # acceleration and magnetic field
USEMODULE += apds9007        # illumination
USEMODULE += ekmb1101111     # pir-based occupancy
USEMODULE += push_button     # simple button

# Security
USEMODULE += crypto
USEMODULE += cipher_modes
CFLAGS    += -DCRYPTO_AES

# Radio
USEMODULE += at86rf233
CFLAGS    += -DIEEE802154_DEFAULT_CHANNEL=26

# Network
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Duty-cycling 
USEMODULE += gnrc_dutymac
CFLAGS    += -DDUTYCYCLE_SLEEP_INTERVAL=000000UL # If it is ZERO, no duty-cycling for packet reception
CFLAGS    += -DLEAF_NODE=1                       # Default is 1  
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6
USEMODULE += gnrc_udp
CFLAGS    += -DSOCK_HAS_IPV6
# Add also the shell, some shell commands
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
# CFLAGS += -DDEVELHELP

QUIET ?= 1

FEATURES_REQUIRED += periph_timer

include $(RIOTBASE)/Makefile.include
