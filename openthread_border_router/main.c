/*
 * Copyright (C) 2018 UC Berkeley
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @brief       OpenThread border router application
 *
 * @author      Hyung-Sin Kim <hs.kim@cs.berkeley.edu>
 */

#include <stdio.h>
#include "ot.h"
#include <openthread/udp.h>
#include <openthread/cli.h>
#include <openthread/openthread.h>
#include "sched.h"
#include "periph/gpio.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

/* LED pin configuration */
#define D1_PIN GPIO_PIN(0, 27)
#define D2_PIN GPIO_PIN(1, 23)
#define D3_PIN GPIO_PIN(1, 22)
#define D5_PIN GPIO_PIN(0, 23)

#define BLINK_INTERVAL 1000000ul

static bool led_serial_on = false;
static bool led_mcu_on = false;

/* OpenThread network parameters */
uint16_t myRloc = 0;

uint32_t packetReTxArray[11];
uint32_t packetReTxCnt = 0;
uint32_t packetSuccessCnt = 0;
uint32_t packetFailCnt = 0;
uint32_t packetBusyChannelCnt = 0;
uint32_t broadcastCnt = 0;
uint32_t queueOverflowCnt = 0;

uint32_t totalIpv6MsgCnt = 0;
uint32_t Ipv6TxSuccCnt = 0;
uint32_t Ipv6TxFailCnt = 0;
uint32_t Ipv6RxSuccCnt = 0;
uint32_t Ipv6RxFailCnt = 0;

uint16_t nextHopRloc = 0;
uint8_t borderRouterLC = 0;
uint8_t borderRouterRC = 0;
uint32_t borderRouteChangeCnt = 0;
uint32_t routeChangeCnt = 0;

uint32_t pollMsgCnt = 0;
uint32_t mleMsgCnt = 0;

uint32_t mleRouterMsgCnt = 0;
uint32_t addrMsgCnt = 0;
uint32_t netdataMsgCnt = 0;

uint32_t meshcopMsgCnt = 0;
uint32_t tmfMsgCnt = 0;

uint32_t totalSerialMsgCnt = 0;


void wdt_clear(void) {
    volatile uint8_t* wdt_status = (volatile uint8_t*) 0x40001007;
    volatile uint8_t* wdt_clear = (volatile uint8_t*) 0x40001008;

    while (*wdt_status);
    *wdt_clear = 0xA5;
    while (*wdt_status);

    /* Blinking Serial led */
    if (led_serial_on) {
        gpio_clear(D5_PIN);
        led_serial_on = false;
    } else {
        gpio_set(D5_PIN);
        led_serial_on = true;
    }
}

void wdt_setup(void) {
    /* Enable the bus for WDT and PAC0. */
    volatile uint32_t* pm_apbamask = (volatile uint32_t*) 0x40000418;
    *pm_apbamask = 0x0000007f;

    /* Setup GCLK_WDT at (32 kHz) / (2 ^ (7 + 1)) = 128 Hz. */
    GCLK->GENDIV.reg  = (GCLK_GENDIV_ID(3)  | GCLK_GENDIV_DIV(7));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(3) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIVSEL | GCLK_GENCTRL_RUNSTDBY |
                         GCLK_GENCTRL_SRC_OSCULP32K);
    while (GCLK->STATUS.bit.SYNCBUSY);

    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(3) | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_WDT_Val));
    while (GCLK->STATUS.bit.SYNCBUSY);

    volatile uint8_t* wdt_status = (volatile uint8_t*) 0x40001007;
    volatile uint8_t* wdt_config = (volatile uint8_t*) 0x40001001;
    volatile uint8_t* wdt_ctrl = (volatile uint8_t*) 0x40001000;

    while (*wdt_status);
    /* Set up the WDT to reset after 4096 cycles (32 s), if not cleared. */
    *wdt_config = 0x09;
    while (*wdt_status);
    *wdt_ctrl = 0x02;
    while (*wdt_status);
}


int main(void)
{
    /* Set up the watchdog before anything else */
    wdt_setup();

    /* Initialize gpio pins */
    gpio_init(D1_PIN, GPIO_OUT);
    gpio_init(D2_PIN, GPIO_OUT);
    gpio_init(D3_PIN, GPIO_OUT);
    gpio_init(D5_PIN, GPIO_OUT);
    gpio_clear(D1_PIN);
    gpio_clear(D2_PIN);
    gpio_clear(D3_PIN);
    gpio_clear(D5_PIN);

    /* Run wpantund to interact with NCP */
    DEBUG("This a test for OpenThread NCP\n");    
    
    while (1) {
		//Sleep
        xtimer_usleep(BLINK_INTERVAL);

        /* Blinking MCU led */
        if (led_mcu_on) {
            gpio_clear(D3_PIN);
            led_mcu_on = false;
        } else {
            gpio_set(D3_PIN);
            led_mcu_on = true;
        }
    }
    return 0;
}
