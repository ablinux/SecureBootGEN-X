/*
 * bootloader.h
 *
 *  Created on: Aug 28, 2018
 *      Author: abhishek.pandey
 */

#ifndef BOOTLOADER_G1_BOOTLOADER_G1_H_
#define BOOTLOADER_G1_BOOTLOADER_G1_H_

#include "stdint.h"
#include "boot_cfg.h"
#include "stdbool.h"
#include "string.h"

/* Downloader commands */
#define   NEW_APP               'A'
#define   GET_APP_INFO          'I'
#define   GET_LOADER_INFO       'G'
#define   DOWNLOADER_EN         'D'
#define   EXIT_BOOT             'E'
#define   GET_LAST_BOOT_STATUS  '\r'
#define   DOWNLOAD_IMAGE        'N'
#define   START_APPLICATION     'S'

/* ACKNOWLEDGEMENTS */
#define     ACK_DWN_IM_IN               'D'
#define     ACK_DWN_NOT_SUFFICIENT      'X'
#define     ACK_DWN_FILE_EMPTY          'Y'
#define     ACK_DWN_SIZE_OK             'S'
#define     ACK_DWN_CHUNK_SIZE_OK       'O'
#define     ACK_DWN_COMPLETE            'C'
#define     ACK_DWN_OK                  'A'
#define     ACK_DWN_RESEND              'R'
#define     ACK_DWN_TIMEOUT             'T'
#define     ACK_DWN_SYS_FAIL            'E'
#define     ACK_DWN_FAILD               'F'
#define     ACK_OK                      'k'

/* DATA */
#define BT_NO_WAIT          (0U)
#define BT_WAIT_FOREVER     (0xFFFFFFFFU)

/* Error return */
#define BT_SUCCESS          (0U)
#define BT_ERROR            (0x10U)
#define BT_TIME_OUT         (0x20U)
#define BT_APP_OK           (0x30U)
#define BT_APP_NOT_OK       (0x40U)
#define BT_DWN_ACK          (0x50U)
#define BT_DWN_NOACK        (0x60U)
/* flash api structure */

typedef struct flash_api_st
{
    uint32_t (* open)(void);
    uint32_t (* close)(void);
    uint32_t (* read)(uint32_t *p_address_offset, uint8_t *p_data_rd, uint32_t bytes);
    uint32_t (* write)(uint32_t *p_address_offset, uint8_t *p_data_wr, uint32_t bytes);
    uint32_t (* erase)(uint32_t *p_address_offset, uint32_t bytes_to_erase);
}flah_apis_t;

typedef struct aes_dec_api_st
{
    void (*open_dec)(void);
    void (*close_dec)(void);
    uint32_t (*dec_data)(uint32_t data_lenght, uint8_t *input_data, uint8_t *output_data);
}aes_dec_api_t;

typedef struct comms_api_st
{
    uint32_t (* open)(void);
    uint32_t (* close)(void);
    uint32_t (* read)(uint8_t *rd_buff,uint32_t count, uint32_t time_out);
    uint32_t (* write)(uint8_t *wr_buff,uint32_t count);
    uint32_t (* abort_transfer)();
}comms_apis_t;

typedef struct timer_api_st
{
    uint32_t (* open)(void);
    uint32_t (* close)(void);
    uint32_t (* start)(void);
    uint32_t (* stop)(void);
}timer_apis_t;

typedef struct crc_api_st
{
    void (*crc_open)(void);
    crc (* crc_check_data)(uint8_t *p_data, uint32_t bytes);
    crc (* crc_calculate)(uint8_t *p_data, uint32_t bytes);
}crc_apis_t;

typedef struct image_info_st
{
    uint32_t app_start_address;
    uint32_t  app_size;
    crc check_sum;
}image_info_t;

typedef struct bootloader_data_sector_st
{
    uint8_t bt_start_of_sector;
    uint8_t bt_last_boot_status;
    uint32_t current_app_offset;
    uint8_t total_image;
    image_info_t image_data_1;
    image_info_t image_data_2;
    crc check_sum;
}bootloader_sector_t;

typedef struct image_data_st
{
    uint8_t is_encrypted;
    uint32_t data_lenth;
    uint8_t actual_data[DWN_FRAG_SIZE];
    crc check_sum;
}image_data_t;

/* Bootloader data_structure */
typedef struct bootloader_cfg_st
{
    flah_apis_t         * flash_apis;
    comms_apis_t        * comms_apis;
    timer_apis_t        * timer_apis;
    crc_apis_t          * crc_apis;
    aes_dec_api_t       * aes_dec_api;
    uint8_t             * err_str;
    bootloader_sector_t   bootloader_sector;
}bootloader_cfg_t;



/* Main Bootloader API's */

void        bt_init_bootloaderG1               (bootloader_cfg_t *p_boot_cfg);

uint32_t    bt_app_start                       (bootloader_cfg_t *p_boot_cfg);

void        bt_update_boot_data_sector         (bootloader_cfg_t *p_boot_cfg);

void        bt_get_boot_data_sector            (bootloader_cfg_t *p_boot_cfg);

bool        bt_is_host_avalilable              (bootloader_cfg_t *p_boot_cfg);

void        bt_get_downloader_command          (bootloader_cfg_t *p_boot_cfg, uint8_t *ret_cmd);

void        bt_download_image                  (bootloader_cfg_t *p_boot_cfg, uint32_t time_out);

void        bt_downloader_commnad_handler      (bootloader_cfg_t *p_boot_cfg);

void        bt_start_sucide_time               (bootloader_cfg_t *p_boot_cfg);

void        bt_start_tick_tock                 (bootloader_cfg_t *p_boot_cfg);

void        bt_mcu_reset                       ();

uint32_t    bt_app_validation                  (bootloader_cfg_t *p_boot_cfg);

uint32_t    bt_validate_download               (bootloader_cfg_t *p_boot_cfg, image_data_t *p_image_data);

void        bt_stop_tick_tock                  (bootloader_cfg_t *p_boot_cfg);

void        bt_command_processer               (bootloader_cfg_t *p_boot_cfg);

uint32_t static inline __SEND_ACK (bootloader_cfg_t *p_boot_cfg, uint8_t ack_byte)
{
    uint32_t bt_err = p_boot_cfg->comms_apis->write(&ack_byte,1);
    if(bt_err)
    {
        return BT_ERROR;
    }
    return BT_SUCCESS;
}

#endif /* BOOTLOADER_G1_BOOTLOADER_G1_H_ */
