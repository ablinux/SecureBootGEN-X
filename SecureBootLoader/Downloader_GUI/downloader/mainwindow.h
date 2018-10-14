#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdint.h>
#include <QMainWindow>
#include <windows.h>
#include "crc.h"

/* Error return */
/* Error return */
#define BT_SUCCESS          (0U)
#define BT_ERROR            (0x10U)
#define BT_TIME_OUT         (0x20U)
#define BT_APP_OK           (0x30U)
#define BT_APP_NOT_OK       (0x40U)
#define BT_DWN_ACK          (0x50U)
#define BT_DWN_NOACK        (0x60U)

/* Command */
/* Downloader commands */
#define   NEW_APP               'A'
#define   GET_APP_INFO          'I'
#define   GET_LOADER_INFO       'G'
#define   DOWNLOADER_EN         'D'
#define   EXIT_BOOT             'E'
#define   GET_LAST_BOOT_STATUS  '\r'
#define   DOWNLOAD_IMAGE        'N'
#define   START_APPLICATION     'S'

/* Ack bit */
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

#define PACKET_SIZE   (sizeof(image_data_t))

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    FILE *fp;
    QString FileName;
    uint32_t FileSize;
    HANDLE hCom;
    bootloader_sector_t bootDataSector;
    image_data_t binPayload;
    bool system_connection;

private slots:

    void on_selecFile_clicked();

    void on_downloadProgressBar_valueChanged(int value);

    void on_SelectComPort_clicked();

    void get_ComPorts();

    void set_Comms();

    uint32_t read_SerialData(void* data,uint32_t bytes);

    void on_downloadImage_clicked();

    void on_SelectComPort_Disconnect_clicked();

    void Sendpreamble();

    uint8_t notify_user(uint8_t status);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
