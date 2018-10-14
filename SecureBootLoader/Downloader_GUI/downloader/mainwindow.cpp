#include "mainwindow.h"
#include "crc.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <cstdio>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->downloadProgressBar->setValue(0);
    this->get_ComPorts();
    ui->comboBoxBaudrate->addItem("115200");
    ui->comboBoxBaudrate->addItem("57600");
    ui->comboBoxBaudrate->addItem("38400");
    ui->comboBoxBaudrate->addItem("19200");
    ui->comboBoxBaudrate->addItem("14400");
    ui->comboBoxBaudrate->addItem("9600");
    this->FileSize = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selecFile_clicked()
{
    /*trigger the file search */
    this->FileSize = 0;
    /* check if user has already placed the file in text field ?*/
    if(0 == ui->fileLocation->displayText().count())
    {
        this->FileName = QFileDialog::getOpenFileName(this,"Open .bin file",
                                                      "C:\\Users\\abhishek.pandey.TELXSI\\e2_studio\\Manual_test\\",
                                                      "*.bin");
    }
    else
    {
        this->FileName = ui->fileLocation->displayText();
    }
    /* Open the selected file */
    this->fp = fopen(this->FileName.toStdString().c_str(),"rb");
    if(this->fp == NULL)
    {
        QMessageBox::information(this,"Error","Error in opening the file");
        this->FileSize = 0;
    }
    else
    {
        fseek (this->fp , 0 , SEEK_END);
        this->FileSize = ftell (this->fp);
        rewind(this->fp);
        ui->downloadProgressBar->setValue(0);
        ui->fileLocation->setText(this->FileName);
        ui->fileSizeWR->setNum((double)this->FileSize);
    }
}


void MainWindow::on_downloadProgressBar_valueChanged(int value)
{
    ui->downloadProgressBar->setValue(value);
}

void MainWindow::on_SelectComPort_clicked()
{
    crcInit();
    /* Get the Serial port number from the combo box */
    QString ComPortName =  ui->comboBox->currentText();
    /* open the com port */
    DWORD last_err;
    char cmd[3] = {'I','I','I'};
    bool Status;
    uint8_t run=0;
    uint32_t bytes_written;
    QString bootStatus = "System OK";
    char comport [15]="\\\\.\\";
    strcat(comport,ComPortName.toStdString().c_str());
    this->hCom = CreateFileA(comport,    	//port name
                             GENERIC_READ | GENERIC_WRITE, 	//Read/Write
                             0,                            	// No Sharing
                             NULL,                         	// No Security
                             OPEN_EXISTING,					// Open existing port only
                             0,            					// Non Overlapped I/O
                             NULL);
    if(INVALID_HANDLE_VALUE == this->hCom)
    {
        last_err = GetLastError();
        if(last_err == ERROR_FILE_NOT_FOUND )
        {
            QMessageBox::information(this,"Error","Error in Opening "+ComPortName);
            CloseHandle(this->hCom);
        }
    }
    else
    {
        this->system_connection = true; /* mark the system as connected */
    }
    this->set_Comms();
    /* Send the device null command to ack that host is connected assuming application is running
        Send 2nd and 3rd time to to sure that device in bootloader mode */
    this->Sendpreamble();
    /* Now get the status of the boot device */
    cmd[0] = 'G';
    WriteFile(this->hCom, &cmd[0], 1,(LPDWORD)&bytes_written, NULL);

    this->read_SerialData(&this->bootDataSector,sizeof(bootloader_sector_t));

    if(this->bootDataSector.bt_last_boot_status == BT_APP_NOT_OK)
    {
        ui->SelectComPort->setStyleSheet("background-color: yellow");
        bootStatus = "System Failed to boot";
    }
    else if(this->bootDataSector.bt_last_boot_status == BT_APP_OK)
    {
        ui->SelectComPort->setStyleSheet("background-color: green");
        bootStatus = "System OK";
    }
    else
    {
        ui->SelectComPort->setStyleSheet("background-color: red");
        bootStatus = "No Bootloader";
    }
    ui->label_wr_lbs->setText(bootStatus);
    char tempBuff[100];
    sprintf(tempBuff,"0x%X",this->bootDataSector.current_app_offset);
    ui->label_wr_ofst->setText(tempBuff);
    sprintf(tempBuff,"%d",this->bootDataSector.total_image);
    ui->label_wr_apcount_2->setText(tempBuff);
}

void MainWindow ::get_ComPorts()
{
    HANDLE hcom;
    char  ComPortName[15] = "\\\\.\\COM";  // Name of the Serial port(May Change) to be opened,
    char port_num[5];
    uint8_t run = 0;
    DWORD last_err;
    ui->comboBox->addItem("Serial Port");
    while(run < 100)
    {
        memset(ComPortName,0x00,15);
        memcpy(ComPortName,"\\\\.\\COM",10);
        sprintf(port_num,"%d",run);
        strcat(ComPortName,port_num);
        /* Check only 100 com ports */

        hcom = CreateFileA(ComPortName,    	//port name
                           GENERIC_READ | GENERIC_WRITE, 	//Read/Write
                           0,                            	// No Sharing
                           NULL,                         	// No Security
                           OPEN_EXISTING,					// Open existing port only
                           0,            					// Non Overlapped I/O
                           NULL);
        if(INVALID_HANDLE_VALUE == hcom)
        {
            last_err = GetLastError();
            if(last_err == ERROR_FILE_NOT_FOUND )
            {

            }
        }
        else
        {
            /* add the value to the list */
            QString add_to_box = &ComPortName[4];
            ui->comboBox->addItem(add_to_box);
        }
        CloseHandle(hcom);
        run++;
    }
}
void MainWindow ::set_Comms()
{
    bool Status;
    DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(this->hCom, &dcbSerialParams);     //retreives  the current settings

    if (Status == FALSE)
        printf("\n   Error! in GetCommState()");

    dcbSerialParams.BaudRate = ui->comboBoxBaudrate->currentText().toUInt();//115200;      // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity   = NOPARITY;      // Setting Parity = None

    Status = SetCommState(this->hCom, &dcbSerialParams);  //Configuring the port according to settings in DCB

    if (Status == FALSE)
    {
        QMessageBox::information(this,"Error","Error in comPort setting ");
    }

    /*------------------------------------ Setting Timeouts --------------------------------------------------*/

    COMMTIMEOUTS timeouts = { 0 };

    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    Status = SetCommTimeouts(this->hCom, &timeouts);
    if (Status == FALSE)
    {
        QMessageBox::information(this,"Error","Error in comPort timeout setting ");
    }
}

uint32_t MainWindow ::read_SerialData(void *data,uint32_t bytes)
{
    DWORD dwEventMask;                     			// Event mask to trigger
    uint32_t bytes_read;
    bool Status = SetCommMask(this->hCom, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

    if (Status == FALSE)
    {
        return 1;
    }

    /* Read back the response */
    Status = WaitCommEvent(this->hCom, &dwEventMask, NULL); //Wait for the character to be received

    /*-------------------------- Program will Wait here till a Character is received ------------------------*/

    if (Status == FALSE)
    {
        return 1;
    }
    else
    {
        Status = ReadFile(this->hCom, data, bytes, (LPDWORD)&bytes_read, NULL);
    }
    return 0;
}

void MainWindow::on_downloadImage_clicked()
{
    if(this->system_connection == true)
    {
        if(this->FileSize != 0)
        {
            fflush(stdin);
            fflush(stdout);

            this->Sendpreamble();
            char cmd = DOWNLOAD_IMAGE;
            uint8_t read_back[10] = {0,0,0,0,0,0,0};
            uint32_t read_size,chunk_size,size_for_trail = 0,remaining_data = 0,bytes_written;
            uint8_t progressPct = 0,ack_status,send_again = 0;

            bool Status = WriteFile(this->hCom, &cmd, 1,(LPDWORD)&bytes_written, NULL);
            this->read_SerialData(read_back,1);
            ack_status = notify_user(read_back[0]);
            if(ack_status == 0) /* procced if everything is ok */
            {
                /* Send the file size to the bootloader */
                Status = WriteFile(this->hCom,  &this->FileSize,4, (LPDWORD)&bytes_written, NULL);
                this->read_SerialData(read_back,1);
                ack_status = notify_user(read_back[0]);
            }
            if(ack_status == 0)/* procced if everything is ok */
            {
                do
                {
                    /* Read the 1st chunk and send cunk size and image payload */
                    if(send_again == 0)
                    {
                        read_size = fread(this->binPayload.actual_data,1,256,this->fp);
                    }

                    chunk_size = PACKET_SIZE;

                    size_for_trail = size_for_trail + read_size;
                    this->binPayload.data_lenth = read_size;
                    this->binPayload.is_encrypted = 0;
                    this->binPayload.check_sum = crcFast(this->binPayload.actual_data,read_size);
                    if(read_size == 0)
                    {
                        chunk_size = 0;
                    }
                    /* send the chunk */
                    WriteFile(this->hCom, &chunk_size, 4,(LPDWORD)&bytes_written, NULL);
                    this->read_SerialData(read_back,1);
                    ack_status = notify_user(read_back[0]);

                    if(ack_status == ACK_DWN_COMPLETE) { break;} /* Download is complete */
                    else if(ack_status == 0)/* keep doing */
                    {
                        /* Send payload to bootloader */
                        WriteFile(this->hCom, &this->binPayload, PACKET_SIZE,(LPDWORD)&bytes_written, NULL);
                        this->read_SerialData(read_back,1);
                        ack_status = notify_user(read_back[0]);
                    }
                    if(ack_status == ACK_DWN_RESEND)
                    {
                        send_again = 1;
                    }
                    else if(ack_status == 1)
                    {
                        break;
                    }
                    else
                    {
                        remaining_data = this->FileSize - size_for_trail;
                        progressPct = (uint8_t)(((this->FileSize - remaining_data) * 100ul) / this->FileSize);
                        /* Inc the progress bar */
                        ui->downloadProgressBar->setValue(progressPct);
                    }
                }while(read_size != 0);
                rewind(this->fp);
            }
        }
        else
        {
            QMessageBox::information(this,"Error","Select the file first");
        }
    }
    else
    {
        QMessageBox::information(this,"Error","No devide is selected");
    }

}

uint8_t MainWindow :: notify_user(uint8_t status)
{
    uint8_t ack_status;
    switch(status)
    {
    case ACK_DWN_IM_IN :
        ui->downloadStatus->setText("Status : Downloading...");
        ack_status = 0;
        break;
    case ACK_DWN_NOT_SUFFICIENT :
        QMessageBox::information(this,"Error"," Size is not sufficent");
        ack_status = 1;
        break;
    case ACK_DWN_FILE_EMPTY     :
        QMessageBox::information(this,"Error"," File is Empty");
        ack_status = 1;
        break;
    case ACK_DWN_SIZE_OK        :
        ui->downloadStatus->setText("Status : Size confirmed.");
        ack_status = 0;
        break;
    case ACK_DWN_CHUNK_SIZE_OK  :
        ui->downloadStatus->setText("Status : Packet size confirmed.");
        ack_status = 0;
        break;
    case ACK_DWN_COMPLETE       :
        ui->downloadStatus->setText("Status : Download success.");
        QMessageBox::information(this,"Complete"," Download is complete"
                                                 " Starting app");
        return ACK_DWN_COMPLETE;
        ack_status = 0;
        break;
    case ACK_DWN_OK             :
        ui->downloadStatus->setText("Status : Downloading...");
        ack_status = 0;
        break;
    case ACK_DWN_RESEND         :
        ui->downloadStatus->setText("Status : CRC failed Resending data...");
        return ACK_DWN_RESEND;
        break;
    case ACK_DWN_TIMEOUT        :
        QMessageBox::information(this,"Error","Download Timeout");
        ack_status = 1;
        break;
    case ACK_DWN_SYS_FAIL       :
        QMessageBox::information(this,"Error","System failure reported reboot the"
                                              "system");
        ack_status = 1;
        break;
    case ACK_DWN_FAILD          :
        QMessageBox::information(this,"Error"," Download failed\n");
        ack_status = 1;
        break;
    case ACK_DWN_NO_SPACE       :
        ui->downloadStatus->setText("Status : No more space");
        QMessageBox::information(this,"Error"," No more space");
        ack_status = 1;
        break;
    case ACK_OK                 :
        ack_status = 0;
        ui->downloadStatus->setText("Status : ACK");
        break;
    default:
        return 'X'; // Something else
    }
    return ack_status;
}

void MainWindow::on_SelectComPort_Disconnect_clicked()
{
    if(this->system_connection == true)
    {
        CloseHandle(this->hCom);
        ui->SelectComPort->setStyleSheet("background-color: light gray");
        ui->label_wr_lbs->setText("None");
        ui->label_wr_ofst->setText("None");
        ui->label_wr_apcount_2->setText("None");
        ui->downloadProgressBar->reset();
        ui->fileSizeWR->setText("unknown");
        ui->downloadStatus->setText("Status : idle");
        ui->fileLocation->clear();
        this->FileSize = 0;
        fclose(this->fp);
    }
}

void MainWindow:: Sendpreamble()
{
    uint32_t run = 0;
    uint8_t cmd = 'I';// Preamble data //
    uint32_t bytes_written;
    bool Status;
    while(run < 4)
    {
        Status = WriteFile(this->hCom, &cmd, 1,(LPDWORD)&bytes_written, NULL);
        run ++;
        Sleep(40);
    }
}
