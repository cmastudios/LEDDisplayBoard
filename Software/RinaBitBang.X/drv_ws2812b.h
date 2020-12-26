/* 
 * File:   drv_ws2812b.h
 * Author: connor
 *
 * Created on December 25, 2020, 4:33 PM
 */

#ifndef DRV_WS2812B_H
#define	DRV_WS2812B_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
enum ws_led_strip {
    LED_0,
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    
    LED_COUNT
};

void ws_init_pins(void);
void ws_transmit(enum ws_led_strip led, const uint8_t *data, int length);


#ifdef	__cplusplus
}
#endif

#endif	/* DRV_WS2812B_H */

