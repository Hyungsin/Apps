#include <stdio.h>
#include <rtt_stdio.h>
#include "xtimer.h"
#include <string.h>
#include "net/gnrc/udp.h"
#include "phydat.h"
#include "saul_reg.h"
#include "periph/adc.h"
#include "periph/dmac.h"
#include "periph/i2c.h"
#include "periph/spi.h"
#include "periph/timer.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#ifndef SAMPLE_INTERVAL
#define SAMPLE_INTERVAL (1000000UL)
#endif

void send_udp(char *addr_str, uint16_t port, uint8_t *data, uint16_t datalen);

int main(void) {
    //This value is good randomness and unique per mote
    srand(*((uint32_t*)fb_aes128_key));
    dmac_init();
    spi_set_dma_channel(0, 0, 1);

	LED_OFF;

    while (1) {
        char* message = "Hello, world!";
        uint16_t message_length = (uint16_t) strlen(message);
		send_udp("ff02::1", 8943, (uint8_t*) message, message_length);
		//Sleep
		xtimer_usleep(SAMPLE_INTERVAL);
    }

    return 0;
}
