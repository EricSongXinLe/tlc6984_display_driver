/*
 * CCSI_Socket.h
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

#ifndef CCSI_SOCKET_H_
#define CCSI_SOCKET_H_

#include "driverlib.h"

void CCSI_write(uint16_t headBytes, uint16_t *dataBytes, uint16_t length, unsigned int checkResponse, unsigned int bus);
void CCSI_read(uint16_t headBytes, unsigned int bus);
void CCSI_write_sync(uint16_t headBytes, uint16_t *dataBytes, uint16_t length, unsigned int bus);
void processReadData(uint16_t headBytes, uint16_t length, unsigned int compareCheckBit, unsigned int bus);

#endif /* CCSI_SOCKET_H_ */
