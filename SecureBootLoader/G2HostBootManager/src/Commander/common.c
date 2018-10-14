/*
 * common.c
 *
 *  Created on: Sep 14, 2018
 *      Author: abhishek.pandey
 */

#include "common.h"
#include "stdio.h"

static FILE *fp;


void welcome()
{
	printf("\n +===============================================+");
	printf("\n |  DOWNLOADER Serial XCP interface (Win32 API)  |");
	printf("\n +===============================================+\n");
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
void show_menu()
{
	puts("1. Download New bin to system\n"
	     "2. Get Target system information\n"
		 "3. Press enter to inn to the target\n\r");
}

uint32_t command_handler(uint8_t command)
{
	switch(command)
	{
	case '1':
	{
		uint8_t file_location_str[300];
		uint32_t file_size;
		uint32_t read_size;
		uint8_t data_holder;
		uint32_t temp_buffer[MAX_DWN_FRAG_SIZE];
		fflush(stdin);
		/* We are in downloader */
		puts("Enter the location of bin file : ");gets(file_location_str);
		/* open the file */
		fp = fopen(file_location_str,"rb"); if(fp == NULL){puts("Error in file open \n");exit(0);}
		/* Get the size of file */
		fseek (fp , 0 , SEEK_END);  file_size = ftell (fp); rewind (fp);
		printf("File Size : %d\n",file_size);

		/* Send Downloader a command to enable */
		send_command(DOWNLOADER_EN);
		/* wait for ACK */
		serial_read(&data_holder, 1);
		notify_user(data_holder);

		/** Send the file size */
		serial_write(&file_size, sizeof(file_size));
		serial_read(&data_holder, 1);
		notify_user(data_holder);

		/** Read the file and send chunk size*/
		read_size = fread(temp_buffer,1,256,fp);
		serial_write(&read_size, sizeof(read_size));
		serial_read(&data_holder, 1);
		notify_user(data_holder);

	}
	}
}
static void send_command(uint8_t cmd)
{
	uint8_t cmd_to_send = cmd;
	serial_write(&cmd_to_send,1);

}
static uint8_t notify_user(uint8_t status)
{
	switch(status)
	{
	case ACK_DWN_IM_IN :
		break;
	case ACK_DWN_NOT_SUFFICIENT :
		puts("Code cant fit into the flash \n");
		exit(0);
		break;
	case ACK_DWN_FILE_EMPTY     :
		puts("File is empty\n");
		exit(0);
		break;
	case ACK_DWN_SIZE_OK        :
		break;
	case ACK_DWN_CHUNK_SIZE_OK  :
		break;
	case ACK_DWN_COMPLETE       :
		break;
	case ACK_DWN_OK             :
		break;
	case ACK_DWN_RESEND         :
		puts("Error in data transfer... retrying\n");
		return ACK_DWN_RESEND;
		break;
	case ACK_DWN_TIMEOUT        :
		puts("Time out\n");
		exit(0);
		break;
	case ACK_DWN_SYS_FAIL       :
		puts("System failed\n");
		exit(0);
		break;
	case ACK_DWN_FAILD          :
		puts("Download failed\n");
		exit(0);
		break;
	case ACK_DWN_NO_SPACE       :
		puts("No more space\n");
		exit(0);
		break;
	case ACK_OK                 :
		break;
	}
	return 0;
}
