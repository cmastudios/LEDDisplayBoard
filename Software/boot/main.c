/* 
 * File:   main.c
 * Author: Connor
 *
 * Created on December 21, 2020, 6:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sam.h>
#include "drv_nrf24l01.h"
#include "drv_ws2812b.h"
#include "images.h"

static inline void delay_cycles(const uint32_t n)
{
	if (n > 0)
	{
		SysTick->LOAD = n;
		SysTick->VAL = 0;

		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		{
			//asm("wfi\r\n");
		};
	}
}

#define CLOCK 16000000
#define delay_us(x) delay_cycles(CLOCK/1000000*(x))
#define delay_ms(x) delay_cycles(CLOCK/1000*(x))



static uint8_t spi_res = 0;
static uint8_t rx_data[32] = {0};
static union image temp_image = { .raw = { [0 ... 659] = 3} };

enum display_target
{
	FACE_DEFAULT,
	FACE_HAPPY,
	FACE_LOVE,
	FACE_SCARED,
	FACE_SURPRISED,
	FACE_TIRED,
	FACE_UWU,
	FACE_WINK,
	ERROR_BATTERY,
	ERROR_WIRELESS,

	DISPLAY_FLASH_COUNT,
    
    FACE_RAM_0 = DISPLAY_FLASH_COUNT,
    FACE_RAM_1,
    FACE_RAM_2,
    FACE_RAM_3,
    FACE_RAM_4,
    FACE_RAM_5,
    FACE_RAM_6,
    FACE_RAM_7,
    DISPLAY_COUNT,

    DISPLAY_RAM_COUNT = DISPLAY_COUNT - FACE_RAM_0
} displayMode = FACE_DEFAULT;

static const union image * displayConfig[DISPLAY_FLASH_COUNT] = {
	[FACE_DEFAULT] = &image_face,
	[FACE_HAPPY] = &image_happy,
	[FACE_LOVE] = &image_love,
	[FACE_SCARED] = &image_scared,
	[FACE_SURPRISED] = &image_surprised,
	[FACE_TIRED] = &image_tired,
	[FACE_UWU] = &image_uwu,
	[FACE_WINK] = &image_wink,
	[ERROR_BATTERY] = &image_lowbattery,
	[ERROR_WIRELESS] = &image_wirelesserror,
};

union ram_face
{
    struct
    {
        uint8_t eyes_raw[20];
        uint8_t mouth_raw[8];
    };
    uint8_t raw[28];
};
static union ram_face ram_face[DISPLAY_RAM_COUNT] = {0};

// takes at least 7ms to return

static void display_image(const union image *image)
{
	if (image != NULL)
	{
		delay_us(350); // probably could omit this cuz each strip takes 1.2ms to complete
		ws_transmit(LED_0, image->eyes, 40 * 3);
		ws_transmit(LED_1, image->eyes + 40 * 3, 40 * 3);
		ws_transmit(LED_2, image->eyes + 40 * 3 * 2, 40 * 3);
		ws_transmit(LED_3, image->eyes + 40 * 3 * 3, 40 * 3);
		ws_transmit(LED_4, image->mouth, 60 * 3);
	}
}

static void decompress_ram_face(const union ram_face * ram_face, union image * output)
{
    for (uint32_t pixel_i = 0; pixel_i < 220; ++pixel_i)
    {
        const uint8_t raw_byte = ram_face->raw[pixel_i / 8U];
        const bool is_active = (raw_byte & (1U << (pixel_i % 8U))) != 0U;
        if (is_active)
        {
            output->raw[pixel_i * 3 + 0] = 255U/10;
            output->raw[pixel_i * 3 + 1] = 78U/10;
            output->raw[pixel_i * 3 + 2] = 158U/10;
        }
        else
        {
            output->raw[pixel_i * 3 + 0] = 0U;
            output->raw[pixel_i * 3 + 1] = 0U;
            output->raw[pixel_i * 3 + 2] = 0U;
        }
    }
}

static void display(enum display_target target)
{
    if (target < DISPLAY_FLASH_COUNT)
    {
        display_image(displayConfig[target]);
    }
    else if (target < DISPLAY_COUNT)
    {
        const uint32_t ram_i = (uint32_t)target - FACE_RAM_0;
        decompress_ram_face(&ram_face[ram_i], &temp_image);
        display_image(&temp_image);
    }
    else
    {
        // don't change current dislpay
    }
}

/*
 * 
 */
int main(void)
{
	// set CPU to 16MHz
	//NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_MANW(1) | NVMCTRL_CTRLB_RWS(1);
	OSCCTRL_REGS->OSCCTRL_OSC48MDIV = OSCCTRL_OSC48MDIV_DIV_DIV3;
	while (OSCCTRL_REGS->OSCCTRL_OSC48MSYNCBUSY)
	{
	}
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;


	drv_nrf_init_ports();
	drv_nrf_init_spi();

	// wait a second in case our code is bad, hopefully enough time to get into the debugger
	delay_ms(1000);

	ws_init_pins();

	spi_res = nrf_read_op(NRF_CMD_NOP, 0, NULL);
	// by default, radio channel is 2.402GHz
	spi_res = nrf_write_reg(NRF_REG_RF_CH, 76); //actually 2.476? should change to <2450 so we're part97
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
		displayMode = ERROR_WIRELESS;

	// set up receive pipe for control
	spi_res = nrf_write_op(NRF_CMD_W_REGISTER | NRF_REG_RX_ADDR_P0, 5, (const uint8_t *)"Rina1");
	spi_res = nrf_write_reg(NRF_REG_RX_PW_P0, 32);

	// enable receiver
	nrf_enable_rxtx();

	// initial display draw
	display_image(displayConfig[displayMode]);

	while (1)
	{

		// check for message
		spi_res = nrf_read_op(NRF_CMD_NOP, 0, NULL);
		if (spi_res & NRF_REG_STATUS_RX_DR)
		{
			uint8_t width;
			spi_res = nrf_read_op(NRF_CMD_R_RX_PL_WID, 1, &width);
			width = (width > 32) ? 32 : width;
			spi_res = nrf_read_op(NRF_CMD_R_RX_PAYLOAD, width, rx_data);

			const enum display_target prevDisplayMode = displayMode;

			
			if (rx_data[0] == 1)
			{
                // toggle visual
				if (rx_data[1] & 0x2)
				{
					// down
					if (displayMode > 0)
						displayMode--;
				}
				else if (rx_data[1] & 1)
				{
					// up
					if (displayMode < DISPLAY_COUNT - 1)
						displayMode++;
				}
				else
				{
					//cycle
					if (++displayMode >= DISPLAY_COUNT)
						displayMode = (enum display_target)0U;
				}
			}
            else if (rx_data[0] == 2)
            {
                // download image
                uint8_t target = rx_data[1];
                if (target < DISPLAY_RAM_COUNT)
                {
                    memcpy(ram_face[target].raw, &rx_data[2], 28);
                }
            }

			nrf_write_reg(NRF_REG_STATUS, NRF_REG_STATUS_RX_DR);

			// update the screen if needed
			if (prevDisplayMode != displayMode)
			{
                display(displayMode);
			}
		}
		delay_ms(10); // note display_image adds 7ms delay when its called. should replace with systick/rtos
	}
}
