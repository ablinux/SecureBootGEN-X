/*
 * aec_driver.h
 *
 *  Created on: Sep 11, 2018
 *      Author: abhishek.pandey
 */

#ifndef PORTINGFORBOOT_G1_CRYPTO_AEC_DRIVER_H_
#define PORTINGFORBOOT_G1_CRYPTO_AEC_DRIVER_H_
#include "bootloader-G1/bootloader-g1.h"

void aes_init();
void aes_free();
uint32_t aes_dec_data(uint32_t data_lenght, uint8_t *input_data, uint8_t *output_data);

#endif /* PORTINGFORBOOT_G1_CRYPTO_AEC_DRIVER_H_ */
