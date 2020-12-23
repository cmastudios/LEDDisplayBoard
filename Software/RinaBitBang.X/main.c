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
#define _LW LED_RGB(0,0,0)
#define _LR LED_RGB(20, 0, 0)

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


const uint8_t alert[] = "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\032\000\000\032\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000";

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

#define CLOCK 16000000
// only valid for x up to CLOCK/2
#define SERIAL_BAUD_SYNC(x) (int) CLOCK/(2.0f*x) - 1

uint8_t spi_xfer(uint8_t out);
uint8_t nrf_read_op(uint8_t command, int length, uint8_t *output);
uint8_t nrf_write_op(uint8_t command, int length, const uint8_t *input);
uint8_t nrf_read_reg(uint8_t reg);
uint8_t nrf_write_reg(uint8_t reg, uint8_t value);


enum nrf_command {
	NRF_CMD_R_REGISTER = 0b00000000,
	NRF_CMD_W_REGISTER = 0b00100000,
	NRF_CMD_R_RX_PAYLOAD = 0b01100001,
	NRF_CMD_W_TX_PAYLOAD = 0b10100000,
	NRF_CMD_FLUSH_TX = 0b11100001,
	NRF_CMD_FLUSH_RX = 0b11100010,
	NRF_CMD_REUSE_TX_PL = 0b11100011,
	NRF_CMD_R_RX_PL_WID = 0b01100000,
	NRF_CMD_W_ACK_PAYLOAD = 0b10101000,
	NRF_CMD_W_TX_PAYLOAD_NOACK = 0b10110000,
	NRF_CMD_NOP = 0b11111111,
};

enum nrf_register {
	NRF_REG_CONFIG = 0,
	NRF_REG_EN_AA,
	NRF_REG_EN_RXADDR,
	NRF_REG_SETUP_AW,
	NRF_REG_SETUP_RETR,
	NRF_REG_RF_CH,
	NRF_REG_RF_SETUP,
	NRF_REG_STATUS,
	NRF_REG_OBSERVE_TX,
	NRF_REG_RPD,
	NRF_REG_RX_ADDR_P0,
	NRF_REG_RX_ADDR_P1,
	NRF_REG_RX_ADDR_P2,
	NRF_REG_RX_ADDR_P3,
	NRF_REG_RX_ADDR_P4,
	NRF_REG_RX_ADDR_P5,
	NRF_REG_TX_ADDR,
	NRF_REG_RX_PW_P0,
	NRF_REG_RX_PW_P1,
	NRF_REG_RX_PW_P2,
	NRF_REG_RX_PW_P3,
	NRF_REG_RX_PW_P4,
	NRF_REG_RX_PW_P5,
	NRF_REG_FIFO_STATUS,
	NRF_REG_DYNPD = 0x1C,
	NRF_REG_FEATURE
};
#define NRF_REG_CONFIG_EN_CRC (1U << 3)
#define NRF_REG_CONFIG_CRCO_CRC16 (1U << 2)
#define NRF_REG_CONFIG_PWR_UP (1U << 1)
#define NRF_REG_CONFIG_PRIM_RX (1U << 0)

#define NRF_REG_STATUS_RX_DR (1U << 6)
#define NRF_REG_STATUS_TX_DS (1U << 5)
#define NRF_REG_STATUS_MAX_RT (1U << 4)


// busy-wait spi transfer
uint8_t spi_xfer(uint8_t out)
{
	// Wait until we can write
	while (!(SERCOM5_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk)) {}
	// Write
	SERCOM5_REGS->SPIM.SERCOM_DATA = out;
	// Wait until done receiving
	while (!(SERCOM5_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk)) {}
	// Read
	return SERCOM5_REGS->SPIM.SERCOM_DATA;
}


uint8_t nrf_read_op(uint8_t command, int length, uint8_t *output)
{
	PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA20;
	uint8_t status = spi_xfer(command);
	for (int i = 0; i < length; ++i)
	{
		output[i] = spi_xfer(0xFF);
	}
	PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA20;
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
	return status;
}

uint8_t nrf_write_op(uint8_t command, int length, const uint8_t *input)
{
	PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA20;
	uint8_t status = spi_xfer(command);
	for (int i = 0; i < length; ++i)
	{
		spi_xfer(input[i]);
	}
	PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA20;
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
	return status;
}

uint8_t nrf_read_reg(uint8_t reg)
{
	if (reg > 0b11111) return 0;
	
	uint8_t data[1] = {0};
	nrf_read_op(NRF_CMD_R_REGISTER | reg, 1, data);
	return data[0];
}

uint8_t nrf_write_reg(uint8_t reg, uint8_t value)
{
	if (reg > 0b11111) return 0;
	
	uint8_t data[] = {value};
	return nrf_write_op(NRF_CMD_W_REGISTER | reg, 1, data);
}

static uint8_t spi_res = 0;
static uint8_t rx_data[32] = {0};

enum display_target {
	FACE_1,
	CSE467,
	WASHU,
	BLANK,
	ALERT,
	
	DISPLAY_COUNT
} displayMode = FACE_1;

/*
 * 
 */
int main(int argc, char** argv)
{
	// set CPU to 16MHz
	//NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW(1) | NVMCTRL_CTRLB_RWS(1);
	OSCCTRL_REGS->OSCCTRL_OSC48MDIV = OSCCTRL_OSC48MDIV_DIV_DIV3;
    while (OSCCTRL_REGS->OSCCTRL_OSC48MSYNCBUSY) {}
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	
	// NRF24L01+ interface
	// MOSI PB16, MISO PB17, SCK PA21
	// CE PA18, CSN PA20, IRQ PA19
	// set up MOSI/MISO/SCK
	PORT_REGS->GROUP[1].PORT_PMUX[16 / 2] = PORT_PMUX_PMUXE(MUX_PB16C_SERCOM5_PAD0) | PORT_PMUX_PMUXO(MUX_PB17C_SERCOM5_PAD1);
	PORT_REGS->GROUP[0].PORT_PMUX[21 / 2] = PORT_PMUX_PMUXO(MUX_PA21C_SERCOM5_PAD3);
	PORT_REGS->GROUP[1].PORT_PINCFG[16] = PORT_PINCFG_PMUXEN(1);
	PORT_REGS->GROUP[1].PORT_PINCFG[17] = PORT_PINCFG_PMUXEN(1);
	PORT_REGS->GROUP[0].PORT_PINCFG[21] = PORT_PINCFG_PMUXEN(1);
	// set up CE/CSN
	PORT_REGS->GROUP[0].PORT_DIRSET = PORT_PA18 | PORT_PA20;
	// pull CSN high to disable SPI
	PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA20;
	// pull CE low to disable transmitter/receiver
	PORT_REGS->GROUP[0].PORT_OUTCLR = PORT_PA18;
	
	delay_cycles(8000000);
	
		// set up SPI periph
	// clocks
	MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_SERCOM5(1);
	GCLK_REGS->GCLK_PCHCTRL[25] = GCLK_PCHCTRL_CHEN(1) | GCLK_PCHCTRL_GEN_GCLK0;
	NVIC_EnableIRQ(SERCOM5_IRQn);
	// reset it
	SERCOM5_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(0);
	while (SERCOM5_REGS->SPIM.SERCOM_SYNCBUSY) {}
	// set configuration
	SERCOM5_REGS->SPIM.SERCOM_CTRLA =
		SERCOM_SPIM_CTRLA_MODE_SPI_MASTER |
		SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW | SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE |
		SERCOM_SPIM_CTRLA_FORM_SPI_FRAME |
		SERCOM_SPIM_CTRLA_DIPO(1) | SERCOM_SPIM_CTRLA_DOPO(3) |
		SERCOM_SPIM_CTRLA_DORD_MSB;

	SERCOM5_REGS->SPIM.SERCOM_CTRLB =
			SERCOM_SPIM_CTRLB_CHSIZE_8_BIT | SERCOM_SPIM_CTRLB_RXEN(1);

	SERCOM5_REGS->SPIM.SERCOM_BAUD = SERIAL_BAUD_SYNC(1000000);
	// enable
	SERCOM5_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE(1);
	while (SERCOM5_REGS->SPIM.SERCOM_SYNCBUSY) {}
	
	// wait a second in case our code is bad, hopefully enough time to get into the debugger
	delay_cycles(16000000);
	
	// outputs for WS2812B-V5 control signals
	PORT_REGS->GROUP[0].PORT_DIRSET = PORT_PA08 | PORT_PA16 | PORT_PA12 | PORT_PA22;
	PORT_REGS->GROUP[1].PORT_DIRSET = PORT_PB12;


	spi_res = nrf_read_op(NRF_CMD_NOP, 0, NULL);
	// by default, radio channel is 2.402GHz
	spi_res = nrf_write_reg(NRF_REG_RF_CH, 76);//actually 2.476?
	// by default, auto-ack is enabled
	// by default, data pipe 0 and 1 is enabled
	// by default, auto-retransmit 3 times, 250us apart
	spi_res = nrf_write_reg(NRF_REG_SETUP_RETR, (0b0101 << 4) | (0b1111)); // delay 1500uS, 15 tries
	// by default, transmit at 2Mbps at 0dBm power
	spi_res = nrf_write_reg(NRF_REG_RF_SETUP, 0); // 1Mbps
	// reset current status
	spi_res = nrf_write_reg(NRF_REG_STATUS, NRF_REG_STATUS_RX_DR | NRF_REG_STATUS_TX_DS | NRF_REG_STATUS_MAX_RT);
	// flush buffers
	spi_res = nrf_read_op(NRF_CMD_FLUSH_TX, 0, NULL);
	spi_res = nrf_read_op(NRF_CMD_FLUSH_RX, 0, NULL);
	// turn the thing on, prime it for receiver mode
	spi_res = nrf_write_reg(NRF_REG_CONFIG, NRF_REG_CONFIG_EN_CRC | NRF_REG_CONFIG_CRCO_CRC16 | NRF_REG_CONFIG_PRIM_RX | NRF_REG_CONFIG_PWR_UP);
	// check if config worked
	spi_res = nrf_read_reg(NRF_REG_CONFIG);
	// set other visual if it failed
	if (spi_res != (NRF_REG_CONFIG_EN_CRC | NRF_REG_CONFIG_CRCO_CRC16 | NRF_REG_CONFIG_PRIM_RX | NRF_REG_CONFIG_PWR_UP))
		displayMode = CSE467;
	
	// set up receive pipe
	const char addr[] = "00001";
	spi_res = nrf_write_op(NRF_CMD_W_REGISTER | NRF_REG_RX_ADDR_P0, 5, (const uint8_t *)addr);
	spi_res = nrf_write_reg(NRF_REG_RX_PW_P0, 32);
	
	// enable receiver
	PORT_REGS->GROUP[0].PORT_OUTSET = PORT_PA18;
	
	while (1)
	{
		delay_cycles(5600);
		// check for message
		spi_res = nrf_read_op(NRF_CMD_NOP, 0, NULL);
		if (spi_res & NRF_REG_STATUS_RX_DR)
		{		
			uint8_t width;
			spi_res = nrf_read_op(NRF_CMD_R_RX_PL_WID, 1, &width);
			width = (width > 32) ? 32 : width;
			spi_res = nrf_read_op(NRF_CMD_R_RX_PAYLOAD, width, rx_data);
			
			// toggle visual
			if (rx_data[0] == 1)
			{
				if (rx_data[1] & 0x2) {
					// down
					if (displayMode > 0)
						displayMode--;
				}
				else if (rx_data[1] & 1)
				{
					// up
					if (displayMode < DISPLAY_COUNT-1)
						displayMode++;
				}
				else
				{
					//cycle
				if (++displayMode >= DISPLAY_COUNT)
					displayMode = (enum display_target)0U;
				}
			}
			
			nrf_write_reg(NRF_REG_STATUS, NRF_REG_STATUS_RX_DR);
		}
		// send 0, 0, 255
		const uint8_t *g1, *g2, *g3, *g4, *g5;
		switch (displayMode)
		{
		case FACE_1:
			g1 = blank2row;
			g2 = eye2row;
			g3 = eye2row;
			g4 = blank2row;
			g5 = mouth;
			break;
		case CSE467:
			g1 = class + 40*3*0;
			g2 = class + 40*3*1;
			g3 = class + 40*3*2;
			g4 = class + 40*3*3;
			g5 = blank3row;
			break;
		case WASHU:
			g1 = washu + 40*3*0;
			g2 = washu + 40*3*1;
			g3 = washu + 40*3*2;
			g4 = washu + 40*3*3;
			g5 = blank3row;
			break;
		case BLANK:
			g1 = blank2row;
			g2 = blank2row;
			g3 = blank2row;
			g4 = blank2row;
			g5 = blank3row;
			break;
		case ALERT:
		default:
			g1 = alert + 40*3*0;
			g2 = alert + 40*3*1;
			g3 = alert + 40*3*2;
			g4 = alert + 40*3*3;
			g5 = blank3row;
			break;
		}
#if 0
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