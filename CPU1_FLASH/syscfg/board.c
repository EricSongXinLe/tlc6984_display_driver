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

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	SYNC_init();
	CLB_init();
	CLB_INPUTXBAR_init();
	CLB_OUTPUTXBAR_init();
	CLBXBAR_init();
	EPWM_init();
	OUTPUTXBAR_init();
	INTERRUPT_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// CLB_OUTPUTXBAR7 -> CLB_SCLK_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SCLK_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR6 -> CLB_SPICLK_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SPICLK_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR8 -> CLB_SCLK_2 Pinmux
	//
	GPIO_setPinConfig(CLB_SCLK_2_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR5 -> CLB_SOUT_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SOUT_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// EPWM1 -> myEPWM0 Pinmux
	//
	GPIO_setPinConfig(myEPWM0_EPWMA_PIN_CONFIG);
	GPIO_setPadConfig(myEPWM0_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEPWM0_EPWMA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(myEPWM0_EPWMB_PIN_CONFIG);
	GPIO_setPadConfig(myEPWM0_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEPWM0_EPWMB_GPIO, GPIO_QUAL_SYNC);

	//
	// OUTPUTXBAR3 -> CLB_SOUT_2 Pinmux
	//
	GPIO_setPinConfig(CLB_SOUT_2_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	CCSI1_RX_TILE_init();
	CCSI1_TX_TILE_init();
	CCSI2_RX_TILE_init();
	CCSI2_TX_TILE_init();
}

void CCSI1_RX_TILE_init(){
	CLB_setOutputMask(CCSI1_RX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI1_RX_TILE_BASE);
	//
	// CCSI1_RX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI1_RX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI1_RX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI1_RX_TILE_BASE, 0);
	//
	// CCSI1_RX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_INPUT1);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN1);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN1);
	//
	// CCSI1_RX_TILE CLB_IN2 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_LOCAL_IN_MUX_INPUT2);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN2);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN2);
	//
	// CCSI1_RX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_GP_REG);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN3);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN3);
	CLB_setGPREG(CCSI1_RX_TILE_BASE,0);

	CLB_disableCLB(CCSI1_RX_TILE_BASE);
}
void CCSI1_TX_TILE_init(){
	CLB_setOutputMask(CCSI1_TX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI1_TX_TILE_BASE);
	//
	// CCSI1_TX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI1_TX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI1_TX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI1_TX_TILE_BASE, 0);
	//
	// CCSI1_TX_TILE CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN0);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN0);
	//
	// CCSI1_TX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN1);
	//
	// CCSI1_TX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN3);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN3);
	//
	// CCSI1_TX_TILE CLB_IN4 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_GLOBAL_IN_MUX_EPWM1B);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN4);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN4);
	//
	// CCSI1_TX_TILE CLB_IN5 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_GLOBAL_IN_MUX_CLB_AUXSIG0);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN5);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN5);
	//
	// CCSI1_TX_TILE CLB_IN6 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN6);
	CLB_setGPREG(CCSI1_TX_TILE_BASE,0);

	CLB_disableCLB(CCSI1_TX_TILE_BASE);
}
void CCSI2_RX_TILE_init(){
	CLB_setOutputMask(CCSI2_RX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI2_RX_TILE_BASE);
	//
	// CCSI2_RX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI2_RX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI2_RX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI2_RX_TILE_BASE, 0);
	//
	// CCSI2_RX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_RX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_INPUT3);
	CLB_configGlobalInputMux(CCSI2_RX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_RX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_RX_TILE_BASE, CLB_IN1);
	CLB_selectInputFilter(CCSI2_RX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_RX_TILE_BASE, CLB_IN1);
	//
	// CCSI2_RX_TILE CLB_IN2 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_RX_TILE_BASE, CLB_IN2, CLB_LOCAL_IN_MUX_INPUT4);
	CLB_configGlobalInputMux(CCSI2_RX_TILE_BASE, CLB_IN2, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_RX_TILE_BASE, CLB_IN2, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_RX_TILE_BASE, CLB_IN2);
	CLB_selectInputFilter(CCSI2_RX_TILE_BASE, CLB_IN2, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(CCSI2_RX_TILE_BASE, CLB_IN2);
	//
	// CCSI2_RX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_RX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_RX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_RX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI2_RX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_RX_TILE_BASE, CLB_IN3);
	CLB_setGPREG(CCSI2_RX_TILE_BASE,0);

	CLB_disableCLB(CCSI2_RX_TILE_BASE);
}
void CCSI2_TX_TILE_init(){
	CLB_setOutputMask(CCSI2_TX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI2_TX_TILE_BASE);
	//
	// CCSI2_TX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI2_TX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI2_TX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI2_TX_TILE_BASE, 0);
	//
	// CCSI2_TX_TILE CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_TX_TILE_BASE, CLB_IN0);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN0, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN0);
	//
	// CCSI2_TX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN1);
	//
	// CCSI2_TX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_TX_TILE_BASE, CLB_IN3);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN3);
	//
	// CCSI2_TX_TILE CLB_IN4 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN4, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN4, CLB_GLOBAL_IN_MUX_EPWM1B);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN4, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_TX_TILE_BASE, CLB_IN4);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN4, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN4);
	//
	// CCSI2_TX_TILE CLB_IN5 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN5, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN5, CLB_GLOBAL_IN_MUX_CLB_AUXSIG0);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN5, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI2_TX_TILE_BASE, CLB_IN5);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN5, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN5);
	//
	// CCSI2_TX_TILE CLB_IN6 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI2_TX_TILE_BASE, CLB_IN6, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI2_TX_TILE_BASE, CLB_IN6, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI2_TX_TILE_BASE, CLB_IN6, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI2_TX_TILE_BASE, CLB_IN6, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI2_TX_TILE_BASE, CLB_IN6);
	CLB_setGPREG(CCSI2_TX_TILE_BASE,0);

	CLB_disableCLB(CCSI2_TX_TILE_BASE);
}

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
void CLB_INPUTXBAR_init(){
	myCLBINPUTXBARINPUT0_init();
	myCLBINPUTXBARINPUT1_init();
	myCLBINPUTXBARINPUT2_init();
	myCLBINPUTXBARINPUT3_init();
}

void myCLBINPUTXBARINPUT0_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, myCLBINPUTXBARINPUT0_INPUT, myCLBINPUTXBARINPUT0_SOURCE);
}
void myCLBINPUTXBARINPUT1_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, myCLBINPUTXBARINPUT1_INPUT, myCLBINPUTXBARINPUT1_SOURCE);
}
void myCLBINPUTXBARINPUT2_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, myCLBINPUTXBARINPUT2_INPUT, myCLBINPUTXBARINPUT2_SOURCE);
}
void myCLBINPUTXBARINPUT3_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, myCLBINPUTXBARINPUT3_INPUT, myCLBINPUTXBARINPUT3_SOURCE);
}

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_OUTPUTXBAR_init(){
	CLB_SCLK_1_init();
	CLB_SPICLK_1_init();
	CLB_SCLK_2_init();
	CLB_SOUT_1_init();
}

	
void CLB_SCLK_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, XBAR_OUT_MUX09_CLB2_OUT1);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, XBAR_MUX09);
}
	
void CLB_SPICLK_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SPICLK_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SPICLK_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SPICLK_1, XBAR_OUT_MUX08_CLB2_OUT0);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SPICLK_1, XBAR_MUX08);
}
	
void CLB_SCLK_2_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SCLK_2, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SCLK_2, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SCLK_2, XBAR_OUT_MUX25_CLB4_OUT1);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SCLK_2, XBAR_MUX25);
}
	
void CLB_SOUT_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, XBAR_OUT_MUX12_CLB2_OUT4);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, XBAR_MUX12);
}

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void CLBXBAR_init(){
	myCLBXBAR_TRANSFER_START_SYNC_init();
}

void myCLBXBAR_TRANSFER_START_SYNC_init(){
		
	XBAR_setCLBMuxConfig(myCLBXBAR_TRANSFER_START_SYNC, XBAR_CLB_MUX07_CLB2_OUT5);
	XBAR_enableCLBMux(myCLBXBAR_TRANSFER_START_SYNC, myCLBXBAR_TRANSFER_START_SYNC_ENABLED_MUXES);
}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(myEPWM0_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);	
    EPWM_setTimeBasePeriod(myEPWM0_BASE, 0);	
    EPWM_setTimeBaseCounter(myEPWM0_BASE, 0);	
    EPWM_setTimeBaseCounterMode(myEPWM0_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);	
    EPWM_disablePhaseShiftLoad(myEPWM0_BASE);	
    EPWM_setPhaseShift(myEPWM0_BASE, 0);	
    EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, 0);	
    EPWM_setCounterCompareShadowLoadMode(myEPWM0_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setCounterCompareValue(myEPWM0_BASE, EPWM_COUNTER_COMPARE_B, 0);	
    EPWM_setCounterCompareShadowLoadMode(myEPWM0_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setRisingEdgeDelayCountShadowLoadMode(myEPWM0_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(myEPWM0_BASE);	
    EPWM_setFallingEdgeDelayCountShadowLoadMode(myEPWM0_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(myEPWM0_BASE);	
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_CCSI1_RX_TILE
	Interrupt_register(INT_CCSI1_RX_TILE, &INT_CCSI1_RX_TILE_ISR);
	Interrupt_disable(INT_CCSI1_RX_TILE);
	
	// Interrupt Setings for INT_CCSI1_TX_TILE
	Interrupt_register(INT_CCSI1_TX_TILE, &INT_CCSI1_TX_TILE_ISR);
	Interrupt_disable(INT_CCSI1_TX_TILE);
	
	// Interrupt Setings for INT_CCSI2_RX_TILE
	Interrupt_register(INT_CCSI2_RX_TILE, &INT_CCSI2_RX_TILE_ISR);
	Interrupt_disable(INT_CCSI2_RX_TILE);
	
	// Interrupt Setings for INT_CCSI2_TX_TILE
	Interrupt_register(INT_CCSI2_TX_TILE, &INT_CCSI2_TX_TILE_ISR);
	Interrupt_disable(INT_CCSI2_TX_TILE);
}
//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	CLB_SOUT_2_init();
}

void CLB_SOUT_2_init(){
	XBAR_setOutputLatchMode(OUTPUTXBAR_BASE, CLB_SOUT_2, false);
	XBAR_invertOutputSignal(OUTPUTXBAR_BASE, CLB_SOUT_2, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(OUTPUTXBAR_BASE, CLB_SOUT_2, XBAR_OUT_MUX13_CLB4_OUT4);
	XBAR_enableOutputMux(OUTPUTXBAR_BASE, CLB_SOUT_2, XBAR_MUX13);
}

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
