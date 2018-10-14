/* HAL-only entry function */
#include "portingForBoot-G1/user_port.h"
#include "hal_data.h"
void bootloader_init();
uint8_t print_buff[100];
uint8_t read_buff[2];

void hal_entry(void)
{
    bt_init_bootloaderG1(&bootloader_cfg);
    /* TODO: add your own code here */
}
