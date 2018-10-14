/*
 * aec_driver.c
 *
 *  Created on: Sep 11, 2018
 *      Author: abhishek.pandey
 */

#include "aes.h"
#include "aec_driver.h"

static mbedtls_aes_context g_ctx;
static uint8_t iv[16];
void aes_init()
{
    mbedtls_aes_init(&g_ctx);
    memset(iv,0x00,16);
}
void aes_free()
{
    mbedtls_aes_free(&g_ctx);
    memset(iv,0x00,16);
}

uint32_t aes_dec_data(uint32_t data_lenght, uint8_t *input_data, uint8_t *output_data)
{
    int err;

    uint8_t *private_key = (uint8_t*)"05189D3AF537763F2CF6C147937DD564"; // AES key;

    /* set the key for dec */
    err = mbedtls_aes_setkey_dec(&g_ctx, private_key, 128);
    if(err == MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH)
    {
        return BT_ERROR;
    }
    /* start the dec */
    err = mbedtls_aes_crypt_cbc(&g_ctx, MBEDTLS_AES_DECRYPT, data_lenght, iv, input_data, output_data);
    if(err == MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
}
