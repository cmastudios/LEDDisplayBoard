#include "drv_ws2812b.h"
#include <sam.h>

static const uint32_t ledConfig[LED_COUNT] = {
    [LED_0] = PIN_PA08,
    [LED_1] = PIN_PA16,
    [LED_2] = PIN_PA12,
    [LED_3] = PIN_PA22,
    [LED_4] = PIN_PB12,
};

static inline void blit0(__IO  uint32_t * outset, __IO  uint32_t * outclr, uint32_t port)
{
    // Critical timing, depends on 16MHz CPU clock
	*outset = port; // 15 insts
	asm("nop\r\nnop\r\nnop\r\n");
	*outclr = port;
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
}

static inline void blit1(__IO  uint32_t * outset, __IO  uint32_t * outclr, uint32_t port)
{
    // this is specific to WS2812B V5 timing -- need to change for regular WS2812B
	*outset = port; // 10 insts
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
	*outclr = port; // 10 insts
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
}

void ws_init_pins(void)
{
    for (enum ws_led_strip led = (enum ws_led_strip)0U; led < LED_COUNT; ++led)
    {
        uint32_t pin = ledConfig[led];
        PORT_REGS->GROUP[pin / 32].PORT_DIRSET = 1 << (pin % 32);
    }
}

void ws_transmit(enum ws_led_strip led, const uint8_t *data, int length)
{
    uint32_t pin = ledConfig[led];
    __IO uint32_t * outset = &PORT_REGS->GROUP[pin / 32].PORT_OUTSET;
    __IO uint32_t * outclr = &PORT_REGS->GROUP[pin / 32].PORT_OUTCLR;
    uint32_t port = 1 << (pin % 32);
    
    __disable_irq();
	for (int i = 0; i < length; ++i)
	{
		uint8_t b = data[i];
        // heuristic: probably going to be outputting more 0's in general
		if (__builtin_expect(b & 0b10000000, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b01000000, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00100000, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00010000, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00001000, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00000100, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00000010, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (__builtin_expect(b & 0b00000001, 0)) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
	}
    __enable_irq();
}