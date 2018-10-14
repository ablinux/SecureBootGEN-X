/*
 * comms_driver_api.h
 *
 *  Created on: Aug 30, 2018
 *      Author: abhishek.pandey
 */

#ifndef PORTINGFORBOOT_G1_COMMS_DRIVER_COMMS_DRIVER_API_H_
#define PORTINGFORBOOT_G1_COMMS_DRIVER_COMMS_DRIVER_API_H_

#include "bootloader-G1/bootloader-g1.h"
#include "hal_data.h"
extern bootloader_cfg_t boot_cfg;

uint32_t uart_open ();
uint32_t uart_close ();
uint32_t uart_read (uint8_t *rd_buff, uint32_t count, uint32_t time_out);
uint32_t uart_write (uint8_t *rd_buff,uint32_t count);
uint32_t uart_abort ();


#endif /* PORTINGFORBOOT_G1_COMMS_DRIVER_COMMS_DRIVER_API_H_ */
