#pragma once
#include <stdint.h>
#include "sam.h"


// Reduce sample averaging. We don't need a lot of precision here, just need to discern 12 values.
#define DRV_ADC_SAMPLENUM	(ADC_AVGCTRL_SAMPLENUM_4 | ADC_AVGCTRL_ADJRES(2))

enum drv_adc_channel {
	DRV_ADC_CHANNEL_BAT,
	
	DRV_ADC_CHANNEL_COUNT
};

struct drv_adc_channelConfig {
	uint16_t adc_id:1;
	uint16_t mux:5;
};

struct drv_adc_config {
	const struct drv_adc_channelConfig * channelConfig;
};
extern struct drv_adc_config drv_adc_config;

struct drv_adc_results {
	uint16_t results[DRV_ADC_CHANNEL_COUNT];
	int error;
};

void drv_adc_init(void);
uint16_t drv_adc_read(int channel); // DOSN"T WORK IF A SEQ IS CONFIGURED
void drv_adc_read_sequence_sync(struct drv_adc_results * results);
