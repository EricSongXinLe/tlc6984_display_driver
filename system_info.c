/*
 * system_info.c
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

#include <stdint.h>
#include "system_info.h"

#if LP589X
    const uint16_t FRAME_PERIOD = 16667;      // 16.67ms = 60 Hz frames-per-second
#else
    const uint16_t FRAME_PERIOD = 40000;      // 40ms = 25 Hz frames-per-second
#endif

struct chipStatus chip_status[CCSI_BUS_NUM][MAX_CASCADED_UNITS];

// The cascaded number of devices
#if (CCSI_BUS_NUM > 1)
    const uint16_t CASCADED_UNITS[CCSI_BUS_NUM] = {CASCADED_UNITS_CCSI1,CASCADED_UNITS_CCSI2};
#else
    const uint16_t CASCADED_UNITS[CCSI_BUS_NUM] = {CASCADED_UNITS_CCSI1};
#endif

