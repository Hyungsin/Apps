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
uint16_t nexthopChangeCt = 0;
uint16_t addressMsgCnt = 0;
uint8_t parentRequestMsgCnt = 0;
uint8_t childIDRequestCnt = 0;
uint8_t dataRequestCnt = 0;
uint8_t childUpdateReqCnt = 0;
uint8_t childUpdateRespCnt = 0;
uint8_t announceCnt = 0;
uint8_t orphanCnt = 0;
uint8_t discoveryCnt = 0;

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

    // next hop change 
    buf[17] = 0;    
    buf[16] = 0;
    // address msg counter
    buf[15] = 0;
    buf[14] = 0;

    // next hop
    buf[13] = 0;
    // link quality out
    buf[12] = 0;
    // link quality in
    buf[11] = 0;
    // path cost
    buf[10] = 0;
    // parent request message count
    buf[9] = 0;
    // child ID request message count
    buf[8] = 0;
    // data request count
    buf[7] = 0;
    // child update request count
    buf[6] = 0;
    // child update response count
    buf[5] = 0;
    // announce count
    buf[4] = 0;
    // orphan count
    buf[3] = 0;
    // discovery count
    buf[2] = 0;
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

        // next hop change number
        buf[16] = nexthopChangeCt & 0xff;
        buf[17] = (nexthopChangeCt >> 8) & 0xff;

        // address msg counter
        buf[14] = addressMsgCnt & 0xff;
        buf[15] = (addressMsgCnt >> 8) & 0xff;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA NEXT HOP CHANGE COUNT: %u\n",nexthopChangeCt);
        printf("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB ADDRESS QUERY COUNT: %u\n", addressMsgCnt);


        // Link quality and next hop
        otRouterInfo routerInfo;
        otThreadGetParentInfo(sInstance, &routerInfo);
        buf[10] = routerInfo.mPathCost;
        buf[11] = routerInfo.mLinkQualityIn;
        buf[12] = routerInfo.mLinkQualityOut;
        buf[13] = routerInfo.mNextHop;
        
        printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC PATH COST: %hu\n", routerInfo.mPathCost);
        printf("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD LQ IN: %hu\n", routerInfo.mLinkQualityIn);
        printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE LQ OUT: %hu\n", routerInfo.mLinkQualityOut);
        printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF NEXT HOP: %hu\n", routerInfo.mNextHop);
    
        buf[9] = parentRequestMsgCnt;
        printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG PARENT REQ MSG CNT: %hu\n", parentRequestMsgCnt);
        buf[8] = childIDRequestCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA CHILD ID REQ CNT: %hu\n", childIDRequestCnt);
        buf[7] = dataRequestCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA DATA REQ CNT: %hu\n", dataRequestCnt);
        buf[6] = childUpdateReqCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA CHILD UPDATE REQ CNT: %hu\n", childUpdateRespCnt);
        buf[5] = childUpdateRespCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA CHILD UPDATE RESP CNT: %hu\n", childUpdateRespCnt);
        buf[4] = announceCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ANNOUNCE CNT: %hu\n", announceCnt);
        buf[3] = orphanCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ORPHAN CNT: %hu\n", orphanCnt);
        buf[2] = discoveryCnt;
        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA DISCOVERY CNT: %hu\n", discoveryCnt);


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
