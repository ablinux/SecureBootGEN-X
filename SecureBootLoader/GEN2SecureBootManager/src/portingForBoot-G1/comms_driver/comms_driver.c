/*
 * comms_driver.c
 *
 *  Created on: Aug 30, 2018
 *      Author: abhishek.pandey
 */


#include "comms_driver_api.h"

/* User may requires to create a wrapper function to meet the bootloader reqs */
volatile uart_event_t tx_complete = UART_EVENT_ERR_OVERFLOW;
volatile uart_event_t rx_complete = UART_EVENT_ERR_OVERFLOW;
/************************************ Comms API ***************************/
uint32_t uart_open ()
{
    ssp_err_t err = SSP_SUCCESS;

    /* Open driver */
    err =  g_uart0.p_api->open(g_uart0.p_ctrl,g_uart0.p_cfg);
    if(err)
        return BT_ERROR;

    /* set the baud rate */
    err = g_uart0.p_api->baudSet(g_uart0.p_ctrl,UART_BAUDRATE);
    if(err)
        return BT_ERROR;

    return 0;
}

uint32_t uart_abort ()
{
    uart_close();
    uart_open();
    return 0;
}

uint32_t uart_close ()
{
    ssp_err_t err = SSP_SUCCESS;

    /* Open driver */
    err =  g_uart0.p_api->close(g_uart0.p_ctrl);
    if(err)
        return BT_ERROR;
    return 0;
}

uint32_t uart_read (uint8_t *rd_buff, uint32_t count, uint32_t time_out)
{
    ssp_err_t err = SSP_SUCCESS;

    rx_complete = UART_EVENT_ERR_PARITY;

    err = g_uart0.p_api->read(g_uart0.p_ctrl,rd_buff,count);
    if(err)
    {
        return BT_ERROR;
    }
    while(!(rx_complete & UART_EVENT_RX_COMPLETE))
    {
        if(time_out == BT_WAIT_FOREVER)
        {
            /* DO NOT Time out if you decided to wait for ever */
            continue;
        }
        else
        {
            time_out --;
            if(time_out <= 0)
            {
                return BT_TIME_OUT;
            }
            /* 1 time out value == 10 ms */
            R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
        }
    }
    rx_complete = UART_EVENT_ERR_PARITY;
    return 0;
}

uint32_t uart_write (uint8_t *we_buff, uint32_t count)
{
    ssp_err_t err = SSP_SUCCESS;
    tx_complete = UART_EVENT_ERR_PARITY;
    err = g_uart0.p_api->write(g_uart0.p_ctrl,we_buff,count);
    if(err)
    {
        return BT_ERROR;
    }
    while(!(tx_complete & UART_EVENT_TX_COMPLETE));
    tx_complete = UART_EVENT_ERR_PARITY;
    return BT_SUCCESS;
}

void user_uart_callback(uart_callback_args_t *p_arg)
{
    tx_complete = p_arg->event;
    rx_complete = p_arg->event;
}

