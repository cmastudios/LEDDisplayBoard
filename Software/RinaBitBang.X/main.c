/* 
 * File:   main.c
 * Author: Connor
 *
 * Created on December 21, 2020, 6:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sam.h>

#define LED0_PORT PORT_PA12

static inline void delay_cycles(
		const uint32_t n)
{
	if (n > 0) {
		SysTick->LOAD = n;
		SysTick->VAL = 0;

		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
		};
	}
}

static inline void blit0(__IO  uint32_t * outset, __IO  uint32_t * outclr, uint32_t port)
{
	*outset = port; // 15 insts
	asm("nop\r\nnop\r\nnop\r\n");
	*outclr = port;
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
}
static inline void blit1(__IO  uint32_t * outset, __IO  uint32_t * outclr, uint32_t port)
{
	*outset = port; // 10 insts
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
	*outclr = port; // 10 insts
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
}


#define LED_RGB(r,g,b) g,r,b
#define LED_PINK4_DIM LED_RGB(255*0.1, 20*0.1, 147*0.1)
#define LED_BLANK LED_RGB(0,0,0)

const uint8_t pinkdata[60*3] = {LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM};

const uint8_t blank2row[40*3] = {LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,};
const uint8_t blank3row[60*3] = {LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,};
const uint8_t eye2row[40*3] = {LED_BLANK,LED_BLANK,LED_BLANK,  LED_PINK4_DIM, LED_PINK4_DIM,  LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,   LED_PINK4_DIM, LED_PINK4_DIM,     LED_BLANK,LED_BLANK,LED_BLANK,
								LED_BLANK,LED_BLANK,LED_BLANK,  LED_PINK4_DIM, LED_PINK4_DIM,  LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,   LED_PINK4_DIM, LED_PINK4_DIM,     LED_BLANK,LED_BLANK,LED_BLANK,};
const uint8_t mouth[60*3] = {
	LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,
	LED_BLANK,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_PINK4_DIM,LED_BLANK,
	LED_BLANK,LED_PINK4_DIM,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_PINK4_DIM, LED_BLANK,
	LED_BLANK,LED_BLANK,LED_PINK4_DIM,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_PINK4_DIM,LED_BLANK,LED_BLANK,
	LED_BLANK,LED_BLANK,LED_BLANK,LED_PINK4_DIM,LED_BLANK,LED_BLANK,LED_PINK4_DIM,LED_BLANK,LED_BLANK,LED_BLANK,
	LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,LED_PINK4_DIM,LED_PINK4_DIM,LED_BLANK,LED_BLANK,LED_BLANK,LED_BLANK,
};

const uint8_t class[] =   "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\032\000"
  "\000\032\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000"
  "\000\032\000\000\000\000\000\032\000\000\032\000\000\032\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000"
  "\000\000\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000"
  "\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000"
  "\000\032\000\000\000\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000"
  "\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000"
  "\000\000\032\000\000\000\000\000\032\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\032\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000"
  "\032\000\000\000\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000"
  "\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000"
  "\000\000\000\032\000\000";

const uint8_t washu[] =   "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000"
  "\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000"
  "\000\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000"
  "\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\000\000"
  "\000\032\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000"
  "\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000"
  "\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\032\000\000"
  "\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\032\000\000\000\000\000\000\000\000\032\000\000\000\000\000\032\000\000"
  "\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000\000\000\000"
  "\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\032\000\000\000\000\000\032\000\000\000\000\000\032\000\000\000"
  "\000\000\032\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000";


static void senddata(__IO  uint32_t * outset, __IO  uint32_t * outclr, uint32_t port, const uint8_t *data, int length)
{
	for (int i = 0; i < length; ++i)
	{
		uint8_t b = data[i];
		if (b & 0b10000000) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b01000000) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00100000) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00010000) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00001000) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00000100) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00000010) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
		if (b & 0b00000001) { blit1(outset, outclr, port); } else { blit0(outset, outclr, port); }
	}
}

/*
 * 
 */
int main(int argc, char** argv)
{
	//NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW(1) | NVMCTRL_CTRLB_RWS(1);
	OSCCTRL_REGS->OSCCTRL_OSC48MDIV = OSCCTRL_OSC48MDIV_DIV_DIV3;
    while (OSCCTRL_REGS->OSCCTRL_OSC48MSYNCBUSY) {}
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	delay_cycles(16000000);
	
	PORT_REGS->GROUP[0].PORT_DIRSET = PORT_PA08 | PORT_PA16 | PORT_PA12 | PORT_PA22;
	PORT_REGS->GROUP[1].PORT_DIRSET = PORT_PB12;
	
	while (1)
	{
		delay_cycles(5600);
		// send 0, 0, 255
#if 1
		const uint8_t *g1 = blank2row;
		const uint8_t *g2 = eye2row;
		const uint8_t *g3 = eye2row;
		const uint8_t *g4 = blank2row;
		const uint8_t *g5 = mouth;
#endif
#if 0
		const uint8_t *g1 = class;
		const uint8_t *g2 = class+40*3;
		const uint8_t *g3 = class+40*3*2;
		const uint8_t *g4 = class+40*3*3;
		const uint8_t *g5 = blank3row;
#endif
#if 0
		const uint8_t *g1 = washu;
		const uint8_t *g2 = washu+40*3;
		const uint8_t *g3 = washu+40*3*2;
		const uint8_t *g4 = washu+40*3*3;
		const uint8_t *g5 = blank3row;
#endif
		senddata(&PORT_REGS->GROUP[0].PORT_OUTSET, &PORT_REGS->GROUP[0].PORT_OUTCLR, PORT_PA08, g1, 40*3);
		senddata(&PORT_REGS->GROUP[0].PORT_OUTSET, &PORT_REGS->GROUP[0].PORT_OUTCLR, PORT_PA16, g2, 40*3);
		senddata(&PORT_REGS->GROUP[0].PORT_OUTSET, &PORT_REGS->GROUP[0].PORT_OUTCLR, PORT_PA12, g3, 40*3);
		senddata(&PORT_REGS->GROUP[0].PORT_OUTSET, &PORT_REGS->GROUP[0].PORT_OUTCLR, PORT_PA22, g4, 40*3);
		senddata(&PORT_REGS->GROUP[1].PORT_OUTSET, &PORT_REGS->GROUP[1].PORT_OUTCLR, PORT_PB12, g5, 60*3);
		delay_cycles(8000000);
	}
}



//4MHz version:

/*
 
 
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			
			
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			
			
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\n");

 
 */




/* 16mhz works great
 * 
 * 
 * 			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT; // 4 insts
//			
//			j = g & 1;
//			if (__builtin_expect(j, 0)) {
//				asm("nop\r\n");
//			} else {
//				asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
//			}
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT; // 15 insts
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			
			
			delay_cycles(50);
			
			
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			
			
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT; // 10 insts
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT; // 10 insts
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTSET = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
			PORT_REGS->GROUP[0].PORT_OUTCLR = LED0_PORT;
			asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");

 */