/*
 * bootloader_cfg.h
 *
 *  Created on: Aug 20, 2018
 *      Author: abhishek.pandey
 */

#define OK 1


#ifndef BOOTLOADER_CFG_H_
#define BOOTLOADER_CFG_H_
#include "hal_data.h"

#define key  "05189D3AF537763F2CF6C147937DD564"

#define CRC16
#define MCU_FLASH 1

#if defined(CRC_CCITT)

typedef unsigned short  crc;

#define CRC_NAME            "CRC-CCITT"
#define POLYNOMIAL          0x1021
#define INITIAL_REMAINDER   0xFFFF
#define FINAL_XOR_VALUE     0x0000
#define REFLECT_DATA        FALSE
#define REFLECT_REMAINDER   FALSE
#define CHECK_VALUE         0x29B1

#elif defined(CRC16)

typedef unsigned short  crc;

#define CRC_NAME            "CRC-16"
#define POLYNOMIAL          0x8005
#define INITIAL_REMAINDER   0x0000
#define FINAL_XOR_VALUE     0x0000
#define REFLECT_DATA        TRUE
#define REFLECT_REMAINDER   TRUE
#define CHECK_VALUE         0xBB3D

#elif defined(CRC32)

typedef unsigned long  crc;

#define CRC_NAME            "CRC-32"
#define POLYNOMIAL          0x04C11DB7
#define INITIAL_REMAINDER   0xFFFFFFFF
#define FINAL_XOR_VALUE     0xFFFFFFFF
#define REFLECT_DATA        TRUE
#define REFLECT_REMAINDER   TRUE
#define CHECK_VALUE         0xCBF43926

#else

#error "One of CRC_CCITT, CRC16, or CRC32 must be #define'd."

#endif

/* CRC check sum */
#ifndef CHECK_SUM
#define CHECK_SUM                                (sizeof(crc))/* 8bit crc */
#endif

#define FLASH_END_ADDRESS                        (0x0400000)

/* NV memory location for bootloader status */
#ifndef APPLICATION_OFFSET
#define APPLICATION_OFFSET                       (0x00080000)// (0x60000000+4096U)
#endif

#define FLASH_ERASE_SIZE                         (8192U)
#define FLASH_SIZE                               (FLASH_END_ADDRESS - APPLICATION_OFFSET)

/* NV memory location for bootloader status */
#ifndef BOOTLOADER_STATUS_OFFSET
#define BOOTLOADER_STATUS_OFFSET                 (APPLICATION_OFFSET - FLASH_ERASE_SIZE)
#endif

/* UART Baud rate */
#ifndef UART_BAUDRATE
#define UART_BAUDRATE                            (115200U)
#endif

/* UART Timeout */
#ifndef BOOTLOADER_TIME_OUT_MS
#define BOOTLOADER_TIME_OUT_MS                   (300U)
#endif

/* Download Timeout */
#ifndef IMAGE_DOWNLOAD_TIMEOUT_MS
#define IMAGE_DOWNLOAD_TIMEOUT_MS                (3000U)
#endif

/* Re-send attempts */
#ifndef DOWN_RETRIES
#define DOWN_RETRIES                             (3U)
#endif

/* Mininum Erase Size check sum */
#ifndef PAGE_SIZE
#define PAGE_SIZE                                (256U)
#endif

/* Chunk size to be download */
#ifndef DWN_FRAG_SIZE
#endif
#define DWN_FRAG_SIZE                            (256U)

/* tick tock timer value */
#ifndef TIMER_VALUE_IN_MILIS
#define TIMER_VALUE_IN_MILIS                     (3000U)
#endif

/* tick tock timer value */
#ifndef CPU_CLK_HZ
#define CPU_CLK_HZ                               (240000000U)
#endif


#endif /* BOOTLOADER_CFG_H_ */
