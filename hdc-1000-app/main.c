#include <stdio.h>
#include <rtt_stdio.h>
#include "xtimer.h"
#include <string.h>
#include "net/gnrc/udp.h"
#include "phydat.h"
#include "saul_reg.h"
#include "periph/dmac.h"
#include "periph/i2c.h"
#include "periph/timer.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#ifndef SAMPLE_INTERVAL
#define SAMPLE_INTERVAL ( 1000000UL)
#endif

saul_reg_t *sensor_hum_t   = NULL;

void critical_error(void) {
    DEBUG("CRITICAL ERROR, REBOOT\n");
    NVIC_SystemReset();
    return;
}

void sensor_config(void) {
    sensor_hum_t = saul_reg_find_type(SAUL_SENSE_HUM);
	if (sensor_hum_t == NULL) {
		DEBUG("[ERROR] Failed to init HUM sensor\n");
		critical_error();
	} else {
		DEBUG("HUM sensor OK\n");
	}
}

/* ToDo: Sampling sequence arrangement or thread/interrupt based sensing may be better */
void sample(void) {
    phydat_t output; /* Sensor output data (maximum 3-dimension)*/
	int dim;         /* Demension of sensor output */

    /* Illumination 1-dim */
	dim = saul_reg_read(sensor_hum_t, &output);
    (void) dim;
    //phydat_dump(&output, dim);
}

int main(void) {
    dmac_init();
    i2c_set_dma_channel(0, 0);

    sensor_config();
	LED_OFF;

    while (1) {
		//Sample
	    sample();
		xtimer_usleep(SAMPLE_INTERVAL);
    }

    return 0;
}
