/*
 ============================================================================
 Name        : G2HostBootManager.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Commander/common.h"
#include "drivers/serial/serial.h"
#include "drivers/crc/crc.h"
/* user includes */

int main(void)
{
	char com_num[5];
	uint8_t command;
	uint32_t err;
	/* init the low level system */
	crcInit();
	welcome();
	/* ask user to enter com port number */
	printf("Enter the last digit of COM port : ");scanf("%s",com_num);
	err = serial_open(com_num); if(err) {return 0;}
	show_menu();

	while(1)
	{
		command = getchar();
		command_handler(command);
	}

	return EXIT_SUCCESS;
}
