#include "drv_nrf24l01.h"
#include <sam.h>

	// MOSI PB16, MISO PB17, SCK PA21
	// CE PA18, CSN PA20, IRQ PA19

#define CLOCK 16000000
// only valid for x up to CLOCK/2
#define SERIAL_BAUD_SYNC(x) (int) CLOCK/(2.0f*x) - 1


#define NRF_SERCOM_REGS SERCOM5_REGS
#define NRF_CTRLA \
    SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | \
    SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW | \
    SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE | \
    SERCOM_SPIM_CTRLA_FORM_SPI_FRAME | \
    SERCOM_SPIM_CTRLA_DIPO(1) | \
    SERCOM_SPIM_CTRLA_DOPO(3) | \
    SERCOM_SPIM_CTRLA_DORD_MSB
#define NRF_CTRLB \
    SERCOM_SPIM_CTRLB_CHSIZE_8_BIT | \
    SERCOM_SPIM_CTRLB_RXEN(1)
#define NRF_BAUD SERIAL_BAUD_SYNC(1000000)


#define NRF_CE_CSN_GROUP 0
#define NRF_CE_PORT PORT_PA18
#define NRF_CSN_PORT PORT_PA20
#define NRF_IRQ


void drv_nrf_init_ports(void)
{
    // set up MOSI/MISO/SCK
	PORT_REGS->GROUP[1].PORT_PMUX[16 / 2] = PORT_PMUX_PMUXE(MUX_PB16C_SERCOM5_PAD0) | // MOSI
                                            PORT_PMUX_PMUXO(MUX_PB17C_SERCOM5_PAD1);  // MISO
	PORT_REGS->GROUP[0].PORT_PMUX[21 / 2] = PORT_PMUX_PMUXO(MUX_PA21C_SERCOM5_PAD3);  // SCK
	PORT_REGS->GROUP[1].PORT_PINCFG[16] = PORT_PINCFG_PMUXEN(1);
	PORT_REGS->GROUP[1].PORT_PINCFG[17] = PORT_PINCFG_PMUXEN(1);
	PORT_REGS->GROUP[0].PORT_PINCFG[21] = PORT_PINCFG_PMUXEN(1);
	// set up CE/CSN
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_DIRSET = NRF_CE_PORT | NRF_CSN_PORT;
	// pull CSN high to disable SPI
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTSET = NRF_CSN_PORT;
	// pull CE low to disable transmitter/receiver
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTCLR = NRF_CE_PORT;
}

void drv_nrf_init_spi(void)
{
    // clocks
	MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_SERCOM5(1);
	GCLK_REGS->GCLK_PCHCTRL[25] = GCLK_PCHCTRL_CHEN(1) | GCLK_PCHCTRL_GEN_GCLK0;
	NVIC_EnableIRQ(SERCOM5_IRQn);
	// reset it
	NRF_SERCOM_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE(0);
	while (NRF_SERCOM_REGS->SPIM.SERCOM_SYNCBUSY) {}
	// set configuration
	NRF_SERCOM_REGS->SPIM.SERCOM_CTRLA = NRF_CTRLA;
	NRF_SERCOM_REGS->SPIM.SERCOM_CTRLB = NRF_CTRLB;
	NRF_SERCOM_REGS->SPIM.SERCOM_BAUD = NRF_BAUD;
	// enable
	NRF_SERCOM_REGS->SPIM.SERCOM_CTRLA |= SERCOM_SPIM_CTRLA_ENABLE(1);
	while (NRF_SERCOM_REGS->SPIM.SERCOM_SYNCBUSY) {}
}

// busy-wait spi transfer
static uint8_t spi_xfer(uint8_t out)
{
	// Wait until we can write
	while (!(NRF_SERCOM_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk)) {}
	// Write
	NRF_SERCOM_REGS->SPIM.SERCOM_DATA = out;
	// Wait until done receiving
	while (!(NRF_SERCOM_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_RXC_Msk)) {}
	// Read
	return NRF_SERCOM_REGS->SPIM.SERCOM_DATA;
}


uint8_t nrf_read_op(uint8_t command, int length, uint8_t *output)
{
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTCLR = NRF_CSN_PORT;
	uint8_t status = spi_xfer(command);
	for (int i = 0; i < length; ++i)
	{
		output[i] = spi_xfer(0xFF);
	}
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTSET = NRF_CSN_PORT;
    // CSN must go high for a certain minimum duration between commands
	asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\n");
	return status;
}

uint8_t nrf_write_op(uint8_t command, int length, const uint8_t *input)
{
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTCLR = NRF_CSN_PORT;
	uint8_t status = spi_xfer(command);
	for (int i = 0; i < length; ++i)
	{
		spi_xfer(input[i]);
	}
	PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTSET = NRF_CSN_PORT;
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

void nrf_enable_rxtx(void)
{
    PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTSET = NRF_CE_PORT;
}

void nrf_disable_rxtx(void)
{
    PORT_REGS->GROUP[NRF_CE_CSN_GROUP].PORT_OUTCLR = NRF_CE_PORT;
}