#include <stdio.h>
#include <rtt_stdio.h>
#include "xtimer.h"
#include <string.h>
#include "periph/timer.h"

#define ENABLE_DEBUG    (0)
#include "debug.h"

#ifndef SAMPLE_INTERVAL
#define SAMPLE_INTERVAL ( 1000000UL)
#endif

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        LED0_TOGGLE;
		//Sleep
        xtimer_periodic_wakeup(&last_wakeup, SAMPLE_INTERVAL);
    }

    return 0;
}
