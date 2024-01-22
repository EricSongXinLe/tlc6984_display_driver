/*
 * system_info.h
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

#ifndef SYSTEM_INFO_H_
#define SYSTEM_INFO_H_

#include <stdint.h>
#include "led_driver.h"
#include "frames.h"

//
// When set to TRUE, animation based on frames will run
// When set to FALSE, simple tests can be run
//
#define ANIMATION              FALSE

#define MONOCHROMATIC          FALSE

#if LP589X
    // The LP589X EVM is independent mode
    #define TOTAL_SCAN_LINES       16
    #define CASCADED_UNITS_CCSI1   1
#else
    // The TLC698X EVM is 2-stackable mode
    #define TOTAL_SCAN_LINES       16 //32
    #define CASCADED_UNITS_CCSI1   1 //2
#endif

// Desired SCLK frequency (in case of TLC6983 or TLC6984 this SCLK frequency is half of this)
// Note: Exact frequency may not be possible
#if LP589X
    #define SPICLK_FREQ_IN_HZ      7500000
#else
    #define SPICLK_FREQ_IN_HZ     10000000 //10 MHz
#endif

struct chipStatus {
    uint16_t LSD;             // LED Short Detection
    uint16_t LOD;             // LED Open Detection
    outChannel LOD_channels[TOTAL_SCAN_LINES];
    outChannel LSD_channels[TOTAL_SCAN_LINES];
};

extern const uint16_t FRAME_PERIOD;

// **************************************************
// **************************************************
// **** Below code is related to second CCSI bus ****
// **************************************************
// **************************************************
// Total CCSI buses supported
#define CCSI_BUS_NUM                    1

#if (CCSI_BUS_NUM > 1)
    #define CASCADED_UNITS_CCSI2        1
    #define MAX_UNIT(length1, length2)  ((length1) < (length2)?(length2):(length1))
    #define MAX_CASCADED_UNITS          MAX_UNIT(CASCADED_UNITS_CCSI1,CASCADED_UNITS_CCSI2)
#else
    #define MAX_CASCADED_UNITS          CASCADED_UNITS_CCSI1
#endif

#define MAX_DATA_LENGTH                 3 * MAX_CASCADED_UNITS + 1

extern const uint16_t CASCADED_UNITS[CCSI_BUS_NUM];

// For diagnostics
extern struct chipStatus chip_status[CCSI_BUS_NUM][MAX_CASCADED_UNITS];

// ********************************************
// ********************************************
// **** Below code is related to animation ****
// ********************************************
// ********************************************
#define ANIMATION_STEP 1
#define REPEAT_CNT 30

enum ANIMATION_DIRECTION{
    N_S = 0,
    E_W,
    S_N,
    W_E,
    FADE_OUT,
    FADE_IN,
    NO_ANI
};

// Definition about to obtain X-coordinate in image based on chip index and channel index
#if LP589X
    // The LP589X EVM is independent mode
    #define X_COORDINATE(chip_idx, line_idx, ch_idx)     ((ch_idx) + (RGB_CHANNEL_CNT * (MAX_CASCADED_UNITS - (chip_idx) - 1)))
#else
    // The TLC698X EVM is 2-stackable mode
    #define X_COORDINATE(chip_idx, line_idx, ch_idx)     ((ch_idx) + (RGB_CHANNEL_CNT * (chip_idx)))
#endif

// Definition about to obtain Y-coordinate in image based on chip index and line index
#define Y_COORDINATE(chip_idx, line_idx, ch_idx)     ((line_idx))

#endif /* SYSTEM_INFO_H_ */
