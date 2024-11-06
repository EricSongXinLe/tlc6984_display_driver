//#############################################################################
//
// FILE:   main.c
//
// External Connections:
//  EVM can plug directly onto site 1 of F280039C LP.
//
//
//#############################################################################
// Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
// ALL RIGHTS RESERVED 
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include <led_hal.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"
#include "led_driver.h"
#include "frames.h"
#include "system_info.h"
#include <CCSI_Socket.h>
#include "FC_settings.h"
#include "TLC698x_LP589x_APIs.h"
#include <stdio.h>

//
// Globals
//
uint16_t cpuTimer0IntCount = 0;

//
// Function Prototypes
//
__interrupt void cpuTimer0ISR(void);
void initCPUTimer(void);
void configCPUTimer(uint32_t, float, float);

/* Function Name: fadeFrame
 *
 * Purpose:
 *
 * Parameters:
 *
 */
void fadeFrame(int fade_step, int compare_val, uint16_t bus){
    int y_idx = 0;
    int x_idx = 0;
    int x_loop_start = 0;
    int x_loop_idx = 0;
    int x_loop_end = 0;
    int y_loop_end = 0;
    int y_loop_length = 0;
    int x_loop_length = 0;
    int temp_val = 0;

    x_loop_start = 0;
    x_loop_end = SCREEN_SIZE_X;
    x_loop_length = x_loop_end - x_loop_start;
    y_idx = 0;
    y_loop_end = SCREEN_SIZE_Y;
    y_loop_length = y_loop_end - y_idx;

    while(y_loop_length > 0){
        x_idx = x_loop_start;
        x_loop_idx = x_loop_length;
        while(x_loop_idx > 0){
#if MONOCHROMATIC
            if(img_mono[y_idx][x_idx][bus] >= (uint16_t) compare_val) {
                temp_val = (int) img_mono[y_idx][x_idx][bus] + fade_step;
                if(temp_val < 0x00) {
                    img_mono[y_idx][x_idx][bus] = 0x00;
                }
                else {
                    if(temp_val > 0xFF) {
                        img_mono[y_idx][x_idx][bus] = 0xFF;
                    }
                    else {
                        img_mono[y_idx][x_idx][bus] = temp_val & 0xFF;
                    }
                }
            }
#else
            if(img_R_RGB[y_idx][x_idx][bus] >= (uint16_t) compare_val) {
                temp_val = (int) img_R_RGB[y_idx][x_idx][bus] + fade_step;
                if(temp_val < 0x00) {
                    img_R_RGB[y_idx][x_idx][bus] = 0x00;
                }
                else {
                    if(temp_val > 0xFF) {
                        img_R_RGB[y_idx][x_idx][bus] = 0xFF;
                    }
                    else {
                        img_R_RGB[y_idx][x_idx][bus] = temp_val & 0xFF;
                    }
                }
            }
            if(img_G_RGB[y_idx][x_idx][bus] >= (uint16_t) compare_val) {
                temp_val = (int) img_G_RGB[y_idx][x_idx][bus] + fade_step;
                if(temp_val < 0x00) {
                    img_G_RGB[y_idx][x_idx][bus] = 0x00;
                }
                else {
                    if(temp_val > 0xFF) {
                        img_G_RGB[y_idx][x_idx][bus] = 0xFF;
                    }
                    else {
                        img_G_RGB[y_idx][x_idx][bus] = temp_val & 0xFF;
                    }
                }
            }
            if(img_B_RGB[y_idx][x_idx][bus] >= (uint16_t) compare_val) {
                temp_val = (int) img_B_RGB[y_idx][x_idx][bus] + fade_step;
                if(temp_val < 0x00) {
                    img_B_RGB[y_idx][x_idx][bus] = 0x00;
                }
                else {
                    if(temp_val > 0xFF) {
                        img_B_RGB[y_idx][x_idx][bus] = 0xFF;
                    }
                    else {
                        img_B_RGB[y_idx][x_idx][bus] = temp_val & 0xFF;
                    }
                }
            }
#endif
            x_idx ++;
            x_loop_idx--;
        }
        y_idx ++;
        y_loop_length--;
    }
}

/* Function Name: animateFrame
 *
 * Purpose: This function will change the content of the frame stored in MCU SRAM
 *
 * Parameters:
 * move_horizontal -> Number of dots to move to the frame to the right (positive number) or to the left (negative number)
 * move_vertical   -> Number of pixels to move the frame to the bottom (positive number) or to the top (negative number)
 *
 */
void animateFrame(int move_horizontal, int move_vertical, uint16_t bus){
    int y_dir = 0;
    int x_dir = 0;
    int y_idx = 0;
    int x_idx = 0;
    int x_loop_start = 0;
    int x_loop_idx = 0;
    int x_loop_end = 0;
    int y_loop_end = 0;
    int y_loop_length = 0;
    int x_loop_length = 0;

    if(move_horizontal > 0){
        x_loop_start = SCREEN_SIZE_X - 1;
        x_loop_end = move_horizontal - 1;
        x_dir = -1;
        x_loop_length = x_loop_start - x_loop_end;
    }
    else{
        x_loop_start = 0;
        x_loop_end = SCREEN_SIZE_X + move_horizontal;
        x_dir = 1;
        x_loop_length = x_loop_end - x_loop_start;
    }

    if(move_vertical > 0){
        y_idx = SCREEN_SIZE_Y - 1;
        y_loop_end = move_vertical - 1;
        y_dir = -1;
        y_loop_length = y_idx - y_loop_end;
    }
    else{
        y_idx = 0;
        y_loop_end = SCREEN_SIZE_Y + move_vertical;
        y_dir = 1;
        y_loop_length = y_loop_end - y_idx;
    }

    while(y_loop_length > 0){
        x_idx = x_loop_start;
        x_loop_idx = x_loop_length;
        while(x_loop_idx > 0){
#if MONOCHROMATIC
            img_mono[y_idx][x_idx][bus] = img_mono[y_idx - move_vertical][x_idx - move_horizontal][bus];
#else
            img_R_RGB[y_idx][x_idx][bus] = img_R_RGB[y_idx - move_vertical][x_idx - move_horizontal][bus];
            img_G_RGB[y_idx][x_idx][bus] = img_G_RGB[y_idx - move_vertical][x_idx - move_horizontal][bus];
            img_B_RGB[y_idx][x_idx][bus] = img_B_RGB[y_idx - move_vertical][x_idx - move_horizontal][bus];
#endif
            x_idx += x_dir;
            x_loop_idx--;
        }
        y_idx += y_dir;
        y_loop_length--;
    }
}

/* Function Name: prepareFrame
 *
 * Purpose: Prepare the frame data to be send to the LED driver. 
 *          This includes loading frame data from Flash and performing animation on the frame data
 *
 * Parameters:
 * animation_cnt -> Step count of animation.
 * frame -> Number of frame to be loaded from Flash.
 *
 */
void prepareFrame(uint16_t animation_cnt, uint16_t frame, uint16_t bus, uint16_t uAni_speed, uint16_t ani_style, uint16_t ani_length, uint16_t ani_start){
    int y_idx = 0;
    int x_idx = 0;
    int x_idx_flash = 0;
    int y_idx_flash = 0;
    int x_loop_start = 0;
    int x_loop_end = 0;
    int y_loop_end = 0;
    int y_move_step = 0;
    int x_move_step = 0;
    int y_loop_length = 0;
    int x_loop_length = 0;
    int x_loop_idx = 0;
    int y_offset = 0;
    int x_offset = 0;
    uint16_t frame_arr_idx = frame / MAX_FRAMES_ARR;
    uint16_t frame_idx = frame - (frame_arr_idx * MAX_FRAMES_ARR);
    uint16_t tempFlashDataR = 0;
    uint16_t x_idx_odd = 0;
    int move_horizontal = 0;
    int move_vertical = 0;
    int move_horizontal_abs = 0;
    int move_vertical_abs = 0;
    int ani_speed = (int) uAni_speed;
    uint16_t needAnimation = FALSE;

    if(ani_speed > 0) {
        needAnimation = TRUE;
        switch(ani_style) {
            case (E_W):
                move_horizontal = -1 * ani_speed;
                move_vertical = 0;
                break;
            case (W_E):
                move_horizontal = ani_speed;
                move_vertical = 0;
                break;
            case (N_S):
                move_horizontal = 0;
                move_vertical = ani_speed;
                break;
            case (S_N):
                move_horizontal = 0;
                move_vertical = -1 * ani_speed;
                break;
            default:
                needAnimation = FALSE;
                move_horizontal = 0;
                move_vertical = 0;
        }
        if(needAnimation) {
            animateFrame(move_horizontal, move_vertical, bus);
        }
    }

    if(move_horizontal < 0) {
        move_horizontal_abs = -1 * move_horizontal;
    }
    else {
        move_horizontal_abs = move_horizontal;
    }

    if(move_vertical < 0) {
        move_vertical_abs = -1 * move_vertical;
    }
    else {
        move_vertical_abs = move_vertical;
    }

    if(move_vertical == 0){
        y_idx = 0;
        y_loop_end = SCREEN_SIZE_Y;
        y_offset = 0;
    }
    else{
        if(move_vertical > 0){
            y_idx = 0;
            y_loop_end = move_vertical + (move_horizontal_abs * (int)animation_cnt);
            y_offset = (int)frame_size_y_dir[frame] - ((move_vertical * (int)animation_cnt) + move_vertical);
        }
        else{
            y_idx = SCREEN_SIZE_Y + move_vertical - (move_horizontal_abs * (int)animation_cnt);
            y_loop_end = SCREEN_SIZE_Y;
            y_move_step = -1 * move_vertical;
            y_offset = ((y_move_step * (int)animation_cnt) + y_move_step) - (int)SCREEN_SIZE_Y;
        }
    }
    y_loop_length = y_loop_end - y_idx;

    if(move_horizontal == 0){
        x_loop_start = 0;
        x_loop_end = SCREEN_SIZE_X;
    }
    else{
        if(move_horizontal > 0){
            x_loop_start = 0;
            x_loop_end = move_horizontal + (move_vertical_abs * (int)animation_cnt);
            x_offset = (int)frame_size_x_dir[frame] - ((move_horizontal * (int)animation_cnt) + move_horizontal);
        }
        else{
            x_loop_start = SCREEN_SIZE_X + move_horizontal - (move_vertical_abs * (int)animation_cnt);;
            x_loop_end = SCREEN_SIZE_X;
            x_move_step = -1 * move_horizontal;
            x_offset = ((x_move_step * (int)animation_cnt) + x_move_step) - (int)SCREEN_SIZE_X;
        }
    }
    x_loop_length = x_loop_end - x_loop_start;
    
    while(y_loop_length > 0){
        x_idx = x_loop_start;
        x_loop_idx = x_loop_length;
        y_idx_flash = y_idx + y_offset;
        while(x_loop_idx > 0){
            if((FRAME_RBG_CNT * 2) < FRAME_SIZE_X){
                // This means 2 color mode
                // first get index - divide by 16
                x_idx_flash = ((x_idx + x_offset) >> 4) & 0xFF;
                // For black/white image need to get bits
                tempFlashDataR = pFrameDataArray[frame_arr_idx][frame_idx][y_idx_flash][x_idx_flash];
#if MONOCHROMATIC
                img_mono[y_idx][x_idx][bus] = tempFlashDataR & (0x1 << ((x_idx + x_offset) & 0xF))?1:0;
#else
                img_R_RGB[y_idx][x_idx][bus] = tempFlashDataR & (0x1 << ((x_idx + x_offset) & 0xF))?1:0;
                // For black/white colors, the data for G and B is same as R
                img_G_RGB[y_idx][x_idx][bus] = img_R_RGB[y_idx][x_idx][bus];
                img_B_RGB[y_idx][x_idx][bus] = img_R_RGB[y_idx][x_idx][bus];
#endif
            }
            else{
                // This means 256 color mode or 16 million color mode
                //x_idx_odd = 1;
                x_idx_flash = x_idx + x_offset;
                x_idx_odd = x_idx_flash & 0x1;
                x_idx_flash = x_idx_flash >> 1;
                tempFlashDataR = pFrameDataArray[frame_arr_idx][frame_idx][y_idx_flash][x_idx_flash];
#if MONOCHROMATIC
                if(x_idx_odd){
                    img_mono[y_idx][x_idx][bus] = tempFlashDataR & 0xFF;
                }
                else{
                    img_mono[y_idx][x_idx][bus] = (tempFlashDataR >> 8) & 0xFF;
                }
#else
                if(x_idx_odd){
                    img_R_RGB[y_idx][x_idx][bus] = tempFlashDataR & 0xFF;
                }
                else{
                    img_R_RGB[y_idx][x_idx][bus] = (tempFlashDataR >> 8) & 0xFF;
                }
                if((FRAME_RBG_CNT * 2) > FRAME_SIZE_X){
                    tempFlashDataG = pFrameDataArray[frame_arr_idx][frame_idx][y_idx_flash][x_idx_flash + (FRAME_SIZE_X >> 1)];
                    tempFlashDataB = pFrameDataArray[frame_arr_idx][frame_idx][y_idx_flash][x_idx_flash + FRAME_SIZE_X];
                    if(x_idx_odd){
                        img_G_RGB[y_idx][x_idx][bus] = tempFlashDataG & 0xFF;
                        img_B_RGB[y_idx][x_idx][bus] = tempFlashDataB & 0xFF;
                    }
                    else{
                        img_G_RGB[y_idx][x_idx][bus] = (tempFlashDataG >> 8) & 0xFF;
                        img_B_RGB[y_idx][x_idx][bus] = (tempFlashDataB >> 8) & 0xFF;
                    }
                }
                else{
                    // For 256 colors, the data for G and B is same as R
                    img_G_RGB[y_idx][x_idx][bus] = img_R_RGB[y_idx][x_idx][bus];
                    img_B_RGB[y_idx][x_idx][bus] = img_R_RGB[y_idx][x_idx][bus];
                }
#endif
            }
            x_idx++;
            x_loop_idx--;
        }
        y_idx++;
        y_loop_length--;
    }

    // Check if need to fade in or out
    if(ani_speed > 0) {
        switch(ani_style) {
            case (FADE_OUT):
                fadeFrame(-1 * ani_speed * (int)animation_cnt, ani_start, bus);
                break;
            case (FADE_IN):
                fadeFrame(-1 * (int)ani_length + ani_speed * (int)animation_cnt, ani_start, bus);
                break;
            default:
                break;
        }
    }
}

//
// Main
//
void main(void)
 {
    volatile uint16_t errorDetected;
    uint16_t i;
    uint16_t y_dir = 0; //Loop index
    uint16_t x_dir = 0;
    uint16_t current_frame[CCSI_BUS_NUM];
    uint16_t max_ani[CCSI_BUS_NUM];
    uint16_t current_ani[CCSI_BUS_NUM];
    uint16_t current_repeat[CCSI_BUS_NUM];
    uint16_t needSendSram = 0;
    uint16_t bus_idx = 0;
    uint16_t current_ani_speed[CCSI_BUS_NUM];
    uint16_t current_ani_step_repeat[CCSI_BUS_NUM];
    uint16_t current_ani_style[CCSI_BUS_NUM];
    uint16_t current_ani_length[CCSI_BUS_NUM];
    uint16_t ani_start[CCSI_BUS_NUM];
    uint16_t max_repeat[CCSI_BUS_NUM];
    uint16_t max_ani_step_repeat[CCSI_BUS_NUM];

    uint16_t fc0_0 = _FC0_0;
    uint16_t fc0_1 = _FC0_1;


    /*  USER-DEFINED VARS*/
    uint16_t frame1Cnt = 0;
    uint16_t frame2Cnt = 0;
    // Initialize device clock and peripherals
    Device_init();
   //printf("Startup\n");

    // Disable pin locks and enable internal pullups.
    Device_initGPIO();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    // Board initialization (generated by SYSCFG tool)
    Board_init();

    // Configure GPIOs for profiling
    LED_HAL_setupProfilingGPIO();

    // Init CLB logic
    LED_HAL_initClbLogic();

    // Configure and enable system interrupt
    LED_HAL_setupInterrupts();

    // Initialize & start LED driver clock
    LED_HAL_startLedClocks();

    // Register timer interrupts
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);

    // Init timer
    initCPUTimer();

    // Configure CPU timer 0 to interrupt every 1ms second
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, FRAME_PERIOD);

    // Enable timer interrupts
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_TIMER0);

    // Start timer
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    ////////////////// TEST \\\\\\\\\\\\\\\\\\\\\

    // Reset VSYNC
    vsyncDone = 0;

    // Clear buffers
    for(bus_idx = 0 ; bus_idx < CCSI_BUS_NUM ; bus_idx++){
        for(i = 0; i < MAX_DATA_LENGTH; i++)
        {
            ledXmtBuffer[bus_idx][i] = 0;
            ledRcvBuffer[bus_idx][i] = 0;
        }
        //Write chip index.
        LED_Set_Chip_Index(FALSE, bus_idx);

        // Clear number of cascaded units
        fc0_0 &= 0xFFE0;
        // Set number of cascaded units for current bus
        fc0_0 |= (CASCADED_UNITS[bus_idx] - 1);
        
        // Clear number of scan lines
        fc0_1 &= (0xFFFF - (SCAN_NUM__MAX >> 16));
        // Set number of scan lines
        fc0_1 |= (TOTAL_SCAN_LINES - 1);

        LED_Write_Reg_Broadcast(W_FC0, _FC0_2, fc0_1, fc0_0, FALSE, bus_idx);
        LED_Write_Reg_Broadcast(W_FC1, _FC1_2, _FC1_1, _FC1_0, FALSE, bus_idx);
        LED_Write_Reg_Broadcast(W_FC2, _FC2_2, _FC2_1, _FC2_0, FALSE, bus_idx);
        LED_Write_Reg_Broadcast(W_FC3, _FC3_2, _FC3_1, _FC3_0, FALSE, bus_idx);
        LED_Write_Reg_Broadcast(W_FC4, _FC4_2, _FC4_1, _FC4_0, FALSE, bus_idx);
    }

    // Write black frame - To prevent showing existing frames in LED Driver SRAM
    // Clear MCU SRAM frame
    for(y_dir = 0; y_dir < SCREEN_SIZE_Y; y_dir++){
        for(x_dir = 0; x_dir < SCREEN_SIZE_X; x_dir++){
            for(bus_idx = 0; bus_idx < CCSI_BUS_NUM; bus_idx++){
#if MONOCHROMATIC
                img_mono[y_dir][x_dir][bus_idx] = 0x00;
#else
                img_R_RGB[y_dir][x_dir][bus_idx] = 0x00;
                img_G_RGB[y_dir][x_dir][bus_idx] = 0x00;
                img_B_RGB[y_dir][x_dir][bus_idx] = 0x00;
#endif
            }
        }
    }
    
    // Set VSYNC
    vsyncDone = 0;

    // Send black frame to LED Driver SRAM and SYNC
    LED_Write_Black_ALL();
    sendSYNCnoWait();



    const unsigned char letter_U[15][15] = {
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 0
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 1
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 2
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 3
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 4
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 5
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0}, // Row 6
                                            {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Row 7
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 8
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 9
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 10
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 11
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 12
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 13
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Row 14
                                        };

    const unsigned char letter_C[15][15] = {
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 0
                                            {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Row 1
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 2
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 3
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 4
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 5
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 6
                                            {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Row 7
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 8
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 9
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row10
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row11
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row12
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row13
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Row14
                                        };
    const unsigned char letter_L[15][15] = {
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 0
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 1
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 2
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 3
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 4
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 5
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 6
                                            {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, // Row 7
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 8
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 9
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row10
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row11
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row12
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row13
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Row14
                                        };
    /*const unsigned char Test[11][11] = {
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
                                                 };*/
    const unsigned char letter_A[15][15] = {
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 0
                                            {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, // Row 1
                                            {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0}, // Row 2
                                            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, // Row 3
                                            {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}, // Row 4
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // Row 5
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // Row 6
                                            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, // Row 7
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 8
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row 9
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row10
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row11
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row12
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Row13
                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Row14
                                        };

    if(ANIMATION == FALSE) {
        //
        // Execute your simple test here
        //
        if(SIMPLE_TEST == TRUE){
            while(1){
                for(frame1Cnt = 0; frame1Cnt<30; frame1Cnt++){
                    LED_Write_White_ALL();
                    //LED_Write_Black_ALL();

                    // Send VSYNC
                    sendSYNC();

                    // Update LOD and LSD information
                    LED_Update_Chip_Status();
                }
                /*
                for(frame1Cnt = 0; frame1Cnt<4; frame1Cnt++){
                    LED_Write_Pos(0,0);
                    //LED_Write_Black_ALL();

                    // Send VSYNC
                    sendSYNC();

                    // Update LOD and LSD information
                    LED_Update_Chip_Status();
                }
                for(frame1Cnt = 0; frame1Cnt<4; frame1Cnt++){
                    //LED_Write_Pos(1,1);
                    LED_Write_Black_ALL();

                    // Send VSYNC
                    sendSYNC();

                    // Update LOD and LSD information
                    LED_Update_Chip_Status();
                }*/


                for(x_dir = 0; x_dir < 11; x_dir++){
                    for(y_dir = 0; y_dir < 11; y_dir++){
                        for(frame1Cnt = 0; frame1Cnt<6; frame1Cnt++){
                            LED_Write_Pos(x_dir,y_dir);
                            //LED_Write_Black_ALL();

                            // Send VSYNC
                            sendSYNC();



                            // Update LOD and LSD information
                            LED_Update_Chip_Status();
                        }
                        /*
                        for(frame1Cnt = 0; frame1Cnt<3; frame1Cnt++){
                            //LED_Write_Pos(1,1);
                            LED_Write_Black_ALL();

                            // Send VSYNC
                            sendSYNC();

                            // Update LOD and LSD information
                            LED_Update_Chip_Status();
                        }*/
                    }
                }
            }
        }





        while(1) {
            /*
             IMPORTANT DEBUGGING GUIDANCE: If the Display won't turn on after the code is downloaded, Plug and unplug the yellow cable that connects the resistor.
             */
            // Make the screen all white
            /* TEST1: 10Frames of White, 10 Frames of black*/
            for(frame1Cnt = 0; frame1Cnt<10; frame1Cnt++){
                            LED_Write_White_ALL();
                            //LED_Write_Black_ALL();

                            // Send VSYNC
                            sendSYNC();

                            // Update LOD and LSD information
                            LED_Update_Chip_Status();
                        }
            for(frame1Cnt = 0; frame1Cnt<25; frame1Cnt++){
                LED_Write_Letter(letter_U);
                //LED_Write_Letter(Test);
                //LED_Write_Black_ALL();

                // Send VSYNC
                sendSYNC();

                // Update LOD and LSD information
                LED_Update_Chip_Status();
            }

            for(frame1Cnt = 0; frame1Cnt<25; frame1Cnt++){
                            LED_Write_Letter(letter_C);
                            //LED_Write_Letter(Test);
                            //LED_Write_Black_ALL();

                            // Send VSYNC
                            sendSYNC();

                            // Update LOD and LSD information
                            LED_Update_Chip_Status();
            }
            for(frame1Cnt = 0; frame1Cnt<25; frame1Cnt++){
                                        LED_Write_Letter(letter_L);
                                        //LED_Write_Black_ALL();

                                        // Send VSYNC
                                        sendSYNC();

                                        // Update LOD and LSD information
                                        LED_Update_Chip_Status();
                        }
            for(frame1Cnt = 0; frame1Cnt<25; frame1Cnt++){
                            LED_Write_Letter(letter_A);
                            //LED_Write_Black_ALL();

                            // Send VSYNC
                            sendSYNC();

                            // Update LOD and LSD information
                            LED_Update_Chip_Status();
            }

            for(frame2Cnt = 0; frame2Cnt<10; frame2Cnt++){
                LED_Write_Black_ALL();
                //LED_Write_Black_ALL();

                // Send VSYNC
                sendSYNC();

                // Update LOD and LSD information
                LED_Update_Chip_Status();
            }

        }
    }
    else {
        // Initial variables
        for(bus_idx = 0; bus_idx < CCSI_BUS_NUM; bus_idx++){
            current_frame[bus_idx] = 0;
            current_ani[bus_idx] = 0;
            current_repeat[bus_idx] = 0;
            current_ani_step_repeat[bus_idx] = 0;
            max_ani_step_repeat[bus_idx] = 0;
            current_ani_speed[bus_idx] = ANIMATION_STEP;
            current_ani_style[bus_idx] = N_S;
            current_ani_length[bus_idx] = 0xFF;
            ani_start[bus_idx] = 0x1C;
            max_repeat[bus_idx] = REPEAT_CNT;
            // Let CCSI bus 2 start at a different frame
            if(bus_idx == 1)
                current_frame[bus_idx] = FRAME_CNT >> 1;
        }

        while(1){
            // Prepare frame
            needSendSram = 0;
            for(bus_idx = 0; bus_idx < CCSI_BUS_NUM; bus_idx++){
                // Check if we need to send SRAM data to one of the buses
                if(current_repeat[bus_idx] < 2){
                    needSendSram = 1;
                }
                if(current_ani_speed[bus_idx] > 0){
                    switch(current_ani_style[bus_idx]) {
                        case (W_E):
                        case (E_W):
                            max_ani[bus_idx] = frame_size_x_dir[current_frame[bus_idx]] / current_ani_speed[bus_idx];
                            break;
                        case (N_S):
                        case (S_N):
                            max_ani[bus_idx] = frame_size_y_dir[current_frame[bus_idx]] / current_ani_speed[bus_idx];
                            break;
                        case (NO_ANI):
                            max_ani[bus_idx] = 1;
                            break;
                        default:
                            if(current_ani_length[bus_idx] > 0) {
                                max_ani[bus_idx] = current_ani_length[bus_idx] / current_ani_speed[bus_idx];
                            }
                            else {
                                max_ani[bus_idx] = 1;
                            }
                    }
                }
                else{
                    max_ani[bus_idx] = 1;
                }
                if((current_ani_step_repeat[bus_idx] == 0) & (current_repeat[bus_idx] == 0) & (current_ani[bus_idx] < max_ani[bus_idx])) {
                    prepareFrame(current_ani[bus_idx], current_frame[bus_idx], bus_idx, current_ani_speed[bus_idx], current_ani_style[bus_idx], current_ani_length[bus_idx], ani_start[bus_idx]);
                }
            }

            // If 1 bus requires to send SRAM data, currently send it to all buses
            if(needSendSram) {
                LED_sendSRAM(TOTAL_SCAN_LINES);
            }
            // Send VSYNC
            sendSYNC();

            // Update variables
            for(bus_idx = 0; bus_idx < CCSI_BUS_NUM; bus_idx++){
                current_ani[bus_idx]++;
                if(current_ani[bus_idx] >= max_ani[bus_idx]){
                    // Reset current animation to max to prevent overflow in case repeat is very long
                    current_ani[bus_idx] = max_ani[bus_idx];

                    current_repeat[bus_idx]++;
                    if(current_repeat[bus_idx] > REPEAT_CNT) {
                        current_repeat[bus_idx] = 0;
                        current_ani[bus_idx] = 0;

                        current_frame[bus_idx]++;
                        current_ani_style[bus_idx]++;
                        if(current_ani_style[bus_idx] > FADE_IN) {
                            current_ani_style[bus_idx] = 0;
                        }

                        if(current_frame[bus_idx] >= FRAME_CNT) {
                            current_frame[bus_idx] = 0;
                        }
                    }
                }
            }
        }
    }
}

//
// initCPUTimer - This function initializes the CPU timer 0
// to a known state.
//
void
initCPUTimer(void)
{
    //
    // Initialize timer period to maximum
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);

    //
    // Reset interrupt counter
    //
    cpuTimer0IntCount = 0;
}

//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void
configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp;

    //
    // Initialize timer period:
    //
    temp = (uint32_t)((freq / 1000000) * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(cpuTimer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);

}

//
// cpuTimer0ISR - Counter for CpuTimer0
//
__interrupt void
cpuTimer0ISR(void)
{
    cpuTimer0IntCount++;

    vsyncDone = 1;
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// End of File
//
