/*
 * serial.h
 *
 *  Created on: Sep 14, 2018
 *      Author: abhishek.pandey
 */

#ifndef SERIAL_SERIAL_H_
#define SERIAL_SERIAL_H_

#include <windows.h>
#include <stdint.h>
#include "stdbool.h"


uint32_t serial_open(uint8_t  *port_name);

uint32_t serial_read_setup();

uint32_t serial_write_setup();

uint32_t serial_read(uint8_t *data_holder, uint32_t bytes);

uint32_t serial_write(uint8_t *data_holder, uint32_t bytes);

uint32_t serial_close();


#endif /* SERIAL_SERIAL_H_ */
