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

#include "ot.h"
#ifndef SAMPLE_INTERVAL
#define SAMPLE_INTERVAL (1000000UL)
#endif
int main(void)
{
    /* Run wpantund to interact with NCP */
    /*while(1) {
        xtimer_usleep(SAMPLE_INTERVAL);
        LED0_TOGGLE;
    }*/
    return 0;
}
