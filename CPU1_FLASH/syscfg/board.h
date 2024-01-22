/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// CLB_OUTPUTXBAR7 -> CLB_SCLK_1 Pinmux
//
//
// CLB_OUTPUTXBAR7 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR7 1
#define CLB_SCLK_1_CLBOUTPUTXBAR_GPIO 1
#define CLB_SCLK_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_1_CLB_OUTPUTXBAR7

//
// CLB_OUTPUTXBAR6 -> CLB_SPICLK_1 Pinmux
//
//
// CLB_OUTPUTXBAR6 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR6 4
#define CLB_SPICLK_1_CLBOUTPUTXBAR_GPIO 4
#define CLB_SPICLK_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_4_CLB_OUTPUTXBAR6

//
// CLB_OUTPUTXBAR8 -> CLB_SCLK_2 Pinmux
//
//
// CLB_OUTPUTXBAR8 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR8 6
#define CLB_SCLK_2_CLBOUTPUTXBAR_GPIO 6
#define CLB_SCLK_2_CLBOUTPUTXBAR_PIN_CONFIG GPIO_6_CLB_OUTPUTXBAR8

//
// CLB_OUTPUTXBAR5 -> CLB_SOUT_1 Pinmux
//
//
// CLB_OUTPUTXBAR5 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR5 8
#define CLB_SOUT_1_CLBOUTPUTXBAR_GPIO 8
#define CLB_SOUT_1_CLBOUTPUTXBAR_PIN_CONFIG GPIO_8_CLB_OUTPUTXBAR5

//
// EPWM1 -> myEPWM0 Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 30
#define myEPWM0_EPWMA_GPIO 30
#define myEPWM0_EPWMA_PIN_CONFIG GPIO_30_EPWM1_A
//
// EPWM1_B - GPIO Settings
//
#define GPIO_PIN_EPWM1_B 31
#define myEPWM0_EPWMB_GPIO 31
#define myEPWM0_EPWMB_PIN_CONFIG GPIO_31_EPWM1_B

//
// OUTPUTXBAR3 -> CLB_SOUT_2 Pinmux
//
//
// OUTPUTXBAR3 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR3 60
#define CLB_SOUT_2_OUTPUTXBAR_GPIO 60
#define CLB_SOUT_2_OUTPUTXBAR_PIN_CONFIG GPIO_60_OUTPUTXBAR3

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define CCSI1_RX_TILE_BASE CLB1_BASE
void CCSI1_RX_TILE_init();
#define CCSI1_TX_TILE_BASE CLB2_BASE
void CCSI1_TX_TILE_init();
#define CCSI2_RX_TILE_BASE CLB3_BASE
void CCSI2_RX_TILE_init();
#define CCSI2_TX_TILE_BASE CLB4_BASE
void CCSI2_TX_TILE_init();

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
#define myCLBINPUTXBARINPUT0_SOURCE 17
#define myCLBINPUTXBARINPUT0_INPUT XBAR_INPUT1
void myCLBINPUTXBARINPUT0_init();
#define myCLBINPUTXBARINPUT1_SOURCE 4
#define myCLBINPUTXBARINPUT1_INPUT XBAR_INPUT2
void myCLBINPUTXBARINPUT1_init();
#define myCLBINPUTXBARINPUT2_SOURCE 61
#define myCLBINPUTXBARINPUT2_INPUT XBAR_INPUT3
void myCLBINPUTXBARINPUT2_init();
#define myCLBINPUTXBARINPUT3_SOURCE 4
#define myCLBINPUTXBARINPUT3_INPUT XBAR_INPUT4
void myCLBINPUTXBARINPUT3_init();

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_SCLK_1_init();
#define CLB_SCLK_1 XBAR_OUTPUT7
#define CLB_SCLK_1_ENABLED_MUXES (XBAR_MUX09)
void CLB_SPICLK_1_init();
#define CLB_SPICLK_1 XBAR_OUTPUT6
#define CLB_SPICLK_1_ENABLED_MUXES (XBAR_MUX08)
void CLB_SCLK_2_init();
#define CLB_SCLK_2 XBAR_OUTPUT8
#define CLB_SCLK_2_ENABLED_MUXES (XBAR_MUX25)
void CLB_SOUT_1_init();
#define CLB_SOUT_1 XBAR_OUTPUT5
#define CLB_SOUT_1_ENABLED_MUXES (XBAR_MUX12)

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void myCLBXBAR_TRANSFER_START_SYNC_init();
#define myCLBXBAR_TRANSFER_START_SYNC XBAR_AUXSIG0

#define myCLBXBAR_TRANSFER_START_SYNC_ENABLED_MUXES (XBAR_MUX07)

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define myEPWM0_BASE EPWM1_BASE
#define myEPWM0_TBPRD 0
#define myEPWM0_COUNTER_MODE EPWM_COUNTER_MODE_STOP_FREEZE
#define myEPWM0_TBPHS 0
#define myEPWM0_CMPA 0
#define myEPWM0_CMPB 0
#define myEPWM0_CMPC 0
#define myEPWM0_CMPD 0
#define myEPWM0_DBRED 0
#define myEPWM0_DBFED 0
#define myEPWM0_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM0_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM0_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_CCSI1_RX_TILE
#define INT_CCSI1_RX_TILE INT_CLB1
#define INT_CCSI1_RX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI1_RX_TILE_ISR(void);

// Interrupt Settings for INT_CCSI1_TX_TILE
#define INT_CCSI1_TX_TILE INT_CLB2
#define INT_CCSI1_TX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI1_TX_TILE_ISR(void);

// Interrupt Settings for INT_CCSI2_RX_TILE
#define INT_CCSI2_RX_TILE INT_CLB3
#define INT_CCSI2_RX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI2_RX_TILE_ISR(void);

// Interrupt Settings for INT_CCSI2_TX_TILE
#define INT_CCSI2_TX_TILE INT_CLB4
#define INT_CCSI2_TX_TILE_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_CCSI2_TX_TILE_ISR(void);

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_SOUT_2_init();
#define CLB_SOUT_2 XBAR_OUTPUT3
#define CLB_SOUT_2_ENABLED_MUXES (XBAR_MUX13)

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	CLB_INPUTXBAR_init();
void	CLB_OUTPUTXBAR_init();
void	CLBXBAR_init();
void	EPWM_init();
void	INTERRUPT_init();
void	OUTPUTXBAR_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
