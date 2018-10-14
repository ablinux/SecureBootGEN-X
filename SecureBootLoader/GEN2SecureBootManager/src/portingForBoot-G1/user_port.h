/*
 * user_port.h
 *
 *  Created on: Sep 3, 2018
 *      Author: abhishek.pandey
 */

#ifndef USER_PORT_H_
#define USER_PORT_H_

/* Include all user driver header files */
#include "bootloader-G1/bootloader-g1.h"
#include "comms_driver/comms_driver_api.h"
#include "flash_driver/flash_driver_api.h"
#include "crc/crc.h"
#include "crypto/aec_driver.h"

extern bootloader_cfg_t bootloader_cfg;

#endif /* USER_PORT_H_ */
