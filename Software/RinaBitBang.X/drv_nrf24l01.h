/* 
 * File:   drv_nrf24l01.h
 * Author: connor
 *
 * Created on December 25, 2020, 3:56 PM
 */

#ifndef DRV_NRF24L01_H
#define	DRV_NRF24L01_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

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

void drv_nrf_init_ports(void);
void drv_nrf_init_spi(void);
uint8_t nrf_read_op(uint8_t command, int length, uint8_t *output);
uint8_t nrf_write_op(uint8_t command, int length, const uint8_t *input);
uint8_t nrf_read_reg(uint8_t reg);
uint8_t nrf_write_reg(uint8_t reg, uint8_t value);
void nrf_enable_rxtx(void);
void nrf_disable_rxtx(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_NRF24L01_H */

