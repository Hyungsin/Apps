/*
 * Copyright (C) 2017 Baptiste CLENET
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @brief       OpenThread test application
 *
 * @author      Baptiste Clenet <bapclenet@gmail.com>
 */

#include <stdio.h>
#include <rtt_stdio.h>
#include "ot.h"
#include <openthread/udp.h>
#include <openthread/cli.h>
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>
#include <openthread/platform/logging.h>
//#include "periph/dmac.h"
#include "periph/adc.h"
#include "periph/i2c.h"
#include "periph/spi.h"

#ifndef SAMPLE_INTERVAL
#define SAMPLE_INTERVAL (30000000UL)
#endif
#define SAMPLE_JITTER   (15000000UL)

#define ENABLE_DEBUG (1)
#include "debug.h"
uint32_t interval_with_jitter(void)
{
    int32_t t = SAMPLE_INTERVAL;
    t += rand() % SAMPLE_JITTER;
    t -= (SAMPLE_JITTER >> 1);
    return (uint32_t)t;
}
uint32_t rtchangeCt = 0;
int main(void)
{
#if DMAC_ENABLE
    dmac_init();
    adc_set_dma_channel(DMAC_CHANNEL_ADC);
    i2c_set_dma_channel(I2C_0,DMAC_CHANNEL_I2C);
    spi_set_dma_channel(0,DMAC_CHANNEL_SPI_TX,DMAC_CHANNEL_SPI_RX);
#endif 
    DEBUG("This a test for OpenThread\n");    
    //xtimer_usleep(300000000ul);

    xtimer_usleep(3000ul);
    DEBUG("[Main] Start UDP\n");    
    // get openthread instance
	otUdpSocket mSocket;
	otError error;
    otInstance *sInstance = openthread_get_instance();
	if (sInstance == NULL) {
        DEBUG("error in init");
    }

    DEBUG("[Main] Msg Creation\n");    
    otMessageInfo messageInfo;
	otMessage *message = NULL;
	memset(&messageInfo, 0, sizeof(messageInfo));
	otIp6AddressFromString("fdde:ad00:beef:0000:c684:4ab6:ac8f:9fe5", &messageInfo.mPeerAddr);
    messageInfo.mPeerPort = 1234;
    messageInfo.mInterfaceId = 1;
    char buf[20];
    for (int i =0; i<20; i++) {
        buf[i] = 0xff;
    }
    buf[19] = 0;
    buf[18] = 0;

    // Routing table changed on packet send.
    buf[17] = 0;    
    buf[16] = 0;
    buf[15] = 0;
    buf[14] = 0;
	while (1) {
		//Sample
	    //sample(&frontbuf);
		//aes_populate();
		//Sleep
        xtimer_usleep(interval_with_jitter());
        

		//Send
        message = otUdpNewMessage(sInstance, true);
        if (message == NULL) {
            printf("error in new message");
        }

        // Tx Sequence number setting
        buf[19]++;
        if (buf[19] == 0) {
            buf[18]++;
        }       

        // Update routing table change number
        buf[14] = rtchangeCt & 0xff;
        buf[15] = (rtchangeCt >> 8) & 0xff;
        buf[16] = (rtchangeCt >> 16) & 0xff;
        buf[17] = (rtchangeCt >> 24) & 0xff;

        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA%lu\n",rtchangeCt);
        // Source addr setting
        uint8_t source = OPENTHREAD_SOURCE;
        buf[0] = source;
        error = otMessageSetLength(message, 20);
        if (error != OT_ERROR_NONE) {
            printf("error in set length\n");
        }
        otMessageWrite(message, 0, buf, 20);
		
        DEBUG("[Main] Tx UDP packet\n");
        error = otUdpSend(&mSocket, message, &messageInfo);
        if (error != OT_ERROR_NONE) {
            printf("error in udp send\n");
        }
    }
    return 0;
}
