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
#include "debug.h"
#include <openthread/udp.h>
#include <openthread/cli.h>
#include <openthread/thread.h>
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
uint32_t interval_with_jitter(void)
{
    int32_t t = SAMPLE_INTERVAL;
    t += rand() % SAMPLE_JITTER;
    t -= (SAMPLE_JITTER >> 1);
    return (uint32_t)t;
}
uint8_t borderRouteChangeCnt = 0;
uint8_t borderRouterLC = 0;
char nexthopBuffer[40];

uint32_t addressMsgCnt = 0;
uint32_t joiningMsgCnt = 0;
uint32_t routingMsgCnt = 0;
uint32_t linkMsgCnt = 0;
uint32_t controlMsgCnt = 0;
uint32_t packetSuccessCnt = 0;
uint32_t packetFailCnt = 0;
uint32_t packetBusyChannelCnt = 0;
uint32_t broadcastCnt = 0;

int main(void)
{
#if DMAC_ENABLE
    dmac_init();
    adc_set_dma_channel(DMAC_CHANNEL_ADC);
    i2c_set_dma_channel(I2C_0,DMAC_CHANNEL_I2C);
    spi_set_dma_channel(0,DMAC_CHANNEL_SPI_TX,DMAC_CHANNEL_SPI_RX);
#endif 
    DEBUG("This a test for OpenThread\n");    
    xtimer_usleep(300000000ul);

    //xtimer_usleep(3000ul);
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
    char buf[84];
    for (int i =0; i<84; i++) {
        buf[i] = 0xff;
    }

    for (int i = 0; i < 84; i++) {
        buf[i] = 0x0;
    }
	
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

        // Address message counts
        buf[17] = addressMsgCnt & 0xff;
        buf[16] = (addressMsgCnt >> 8) & 0xff;
        buf[15] = (addressMsgCnt >> 16) & 0xff;
        buf[14] = (addressMsgCnt >> 24) & 0xff;

        // next hop change number
        buf[13] = borderRouteChangeCnt;

        // Link quality, next hop, path cost
        // otRouterInfo routerInfo;
        // otThreadGetParentInfo(sInstance, &routerInfo);
        // buf[9] = routerInfo.mPathCost;
        // buf[10] = routerInfo.mLinkQualityIn;
        // buf[11] = routerInfo.mLinkQualityOut;
        // buf[12] = routerInfo.mNextHop;

        buf[9] = borderRouterLC;
        // buf[10] = border


        buf[8] = joiningMsgCnt & 0xff;
        buf[7] = (joiningMsgCnt >> 8) & 0xff; 
        buf[6] = (joiningMsgCnt >> 16) & 0xff; 
        buf[5] = (joiningMsgCnt >> 24) & 0xff; 

        buf[4] = routingMsgCnt & 0xff;
        buf[3] = (routingMsgCnt >> 8) & 0xff; 
        buf[2] = (routingMsgCnt >> 16) & 0xff; 
        buf[1] = (routingMsgCnt >> 24) & 0xff;

        buf[23] = linkMsgCnt & 0xff;
        buf[22] = (linkMsgCnt >> 8) & 0xff; 
        buf[21] = (linkMsgCnt >> 16) & 0xff; 
        buf[20] = (linkMsgCnt >> 24) & 0xff;

        buf[27] = controlMsgCnt & 0xff;
        buf[26] = (controlMsgCnt >> 8) & 0xff; 
        buf[25] = (controlMsgCnt >> 16) & 0xff; 
        buf[24] = (controlMsgCnt >> 24) & 0xff;

        buf[31] = packetSuccessCnt & 0xff;
        buf[30] = (packetSuccessCnt >> 8) & 0xff; 
        buf[29] = (packetSuccessCnt >> 16) & 0xff; 
        buf[28] = (packetSuccessCnt >> 24) & 0xff;

        buf[35] = packetFailCnt & 0xff;
        buf[34] = (packetFailCnt >> 8) & 0xff; 
        buf[33] = (packetFailCnt >> 16) & 0xff; 
        buf[32] = (packetFailCnt >> 24) & 0xff;

        buf[39] = packetBusyChannelCnt & 0xff;
        buf[38] = (packetBusyChannelCnt >> 8) & 0xff; 
        buf[37] = (packetBusyChannelCnt >> 16) & 0xff; 
        buf[36] = (packetBusyChannelCnt >> 24) & 0xff;

        buf[43] = broadcastCnt & 0xff;
        buf[42] = (broadcastCnt >> 8) & 0xff; 
        buf[41] = (broadcastCnt >> 16) & 0xff; 
        buf[40] = (broadcastCnt >> 24) & 0xff;

        for (int i = 0; i < 40; i++) {
            buf[i+44] = nexthopBuffer[i];
        }

        printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC LINK COST: %hu\n", borderRouterLC);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BORDER ROUTER NEXT HOP CHANGE CNT: %hu\n", borderRouteChangeCnt);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ADDRESS MSG CNT: %lu\n", addressMsgCnt);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA JOINING MSG CNT: %lu\n", joiningMsgCnt);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ROUTING MSG CNT: %lu\n", routingMsgCnt);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA LINK MSG CNT: %lu\n", linkMsgCnt);
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA CONTROl MSG CNT: %lu\n", controlMsgCnt);

        // Source addr setting
        uint8_t source = OPENTHREAD_SOURCE;
        buf[0] = source;
        error = otMessageSetLength(message, 84);
        if (error != OT_ERROR_NONE) {
            printf("error in set length\n");
        }
        otMessageWrite(message, 0, buf, 84);
		
        DEBUG("[Main] Tx UDP packet\n");
        error = otUdpSend(&mSocket, message, &messageInfo);
        if (error != OT_ERROR_NONE) {
            printf("error in udp send\n");
        }
    }
    return 0;
}
