/**********************************************************************
 *
 * Filename:    main.c
 *
 * Description: A simple test program for the CRC implementations.
 *
 * Notes:       To test a different CRC standard, modify crc.h.
 *
 *
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include <windows.h>
#include <time.h>
#include "crc/crc.h"
#include "crypto/aes.h"
#if defined (PLATFORM_LINUX)
#define OUTPUT_RESET                        "\033[0m"
#define OUTPUT_RED                          "\033[31m"
#define OUTPUT_GREEN                        "\033[32m"
#define OUTPUT_YELLOW                       "\033[33m"
#else
#define OUTPUT_RESET                        ""
#define OUTPUT_RED                          ""
#define OUTPUT_GREEN                        ""
#define OUTPUT_YELLOW                       ""
#endif

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
#define     ACK_DWN_NO_SPACE            'Q'
#define     ACK_OK                      'k'

#define     MAX_DWN_FRAG_SIZE                (256U)
#define 	CHECK_SUM                    (sizeof(crc))/* 8bit crc */
DCB dcbSerialParams = { 0 }; // Initializing DCB structure

HANDLE hComm;
bool Status;

unsigned char cmd = 0;

DWORD dwEventMask;                     			// Event mask to trigger

DWORD bytes_read=0;

unsigned char lpBuffer;
DWORD bytes_to_write;         					// No of bytes to write into the port

typedef struct image_data_st
{
    uint8_t is_encrypted;
    uint32_t data_lenth;
    uint8_t actual_data[MAX_DWN_FRAG_SIZE];
    crc check_sum;
}image_data_t;

typedef struct image_info_st
{
    uint32_t app_start_address;
    uint8_t  app_size;
    uint32_t check_sum;
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

#define PACKET_SIZE   (sizeof(image_data_t))

unsigned long comms_read(unsigned char p_return_data[],unsigned long num_bytes);
unsigned long comms_write(unsigned char p_return_data[],unsigned long num_bytes,unsigned long *p_bytes_written);
static char const * GetLineTrailerByPercentage(uint8_t percentage);

int main(void)
{
	image_data_t data;
	bootloader_sector_t bootloader_data;
	DWORD bytes_written = 0;     // No of bytes written to the port
	unsigned char  SerialBuffer[300];               // Buffer Containing Rxed Data
	unsigned char  bt_status[10];
	FILE *fp;
	printf("\n +===============================================+");
	printf("\n |  DOWNLOADER Serial XCP interface (Win32 API)  |");
	printf("\n +===============================================+\n");
	char  ComPortName[] = "\\\\.\\COM";  // Name of the Serial port(May Change) to be opened,
	char com_num[5];
	/* ask user to enter com port number */
	printf("Enter the last digit of COM port : ");scanf("%s",&com_num[0]);

	/* add both the string */
	strcat(ComPortName,com_num);

	hComm = CreateFile(ComPortName,    	//port name
			GENERIC_READ | GENERIC_WRITE, 	//Read/Write
			0,                            	// No Sharing
			NULL,                         	// No Security
			OPEN_EXISTING,					// Open existing port only
			0,            					// Non Overlapped I/O
			NULL);        					// Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
	{
		printf("Error in opening serial port\n");
	}


	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

	DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

	if (Status == FALSE)
		printf("\n   Error! in GetCommState()");

	dcbSerialParams.BaudRate = 115200;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB

	if (Status == FALSE)
	{
		printf("\n   Error! in Setting DCB Structure");
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
	{
		printf("\n   Error! in Setting Time Outs");
	}
#define ENC 1
	crcInit();
	crc check_sum = 0;
	uint32_t chunk_size = 0;

#if ENC
	uint32_t databuff [256];
	mbedtls_aes_context g_ctx;
	mbedtls_aes_init(&g_ctx);
	uint8_t iv[16];
	uint8_t *key = (uint8_t *)"05189D3AF537763F2CF6C147937DD564";
	memset(iv,0x00,16);
#endif
	data.is_encrypted = ENC;
	/*-------------------- Get the command from user -----------------------*/
	while(1)
	{
		time_t t1,t2 = 0;
		/*Color Save current attributes */

		uint32_t size_for_trail = 0;
		uint32_t remaining_data = 0;
		uint8_t progressPct = 0;

		uint8_t retrys = 0;
		uint8_t send_again = 0;

		printf ("Enter the command : ");
		fflush(stdin);
		scanf("%c",&cmd);
		printf("\n");
		unsigned int ret = 0;
		switch(cmd)
		{
		case 'N': /// DOWNLOAD the IMAGE
			/* opne the file */
			Status = WriteFile(hComm,        // Handle to the Serial port
								&cmd,     			// Data to be written to the port
								1,  	//No of bytes to write
								&bytes_written, 	//Bytes written
								NULL);

			comms_read(bt_status,10);
			if(bt_status[0] == ACK_DWN_IM_IN)
			{
				puts("Starting DOWNLOAD ");
			}
			else
			{
				puts("DOWNLOAD EN ACK FAILD....\n");
				return 0;
			}
			fp = fopen("C:\\Users\\abhishek.pandey.TELXSI\\e2_studio\\c_and_cpp_learning\\Host_downloder\\src\\app_for_bootldr.bin","rb");
			if(fp == NULL)
			{
				printf("    Error in opening the file\n");
				return 0;
			}
			/* Get the size of file */
			fseek (fp , 0 , SEEK_END);
			long lSize = ftell (fp);

			Status = WriteFile(hComm,        // Handle to the Serial port
					           &lSize,     			// Data to be written to the port
					           4,  	//No of bytes to write
					           &bytes_written, 	//Bytes written
					           NULL);
			comms_read(bt_status,10);
			if(bt_status[0] == ACK_DWN_SIZE_OK) /* Size ACK */
			{
//				puts("File SIZE Confirms ---\n");
			}
			else if(bt_status[0] == ACK_DWN_SYS_FAIL)
			{
				puts("system failed ---\n");
				return 0;
			}
			else if(bt_status[0] == ACK_DWN_NOT_SUFFICIENT)
			{
				puts("File size can fit the flash memory\n");
				return 0;
			}
			else
			{
				puts("FILE SIZE ERROR..\n");
				return 0;
			}
			rewind (fp);
			t1 = clock();
			do
			{
				if(send_again == 0)
				{
#if ENC
					ret = fread(databuff,1,MAX_DWN_FRAG_SIZE,fp);
#else
					ret = fread(data.actual_data,1,MAX_DWN_FRAG_SIZE,fp);
#endif
				}
				size_for_trail = size_for_trail + ret;
				/* create a check_sum */
				data.data_lenth = ret;
#if ENC
				data.check_sum = crcFast(databuff,ret);
#else
				data.check_sum = crcFast(data.actual_data,ret);
#endif
				chunk_size = PACKET_SIZE;
#if ENC

				mbedtls_aes_setkey_enc(&g_ctx, key, 128);
				mbedtls_aes_crypt_cbc(&g_ctx, MBEDTLS_AES_ENCRYPT, 256, iv, databuff, data.actual_data);
#endif

				if(ret == 0)
				{
					chunk_size = 0;
				}

				/* Send the size first */
				Status = WriteFile(hComm,          //Handle to the Serial port
									&chunk_size,   //Data to be written to the port
									4,  			//No of bytes to write
									&bytes_written, //Bytes written
									NULL);
				if(ret == 0)
				{
					comms_read(bt_status,10);
					if (bt_status[0] == ACK_DWN_COMPLETE)
					{
						puts("Transfer complete\n");
					}
					break; /* NO MORE DATA to SEND */
				}
				/* wait for device ack */
				comms_read(bt_status,10);
				if(bt_status[0] == ACK_DWN_CHUNK_SIZE_OK)
				{
//					puts("Chunk Size Confirms ---\n");
				}
				else if (bt_status[0] == ACK_DWN_COMPLETE)
				{
					puts("Transfer complete B---\n");
					return 0;
				}
				else
				{
					printf("We got = %c\n",bt_status[0]);
					puts("Something else \n");
					return 0;
				}
				/*=============== Write a DATA to Bootloader ===================*/
				/* error condi */

				/* end of it */
				Status = WriteFile(hComm,            // Handle to the Serial port
								   &data,     // Data to be written to the port
								   chunk_size,  	 //No of bytes to write
						           &bytes_written, 	 //Bytes written
						           NULL);
				if(chunk_size != bytes_written)
				{
					puts("Error in download data transmission \n");
					return 0;
				}
				/* wait for device ack */
				comms_read(bt_status,10);
				if(bt_status[0] == ACK_DWN_OK)
				{
					send_again = 0;
//					puts("Sending more data ---\n");
				}
				else if(bt_status[0] == ACK_DWN_TIMEOUT)
				{
					puts("Session TIME OUT ---\n");
					return 0;
				}
				else if(bt_status[0] == ACK_DWN_SYS_FAIL)
				{
					puts("Enternal comms error  ---\n");
					return 0;
				}
				else if(bt_status[0] == ACK_DWN_RESEND)
				{
					retrys ++;
					puts("CRC check sum failed \n");
					if(retrys == 3)
					{
						puts("DOWNLOAD Failed \n");
						send_again = 0;
						break;
					}
					send_again = 1;
					continue;
				}
				else if(bt_status[0] == ACK_DWN_FAILD)
				{
					puts("DOWNLOAD Failed \n");
					break;
					//nothing
				}
				remaining_data = lSize - size_for_trail;
				progressPct = (uint8_t)(((lSize - remaining_data) * 100ul) / lSize);
				printf("DOWNLOAD PROGRESS : %s\r", GetLineTrailerByPercentage(progressPct)); (void)fflush(stdout);
			}while(ret != 0);
			t2 = clock() - t1;
			printf("Time taken to transfer %ld bytes is %ld ms\n",lSize,t2);
			break;

		case 'G':
			Status = WriteFile(hComm,        // Handle to the Serial port
					&cmd,     			// Data to be written to the port
					1,  	//No of bytes to write
					&bytes_written, 	//Bytes written
					NULL);
			comms_read((uint8_t*)&bootloader_data,sizeof(bootloader_sector_t));
			printf("Start of Sector marker : 0X%X\n",bootloader_data.bt_start_of_sector);
			printf("Last status of MCU boot : 0X%X\n",bootloader_data.bt_last_boot_status);
			printf("Current application offset address : 0X%X\n", bootloader_data.current_app_offset);
			printf("Total number of bin in system : %d\n",bootloader_data.total_image);
		}
		/* Write to serial port */
	}

	CloseHandle(hComm);//Closing the Serial Port

}   /* main() */

unsigned long comms_read(unsigned char p_return_data[],unsigned long num_bytes)
{
	Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (Status == FALSE)
	{
		printf("\n\n    Error! in Setting CommMask");
	}

	/* Read back the response */
	Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

	/*-------------------------- Program will Wait here till a Character is received ------------------------*/

	if (Status == FALSE)
	{
		printf("\n    Error! in Setting WaitCommEvent()");
		return 0;
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{

		Status = ReadFile(hComm, p_return_data, num_bytes, &bytes_read, NULL);
	}
	bytes_read = 0;
	return 0;
}
unsigned long comms_write(unsigned char *p_data,unsigned long num_bytes, unsigned long *p_bytes_written)
{
	Status = WriteFile(hComm,            // Handle to the Serial port
						p_data,     	 // Data to be written to the port
						num_bytes,  	 //No of bytes to write
						p_bytes_written, //Bytes written
						NULL);
	return 0;
}
static char const * GetLineTrailerByPercentage(uint8_t percentage)
{
  char const * result;
  /* Note that the following string was declared static to guarantee that the pointer
   * stays valid and can be used by the caller of this function.
   */
  static char trailerStrPct[32] = "";

  /* Construct the trailer. */
  sprintf(trailerStrPct, "[" OUTPUT_YELLOW "%3hhu%%" OUTPUT_RESET "]", percentage);
  /* Set the result. */
  result = &trailerStrPct[0];
  /* Give the result back to the caller. */
  return result;
} /*** end of GetLineTrailerByPercentage ***/
