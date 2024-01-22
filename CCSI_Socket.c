/*
 * CCSI_Socket.c
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

//
// Included Files
//
#include <CCSI_Socket.h>
#include <led_hal.h>
#include "driverlib.h"
#include "led_driver.h"
#include "system_info.h"

/* Function Name: CCSI_write
 *
 * Purpose: Write data bytes
 *
 * Parameters:
 * headBytes -> Head bytes of the command.
 * dataBytes -> Data bytes, MSB first.
 * length -> Length of data bytes.
 * checkResponse -> For CLB solution this is a dummy argument
 *
 */
void CCSI_write(uint16_t headBytes, uint16_t *dataBytes, uint16_t length, unsigned int checkResponse, unsigned int bus)
{
    uint32_t ledXmtFrameSize;
    uint32_t i = 0;

    // Wait for CLB to finish any ongoing transfer
    while(xmtDone[bus] == 0){}

    // Load data to CLB transfer array
    ledXmtBuffer[bus][0] = (uint32_t)headBytes;
    for(i = 0; i < length; i++)
    {
        ledXmtBuffer[bus][i + 1] = (uint32_t)dataBytes[i];
    }

    ledXmtFrameSize = i + 1;

    if(checkResponse)
    {
        // Trash any data in the RX CLB
        LED_HAL_ringBufferReset(bus);

        // Enable the RX CLB
        LED_HAL_enableClbReceiver(bus);
    }

    //
    // Start CLB transfer
    //
    LED_HAL_clbTransferNonBlocking(bus, &ledXmtBuffer[bus][0], ledXmtFrameSize, false);

    if(checkResponse)
    {
        processReadData(headBytes, ledXmtFrameSize, FALSE, bus);

        // Disable the receiver
        LED_HAL_disableClbReceiver(bus);
    }
}

void CCSI_write_sync(uint16_t headBytes, uint16_t *dataBytes, uint16_t length, unsigned int bus)
{
    uint32_t ledXmtFrameSize;
    uint32_t i = 0;

    // Wait for CLB to finish any ongoing transfer
    while(xmtDone[bus] == 0){}

    // Load data to CLB transfer array
    ledXmtBuffer[bus][0] = (uint32_t)headBytes;
    for(i = 0; i < length; i++)
    {
        ledXmtBuffer[bus][i + 1] = (uint32_t)dataBytes[i];
    }

    ledXmtFrameSize = i + 1;

    //
    // Prepare CLB for transfer, but do not start transfer yet
    //
    LED_HAL_clbTransferNonBlocking(bus, &ledXmtBuffer[bus][0], ledXmtFrameSize, true);
}


/* Function Name: processReadData
 *
 * Purpose: Process the RX data buffer from SPI
 *
 * Parameters:
 * length -> Length of data bytes requested.
 * compareCheckBit -> Check the buffer if CHECK bit was successfully received.
 *
 */
void processReadData(uint16_t headBytes, uint16_t length, unsigned int compareCheckBit, unsigned int bus)
{
    uint16_t rcvCount = 1;
    uint16_t checkByte;
    uint16_t data;
    uint16_t max_trials = 16 * MAX_DATA_LENGTH;

    if(headBytes == W_CHIP_INDEX) {
        checkByte = (W_CHIP_INDEX + CASCADED_UNITS[bus]);
    }
    else {
        checkByte = headBytes;
    }

    // Clear first index
    ledRcvBuffer[bus][0] = 0;

    // First wait until headByte is received - It might be that data from previous transmission is first received
    while((max_trials > 0) & (!(ledRcvBuffer[bus][0] == checkByte))) {
        if (LED_HAL_clbRxRingBuffRead(bus, &data) == 0) {
            ledRcvBuffer[bus][0] = data;
        }
        max_trials--;
    }

    // Wait for receive data bytes
    if(max_trials > 0) {
        while(rcvCount < length)
        {
            if (LED_HAL_clbRxRingBuffRead(bus, &ledRcvBuffer[bus][rcvCount]) == 0)
                rcvCount++;
        }
    }
}

/* Function Name: CCSI_read
 *
 * Purpose: Read data bytes
 *
 * Parameters:
 * headBytes -> Head bytes of the command.
 *
 */
void CCSI_read(uint16_t headBytes, unsigned int bus)
{
    uint16_t data[1] = {0x0000};

    // Wait for CLB to finish any ongoing transfer
    while(xmtDone[bus] == 0){}

    // Trash any data in the RX CLB
    LED_HAL_ringBufferReset(bus);

    // Enable the RX CLB
    LED_HAL_enableClbReceiver(bus);

    CCSI_write(headBytes, data, 0, FALSE, bus);

    processReadData(headBytes, (CASCADED_UNITS[bus] * 3) + 1, FALSE, bus);

    // Disable the receiver
    LED_HAL_disableClbReceiver(bus);
}
