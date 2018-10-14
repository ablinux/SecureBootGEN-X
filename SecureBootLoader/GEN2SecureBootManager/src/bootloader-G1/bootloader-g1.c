/*
 * bootloader.c
 *
 *  Created on: Aug 28, 2018
 *      Author: abhishek.pandey
 */

#include "bootloader-g1.h"

void bt_init_bootloaderG1(bootloader_cfg_t *p_boot_cfg)
{
    uint32_t err;

    /* init the CRC */
    p_boot_cfg->crc_apis->crc_open();

    /* init the comms */
    err = p_boot_cfg->comms_apis->open();
    if(err)
    {
        bt_mcu_reset();
    }

    /* init the flash */
    err = p_boot_cfg->flash_apis->open();
    if(err)
    {
        bt_mcu_reset();
    }
    err = p_boot_cfg->flash_apis->read((uint32_t *)BOOTLOADER_STATUS_OFFSET,
                                 (uint8_t*)&p_boot_cfg->bootloader_sector,sizeof(bootloader_sector_t));
    if(err)
    {
        bt_mcu_reset();
    }
    if(p_boot_cfg->bootloader_sector.bt_last_boot_status == BT_APP_NOT_OK)
    {
        /* Start the command handler */
        bt_downloader_commnad_handler(p_boot_cfg);
    }

    /* Bootloder logic */
    if(true == bt_is_host_avalilable(p_boot_cfg)) // Check if host is connected to target
    {                                             // if check is positive before BT_TIME_OUT
        // Run the bootloader else verify and run user app
        bt_downloader_commnad_handler(p_boot_cfg);
    }
    else
    {
        bt_app_start(p_boot_cfg);               // this call involve check and run the user application
    }

}

uint32_t bt_app_start (bootloader_cfg_t *p_boot_cfg)
{
    uint32_t err = BT_SUCCESS;

    /* Validate the CRC of Installed APP */
    err = bt_app_validation(p_boot_cfg);
    if(err == BT_APP_NOT_OK)
    {
        /* and may be glow some led or buzzer*/

        /* Start the comms */
        p_boot_cfg->comms_apis->open();

        /* set the boot data flag in */
        p_boot_cfg->bootloader_sector.bt_start_of_sector = 0xAA;
        p_boot_cfg->bootloader_sector.bt_last_boot_status = BT_APP_NOT_OK;
        bt_update_boot_data_sector(p_boot_cfg);

        /* Start the handler even if user has not connected the serial yet and send the message to host after
         * connection that there was a problem with app
         */
        bt_downloader_commnad_handler(p_boot_cfg);
    }
    p_boot_cfg->bootloader_sector.bt_last_boot_status = BT_APP_OK;
    bt_update_boot_data_sector(p_boot_cfg);

    /* close everything */
    p_boot_cfg->aes_dec_api->close_dec();
    p_boot_cfg->comms_apis->close();
    p_boot_cfg->flash_apis->close();

    /* User application entry point prototype */
    void (*user_app_entry) (void);

    /* Set the VTOR to re-allocate */
    SCB->VTOR = APPLICATION_OFFSET & (uint32_t)0x1FFFFF80;

    /* Assign the address of reset vector */
    user_app_entry = (void(*)(void))(*((uint32_t *)(APPLICATION_OFFSET + 4)));

    /* call the user application */
    user_app_entry();

    return BT_SUCCESS;
}

void bt_downloader_commnad_handler(bootloader_cfg_t *p_boot_cfg)
{
    uint32_t err = BT_SUCCESS;
    uint32_t run = 1;
    uint8_t cmd_in[1] = {0};


    while(run)
    {
        err = p_boot_cfg->comms_apis->read(cmd_in,1,BT_WAIT_FOREVER);
        if(err == BT_ERROR)
        {
            bt_mcu_reset();
        }
        switch(cmd_in[0])
        {
            case DOWNLOADER_EN:

                break;
            case NEW_APP:

                break;
            case GET_APP_INFO:

                break;
            case GET_LOADER_INFO:
                bt_get_boot_data_sector(p_boot_cfg);
                break;
            case GET_LAST_BOOT_STATUS:
                bt_get_boot_data_sector(p_boot_cfg);
            break;
            case DOWNLOAD_IMAGE:
                bt_download_image(p_boot_cfg, IMAGE_DOWNLOAD_TIMEOUT_MS);
                break;
            default:
                p_boot_cfg->comms_apis->write((uint8_t*)"----WRONG CMD----\r\n",strlen("----WRONG CMD----\r\n"));
                break;
        }
    }
}

bool bt_is_host_avalilable(bootloader_cfg_t *p_boot_cfg)
{
    uint32_t err;
    uint8_t temp_buf[1];

    err = p_boot_cfg->comms_apis->read(temp_buf,1,BOOTLOADER_TIME_OUT_MS);
    if(err == BT_TIME_OUT)
    {
        p_boot_cfg->comms_apis->abort_transfer();
        return false;
    }
    else if(err == BT_ERROR) /* UART returns communication error hence do not proceed
            instead reset and retry */
    {   /* Generally this will never happen for production level*/
        bt_mcu_reset();
    }
    else
    {
        p_boot_cfg->comms_apis->abort_transfer();
        /*nothing */
    }
    return true;
}

void bt_update_boot_data_sector(bootloader_cfg_t *p_boot_cfg)
{
    p_boot_cfg->flash_apis->erase((uint32_t *)BOOTLOADER_STATUS_OFFSET,FLASH_ERASE_SIZE);
    p_boot_cfg->flash_apis->write((uint32_t *)BOOTLOADER_STATUS_OFFSET,
                                  (uint8_t *)&p_boot_cfg->bootloader_sector,sizeof(bootloader_sector_t));
}

void bt_get_boot_data_sector(bootloader_cfg_t *p_boot_cfg)
{
    p_boot_cfg->flash_apis->read((uint32_t *)BOOTLOADER_STATUS_OFFSET,
                                 (uint8_t *)&p_boot_cfg->bootloader_sector,sizeof(p_boot_cfg->bootloader_sector));

    p_boot_cfg->comms_apis->write((uint8_t*)&p_boot_cfg->bootloader_sector,sizeof(bootloader_sector_t));
}

void bt_download_image(bootloader_cfg_t *p_boot_cfg, uint32_t time_out)
{
    uint32_t err;

    uint8_t retry_count = DOWN_RETRIES;
    uint8_t temp_data[256];

    uint32_t app_size=0;
    uint32_t chunk_size = 0;

    image_data_t image_chunk;

    uint32_t sectors_to_erase = 0;
    uint32_t user_app_address = APPLICATION_OFFSET;

    __SEND_ACK(p_boot_cfg,ACK_DWN_IM_IN);

    /* Read the app_size of image */
    err = p_boot_cfg->comms_apis->read((uint8_t *)&app_size,4,time_out);
    if(err == BT_ERROR)
    {
        p_boot_cfg->comms_apis->abort_transfer();
        __SEND_ACK(p_boot_cfg, ACK_DWN_SYS_FAIL);
        return;
    }
    else if(err == BT_TIME_OUT)
    {
        p_boot_cfg->comms_apis->abort_transfer();
        __SEND_ACK(p_boot_cfg, ACK_DWN_TIMEOUT);
        return;
    }
    else
    {
        /* Nothing everything look fine */
    }
    if(app_size != 0)
    {
        /* check the flash availability */
        if(app_size > FLASH_SIZE)
        {
            p_boot_cfg->comms_apis->abort_transfer();
            __SEND_ACK(p_boot_cfg, ACK_DWN_NOT_SUFFICIENT);
            return;
        }
        /* calculate the number of sector need to erase */
        sectors_to_erase = app_size/FLASH_ERASE_SIZE;
        if(app_size % FLASH_ERASE_SIZE)
        {
            sectors_to_erase = sectors_to_erase + 1;
        }

        /* Erase the number of sector */
        while(sectors_to_erase)
        {
            p_boot_cfg->flash_apis->erase((uint32_t *)user_app_address,FLASH_ERASE_SIZE);
            user_app_address = user_app_address + FLASH_ERASE_SIZE;
            sectors_to_erase = sectors_to_erase - 1U;
        }
        /* Reset the user offset register */
        user_app_address = APPLICATION_OFFSET;

        __SEND_ACK(p_boot_cfg, ACK_DWN_SIZE_OK);
    }
    else
    {

    }

    while(1)
    {
        /* First read the app_size of chunk to download */
        err = p_boot_cfg->comms_apis->read((uint8_t *)&chunk_size,4,time_out);
        if(err == BT_ERROR)
        {
            p_boot_cfg->comms_apis->abort_transfer();
            __SEND_ACK(p_boot_cfg, ACK_DWN_SYS_FAIL);
            break;
        }
        else if(err == BT_TIME_OUT)
        {
            p_boot_cfg->comms_apis->abort_transfer();
            __SEND_ACK(p_boot_cfg, ACK_DWN_TIMEOUT);
            break;
        }

        if(chunk_size == 0)
        {

            /* Image download is completed Set the bootloader data sector with parameters */
            p_boot_cfg->bootloader_sector.current_app_offset = APPLICATION_OFFSET;
            p_boot_cfg->bootloader_sector.image_data_1.app_size = app_size;
            p_boot_cfg->bootloader_sector.image_data_1.app_start_address = APPLICATION_OFFSET;
            p_boot_cfg->bootloader_sector.total_image = 1;

            /* calculate the hole image check sum */
            p_boot_cfg->bootloader_sector.image_data_1.check_sum =
                    p_boot_cfg->crc_apis->crc_calculate((uint8_t *)p_boot_cfg->bootloader_sector.current_app_offset,
                                                        app_size);
            bt_update_boot_data_sector(p_boot_cfg);

            __SEND_ACK(p_boot_cfg, ACK_DWN_COMPLETE);

            bt_app_start(p_boot_cfg);

            break;
        }
        else
        {
            __SEND_ACK(p_boot_cfg, ACK_DWN_CHUNK_SIZE_OK);
        }

        /* use the app_size in read API to read the data */
        err = p_boot_cfg->comms_apis->read((uint8_t *)&image_chunk,chunk_size,time_out);
        if(err == BT_TIME_OUT)
        {
            p_boot_cfg->comms_apis->abort_transfer();
            __SEND_ACK(p_boot_cfg, ACK_DWN_TIMEOUT);
            break;
        }
        else if(err == BT_ERROR)
        {
            p_boot_cfg->comms_apis->abort_transfer();
            __SEND_ACK(p_boot_cfg, ACK_DWN_SYS_FAIL);
            break;
        }
        else
        {
            /* Check if image was encrypted ? */
            if(image_chunk.is_encrypted)
            {
                /* de-crypted the data */
                p_boot_cfg->aes_dec_api->dec_data(256,image_chunk.actual_data,temp_data);
                memcpy(image_chunk.actual_data,temp_data,256);
            }
            err = bt_validate_download(p_boot_cfg, &image_chunk);
            /* if CRC validation fails ack host to send the data again */
            if(err == BT_APP_NOT_OK)
            {
                /* Send the nack to HOST and ask to send again */
                retry_count --;
                __SEND_ACK(p_boot_cfg, ACK_DWN_RESEND);
                if(retry_count == 0)
                {
                    __SEND_ACK(p_boot_cfg, ACK_DWN_FAILD);
                    break;
                }
                continue;
                /* if NACK is sent multiple time exit the download and report to host */
            }
            else
            {
                /* Copy the image to Flash */
                p_boot_cfg->flash_apis->write((uint32_t *)user_app_address,image_chunk.actual_data,image_chunk.data_lenth);
                /* INC the address */
                user_app_address = user_app_address + image_chunk.data_lenth;
                /* and */
                /* ACK to host for next chunk */
                __SEND_ACK(p_boot_cfg, ACK_DWN_OK);
                retry_count = DOWN_RETRIES;
            }
        }
    }
}
void bt_start_tick_tock(bootloader_cfg_t *p_boot_cfg)
{
    uint32_t err;
    err = p_boot_cfg->timer_apis->start();
    if(err)
        while(1);
}

void bt_mcu_reset ()
{
    __NVIC_SystemReset();
}

uint32_t bt_app_validation (bootloader_cfg_t *p_boot_cfg)
{
    crc image_check_sum = p_boot_cfg->crc_apis->crc_calculate((uint8_t*)p_boot_cfg->bootloader_sector.current_app_offset,
                                                              p_boot_cfg->bootloader_sector.image_data_1.app_size);
    if(image_check_sum != p_boot_cfg->bootloader_sector.image_data_1.check_sum)
    {
        return BT_APP_NOT_OK;
    }
    return BT_APP_OK;

}

uint32_t bt_validate_download(bootloader_cfg_t *p_boot_cfg,image_data_t *p_image_data)
{
    /* validate the image data using crc */
    crc check_sum;
    check_sum =  p_boot_cfg->crc_apis->crc_check_data(p_image_data->actual_data,p_image_data->data_lenth);
    if(check_sum != p_image_data->check_sum)
    {
        return BT_APP_NOT_OK;
    }
    return BT_APP_OK;
}

void bt_stop_tick_tock(bootloader_cfg_t *p_boot_cfg)
{
    (void)(p_boot_cfg);
}

void bt_command_processer(bootloader_cfg_t *p_boot_cfg)
{
    (void)(p_boot_cfg);
}
