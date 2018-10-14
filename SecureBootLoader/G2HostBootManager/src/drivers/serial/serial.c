/*
 * serial.c
 *
 *  Created on: Sep 14, 2018
 *      Author: abhishek.pandey
 */

#include "serial.h"

static HANDLE hComm;
static DCB dcbSerialParams = { 0 }; // Initializing DCB structure
static COMMTIMEOUTS timeouts = { 0 };
static DWORD dwEventMask;                     			// Event mask to trigger

static char  ComPortName[] = "\\\\.\\COM";  // Name of the Serial port(May Change) to be opened,

uint32_t serial_open(uint8_t  *port_name)
{
	bool Status;
	strcat(ComPortName,port_name);

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
		return 1;
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

	if (Status == FALSE)
	{
		printf("\n   Error! in GetCommState()");
		return 1;
	}

	dcbSerialParams.BaudRate = 115200;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB

	if (Status == FALSE)
	{
		printf("\n   Error! in Setting DCB Structure");
		return 1;
	}
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
	{
		printf("\n   Error! in Setting Time Outs");
		return 1;
	}
	return 0;
}

uint32_t serial_read(uint8_t *data_holder, uint32_t bytes)
{
	bool Status;
	Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	uint32_t bytes_read;

	if (Status == FALSE)
	{
		printf("\n\n    Error! in Setting CommMask");
		return 1;
	}

	/* Read back the response */
	Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
	if (Status == FALSE)
	{
		printf("\n\n    Error! in Geting mask");
		return 1;
	}

	/*-------------------------- Program will Wait here till a Character is received ------------------------*/

	if (Status == FALSE)
	{
		printf("\n    Error! in Setting WaitCommEvent()");
		return 1;
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{

		Status = ReadFile(hComm, data_holder, bytes, &bytes_read, NULL);
	}
	bytes_read = 0;
	return 0;
}

uint32_t serial_write(uint8_t *data_holder, uint32_t bytes)
{
	bool Status;
	uint32_t p_bytes_written;
	Status = WriteFile(hComm,            // Handle to the Serial port
			data_holder,     	 // Data to be written to the port
			bytes,  	 //No of bytes to write
			&p_bytes_written, //Bytes written
			NULL);
	if(Status == false)
	{
		puts("Write failed\n");
		return 1;
	}
	return 0;
}

uint32_t serial_close()
{
	return 0;
}
