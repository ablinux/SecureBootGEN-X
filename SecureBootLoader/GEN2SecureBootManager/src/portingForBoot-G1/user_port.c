/*
 * user_port.c
 *
 *  Created on: Sep 3, 2018
 *      Author: abhishek.pandey
 */

#include "user_port.h"

static comms_apis_t comms_api =
{
 .close  = uart_close,
 .open   = uart_open,
 .read   = uart_read,
 .write  = uart_write,
 .abort_transfer = uart_abort
};

static aes_dec_api_t aec_dec_api =
{
 .open_dec = aes_init,
 .close_dec = aes_free,
 .dec_data = aes_dec_data
};

static flah_apis_t flash_api =
{
 .close  = flash_close,
 .open   = flash_open,
 .read   = flash_read,
 .write  = flash_write,
 .erase  = flash_erase
};

static crc_apis_t crc_api =
{
 .crc_calculate = crcFast,
 .crc_check_data = crcFast,
 .crc_open = crcInit
};

bootloader_cfg_t bootloader_cfg =
{
 .comms_apis = &comms_api,
 .flash_apis = &flash_api,
 .crc_apis = & crc_api,
 .aes_dec_api = &aec_dec_api
};

