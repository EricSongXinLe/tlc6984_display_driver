/*
 * led_hal.h
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

#include "driverlib.h"
#include "frames.h"
#include "board.h"
#include "led_driver.h"
#include "system_info.h"

#ifndef LED_HAL_H_
#define LED_HAL_H_

// CCSI bus instantiations
#define CCSIBUS1    0
#define CCSIBUS2    1

// CCSI SCLK configuration
#ifdef TLC698X
#define CLB_SCLK_CNFG_DUAL_EDGE 1
#endif

#ifdef LP589X
#define CLB_SCLK_CNFG_SINGLE_EDGE 1
#endif

//
// GPIO profiling
//
#define LED_GPIO_PROFILING1                40
#define LED_GPIO_PROFILING1_SET_REG        GPIO_O_GPBSET
#define LED_GPIO_PROFILING1_CLEAR_REG      GPIO_O_GPBCLEAR
#define LED_GPIO_PROFILING1_SET            GPIO_GPBSET_GPIO40
#define LED_GPIO_PROFILING1_CLEAR          GPIO_GPBCLEAR_GPIO40
#define LED_GPIO_PROFILING1_PIN_CONFIG     GPIO_40_GPIO40

#define LED_GPIO_PROFILING2                41
#define LED_GPIO_PROFILING2_SET_REG        GPIO_O_GPBSET
#define LED_GPIO_PROFILING2_CLEAR_REG      GPIO_O_GPBCLEAR
#define LED_GPIO_PROFILING2_SET            GPIO_GPBSET_GPIO41
#define LED_GPIO_PROFILING2_CLEAR          GPIO_GPBCLEAR_GPIO41
#define LED_GPIO_PROFILING2_PIN_CONFIG     GPIO_41_GPIO41

#pragma FUNC_ALWAYS_INLINE(LED_HAL_setProfilingGPIO1)
static inline void LED_HAL_setProfilingGPIO1(void)
{
    #pragma diag_suppress = 770
    #pragma diag_suppress = 173
    HWREG(GPIODATA_BASE + LED_GPIO_PROFILING1_SET_REG) =
                                              LED_GPIO_PROFILING1_SET;
    #pragma diag_warning = 770
    #pragma diag_warning = 173
}

#pragma FUNC_ALWAYS_INLINE(LED_HAL_resetProfilingGPIO1)
static inline void LED_HAL_resetProfilingGPIO1(void)
{
    #pragma diag_suppress = 770
    #pragma diag_suppress = 173
    HWREG(GPIODATA_BASE + LED_GPIO_PROFILING1_CLEAR_REG) =
                                                  LED_GPIO_PROFILING1_CLEAR;
    #pragma diag_warning = 770
    #pragma diag_warning = 173
}

#pragma FUNC_ALWAYS_INLINE(LED_HAL_setProfilingGPIO2)
static inline void LED_HAL_setProfilingGPIO2(void)
{
    #pragma diag_suppress = 770
    #pragma diag_suppress = 173
    HWREG(GPIODATA_BASE + LED_GPIO_PROFILING2_SET_REG) =
                                              LED_GPIO_PROFILING2_SET;
    #pragma diag_warning = 770
    #pragma diag_warning = 173
}

#pragma FUNC_ALWAYS_INLINE(LED_HAL_resetProfilingGPIO2)
static inline void LED_HAL_resetProfilingGPIO2(void)
{
    #pragma diag_suppress = 770
    #pragma diag_suppress = 173
    HWREG(GPIODATA_BASE + LED_GPIO_PROFILING2_CLEAR_REG) =
                                                  LED_GPIO_PROFILING2_CLEAR;
    #pragma diag_warning = 770
    #pragma diag_warning = 173
}

extern uint32_t ledXmtBuffer[CCSI_BUS_NUM][MAX_DATA_LENGTH]; //Stores the data bytes to be sent
extern uint16_t ledRcvBuffer[CCSI_BUS_NUM][MAX_DATA_LENGTH]; //Stores the bytes received
extern volatile uint16_t xmtDone[];

uint32_t LED_HAL_SclkInHz(void);
void LED_HAL_initClbLogic();
void LED_HAL_setupInterrupts();
void LED_HAL_startLedClocks();
void LED_HAL_clbTransferBlocking(uint16_t ccsiBusNum, uint32_t *xmtBuffer, uint32_t numXmtWords);
void LED_HAL_clbTransferNonBlocking(uint16_t ccsiBusNum, uint32_t *xmtBuffer, uint32_t numXmtWords, bool syncXfer);
int16_t LED_HAL_clbRxRingBuffRead (uint16_t ccsiBusNum, uint16_t *data);
void LED_HAL_clbSyncTransferStart(void);
void LED_HAL_ringBufferReset(uint16_t ccsiBusNum);
void LED_HAL_enableClbReceiver(uint16_t ccsiBusNum);
void LED_HAL_disableClbReceiver(uint16_t ccsiBusNum);

void LED_HAL_setupProfilingGPIO(void);

#endif /* LED_HAL_H_ */
