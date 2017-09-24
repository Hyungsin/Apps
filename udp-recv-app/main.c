#include <stdio.h>
#include <rtt_stdio.h>
#include "xtimer.h"
#include <string.h>
#include "net/gnrc/pktdump.h"
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
#define SAMPLE_INTERVAL (100000000UL)
#endif

void send_udp(char *addr_str, uint16_t port, uint8_t *data, uint16_t datalen);

int main(void) {
    //This value is good randomness and unique per mote
    srand(*((uint32_t*)fb_aes128_key));
    dmac_init();
    spi_set_dma_channel(0, 0, 1);

	LED_OFF;

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    gnrc_netreg_entry_t server = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &server);

    msg_t msg;
    char stringbuf[127];
    for (;;) {
        msg_receive(&msg);

        gnrc_pktsnip_t* pkt = msg.content.ptr;
        uint8_t* data = pkt->data + 8; // Don't use UDP header
        size_t data_size = pkt->size - 8;
        size_t to_copy = (data_size < sizeof(stringbuf)) ? data_size : (sizeof(stringbuf) - 1);
        memcpy(stringbuf, data, to_copy);
        stringbuf[to_copy] = '\0';
        printf("Got a packet: %s\n", stringbuf);
        gnrc_pktbuf_release(pkt);
    }

    return 0;
}
