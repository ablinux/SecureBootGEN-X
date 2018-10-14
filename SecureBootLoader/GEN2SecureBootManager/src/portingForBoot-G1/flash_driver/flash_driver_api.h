/*
 * flash_driver_api.h
 *
 *  Created on: Aug 30, 2018
 *      Author: abhishek.pandey
 */

#ifndef PORTINGFORBOOT_G1_FLASH_DRIVER_FLASH_DRIVER_API_H_
#define PORTINGFORBOOT_G1_FLASH_DRIVER_FLASH_DRIVER_API_H_

#include "bootloader-G1/bootloader-g1.h"
#include "hal_data.h"
extern bootloader_cfg_t boot_cfg;

uint32_t flash_open ();
uint32_t flash_close ();
uint32_t flash_read (uint32_t *p_address_offset, uint8_t *p_data, uint32_t bytes);
uint32_t flash_write (uint32_t *p_address_offset, uint8_t *p_data, uint32_t bytes);
uint32_t flash_erase (uint32_t *p_address_offset, uint32_t bytes);

#endif /* PORTINGFORBOOT_G1_FLASH_DRIVER_FLASH_DRIVER_API_H_ */
