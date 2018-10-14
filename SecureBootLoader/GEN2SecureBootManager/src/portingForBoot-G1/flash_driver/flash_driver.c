/*
 * flash_driver.c
 *
 *  Created on: Aug 30, 2018
 *      Author: abhishek.pandey
 */

#include "flash_driver_api.h"


volatile flash_event_t status = FLASH_EVENT_ERR_FAILURE;

/************************************ Flash API  ***************************/
uint32_t flash_open ()
{
#if !MCU_FLASH
    ssp_err_t err;
    err = g_qspi0.p_api->open(g_qspi0.p_ctrl,g_qspi0.p_cfg);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#else
    ssp_err_t err;
    err = g_flash0.p_api->open(g_flash0.p_ctrl,g_flash0.p_cfg);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#endif
}

uint32_t flash_info_get ()
{
#if !MCU_FLASH
    ssp_err_t err;
    err = g_qspi0.p_api->open(g_qspi0.p_ctrl,g_qspi0.p_cfg);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#else
    ssp_err_t err;
    err = g_flash0.p_api->open(g_flash0.p_ctrl,g_flash0.p_cfg);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#endif
}

uint32_t flash_close ()
{
#if !MCU_FLASH
    ssp_err_t err;
    err = g_qspi0.p_api->close(g_qspi0.p_ctrl);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#else
    ssp_err_t err;
    err = g_flash0.p_api->close(g_flash0.p_ctrl);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#endif
}

uint32_t flash_read (uint32_t *p_address_offset, uint8_t *p_data, uint32_t bytes)
{
#if !MCU_FLASH
    ssp_err_t err;
    bool in_progress = true;
    err = g_qspi0.p_api->read(g_qspi0.p_ctrl,(uint8_t *)p_address_offset,p_data,bytes);
    if(err)
    {
        return BT_ERROR;
    }
    do
    {
        err = g_qspi0.p_api->statusGet(g_qspi0.p_ctrl,&in_progress);
    }while(in_progress);
    return BT_SUCCESS;
#else
    ssp_err_t err;
    err = g_flash0.p_api->read(g_flash0.p_ctrl, p_data, (uint32_t)p_address_offset, bytes);
    if(err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
#endif
}

uint32_t flash_write (uint32_t *p_address_offset, uint8_t *p_data, uint32_t bytes)
{
#if !MCU_FLASH
    ssp_err_t err;
    bool in_progress = true;
    err = g_qspi0.p_api->pageProgram(g_qspi0.p_ctrl,(uint8_t *)p_address_offset,p_data,bytes);
    if(err)
    {
        return BT_ERROR;
    }
    do
    {
        err = g_qspi0.p_api->statusGet(g_qspi0.p_ctrl,&in_progress);
    }while(in_progress);

    return BT_SUCCESS;
#else
    ssp_err_t err;

    status = FLASH_EVENT_ERR_FAILURE;

    if(bytes < 256)
    {
        bytes = 256;
    }

    err = g_flash0.p_api->write(g_flash0.p_ctrl, (uint32_t)p_data, (uint32_t)p_address_offset, bytes);
    if(err)
    {
        return err;
    }
    while(SSP_ERR_IN_USE == g_flash0.p_api->statusGet(g_flash0.p_ctrl));

    return BT_SUCCESS;
#endif
}

uint32_t flash_erase (uint32_t *p_address_offset, uint32_t bytes)
{
#if !MCU_FLASH
    ssp_err_t err;
    bool in_progress = true;
    err = g_qspi0.p_api->erase(g_qspi0.p_ctrl,(uint8_t *)p_address_offset,bytes);
    if(err)
    {
        return BT_ERROR;
    }
    do
    {
        err = g_qspi0.p_api->statusGet(g_qspi0.p_ctrl,&in_progress);
    }while(in_progress);
    return BT_SUCCESS;
#else
    ssp_err_t err;
    uint32_t block_count = 0;
    flash_info_t flash_info;
    err= g_flash0.p_api->infoGet(g_flash0.p_ctrl, &flash_info); //TODO: Start from here
    if(err)
    {
        return BT_ERROR;
    }
    /* get the block count for erase*/
    block_count = (flash_info.code_flash.p_block_array->block_size / bytes);

    err = g_flash0.p_api->erase(g_flash0.p_ctrl, (uint32_t)p_address_offset, block_count);
    if(err)
    {
        return BT_ERROR;
    }
    while(SSP_ERR_IN_USE == g_flash0.p_api->statusGet(g_flash0.p_ctrl));
    return BT_SUCCESS;
#endif
}
/************************************ Flash API END ***************************/

#if MCU_FLASH
void flash_callback(flash_callback_args_t *p_args)
{
    status = p_args->event;
}
#endif
