/*
 * led_hal.c
 *
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
 */

//
// Included Files
//
#include <led_hal.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"

#define DEBUG   1

#define CLBFIFOMAXWRITES   4  // Maximum reads/writes allowed to avoid overrun of TX FIFO

#define GPREG_TRANSFER_START        1U
#define GPREG_TRANSFER_START_SYNC   6U
#define GPREG_ENABLE_RCVR           3U

#define RCV_BUF_SIZE    128     // This must always be a power of 2

// Ring buffer for receiving data
uint32_t clbRxRingBuff[CCSI_BUS_NUM][RCV_BUF_SIZE];
//#pragma DATA_SECTION(clbRxRingBuff, "ramgs0");  // map the RX data to memory
uint16_t ringBuffHead[CCSI_BUS_NUM], ringBuffTail[CCSI_BUS_NUM];

//
// Globals
//
#ifdef CCSI2_TX_TILE_BASE
uint32_t txTileBase[] = {CCSI1_TX_TILE_BASE, CCSI2_TX_TILE_BASE};
#else
uint32_t txTileBase[] = {CCSI1_TX_TILE_BASE};
#endif
#ifdef CCSI2_RX_TILE_BASE
uint32_t rxTileBase[] = {CCSI1_RX_TILE_BASE, CCSI2_RX_TILE_BASE};
#else
uint32_t rxTileBase[] = {CCSI1_RX_TILE_BASE};
#endif

volatile uint16_t xmtCount[CCSI_BUS_NUM];
volatile uint16_t xmtDone[CCSI_BUS_NUM];

volatile uint32_t *xmtBufPtr[CCSI_BUS_NUM];
volatile uint16_t xmtXferSize[CCSI_BUS_NUM];

uint32_t ledXmtBuffer[CCSI_BUS_NUM][MAX_DATA_LENGTH]; //Stores the data bytes to be sent
uint16_t ledRcvBuffer[CCSI_BUS_NUM][MAX_DATA_LENGTH]; //Stores the data bytes to be sent
//#pragma DATA_SECTION(ledXmtBuffer, "ramgs0");  // map the TX data to memory


uint32_t LED_HAL_SclkInHz(void) {
    uint16_t tbPrdVal = 0U;
    uint32_t result;

    tbPrdVal = (uint16_t)(DEVICE_SYSCLK_FREQ / (2 * SPICLK_FREQ_IN_HZ));

    result = DEVICE_SYSCLK_FREQ / (2 * tbPrdVal);
#ifdef CLB_SCLK_CNFG_DUAL_EDGE
    return (result >> 2);
#else
    return result;
#endif
}

//
// Initializes CLB logic
//
void LED_HAL_initClbLogic()
{
    uint16_t i;

    for (i=0; i<CCSI_BUS_NUM; i++)
    {
        // Setup the CLB Tiles
        initTILE1(rxTileBase[i]);
        initTILE2(txTileBase[i]);

        // Initialize the TX CLB PULL FIFOs to some default/safe value
        HWREG(txTileBase[i] + CLB_DATAEXCH + CLB_O_PULL(0)) = 0xFFFFFFFF;
        HWREG(txTileBase[i] + CLB_DATAEXCH + CLB_O_PULL(1)) = 0xFFFFFFFF;
        HWREG(txTileBase[i] + CLB_DATAEXCH + CLB_O_PULL(2)) = 0xFFFFFFFF;
        HWREG(txTileBase[i] + CLB_DATAEXCH + CLB_O_PULL(3)) = 0xFFFFFFFF;

        // Load the TX CLB with some default/safe value
        CLB_writeInterface(txTileBase[i], CLB_ADDR_COUNTER_1_LOAD, 0xFFFFFFFF);

        // Put TX CLB in idle
        CLB_setGPREG(txTileBase[i], 0U << GPREG_TRANSFER_START);

        // Setup receive ring buffer pointers
        ringBuffHead[i] = 0;
        ringBuffTail[i] = 0;

        //
        // Reset the RX CLB PUSH FIFO pointer
        // Note: The register CLB_O_BUF_PTR contains both PUSH and PULL FIFO pointers.
        //       However PULL FIFO pointer is not used, so it's okay to overwrite it.
        //
        HWREG(rxTileBase[i] + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;

        // Clear the RX CLB PUSH FIFO
        HWREG(rxTileBase[i] + CLB_DATAEXCH + CLB_O_PUSH(0)) = 0x0;
        HWREG(rxTileBase[i] + CLB_DATAEXCH + CLB_O_PUSH(1)) = 0x0;
        HWREG(rxTileBase[i] + CLB_DATAEXCH + CLB_O_PUSH(2)) = 0x0;
        HWREG(rxTileBase[i] + CLB_DATAEXCH + CLB_O_PUSH(3)) = 0x0;

        // Modify CLB_SCLK output configuration if single-edge operation needed
        #ifdef CLB_SCLK_CNFG_SINGLE_EDGE
            //TX TILE CLB IN4 initialization
            CLB_configGlobalInputMux(txTileBase[i], CLB_IN4, CLB_GLOBAL_IN_MUX_EPWM1A);
        #endif

        // Enable the CLB logic
        CLB_enableCLB(rxTileBase[i]);
        CLB_enableCLB(txTileBase[i]);

        // Disable receive in CLB RX
        LED_HAL_disableClbReceiver(i);

        // Init globals used for xmit
        xmtCount[i] = 0;
        xmtDone[i] = 1;
    }

    // Enable TX and RX CLB interrupt
    Interrupt_enable(INT_CCSI1_RX_TILE);
    Interrupt_enable(INT_CCSI1_TX_TILE);

    #if (CCSI_BUS_NUM == 2)
        Interrupt_enable(INT_CCSI2_RX_TILE);
        Interrupt_enable(INT_CCSI2_TX_TILE);
    #endif
}

//
// Registers CLB interrupts
//
void LED_HAL_setupInterrupts()
{
    //
    // CCSI1 & CCSI2 bus interrupts already registered by sysconfig.
    // Register additional CCSI bus interrupts if needed.
    //
}

//
// Configures PWM to generate SCLK and SPI_CLK (2x SCLK)
//

void LED_HAL_startLedClocks()
{
    uint16_t tbPrdVal = 0U;

    //
    // Configure PWM to continue operating even during emulation stop
    //
    EPWM_setEmulationMode(myEPWM0_BASE, EPWM_EMULATION_FREE_RUN);

    //
    // Set Time Base counter Clock equal to SYSCLK
    //
    EPWM_setClockPrescaler(myEPWM0_BASE, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);

    //
    // Configure Time Base Counter Mode
    //
    EPWM_setTimeBaseCounterMode(myEPWM0_BASE, EPWM_COUNTER_MODE_UP_DOWN);

    //
    // Calculate TBPRD value to be configured for achieving desired
    // signal
    // NOTE: Actual output clock frequency can be calculated as:
    //       DEVICE_SYSCLK_FREQ / (2 * tpPrdVal), where tpPrdVal is an integer
    //
    tbPrdVal = (uint16_t)(DEVICE_SYSCLK_FREQ / (2 * SPICLK_FREQ_IN_HZ));

    //
    // Configure TBPRD value
    //
    EPWM_setTimeBasePeriod(myEPWM0_BASE, tbPrdVal);

    //
    // Default Configurations.
    //
    EPWM_disablePhaseShiftLoad(myEPWM0_BASE);
    EPWM_setPhaseShift(myEPWM0_BASE, 0U);
    EPWM_setTimeBaseCounter(myEPWM0_BASE, 0U);

    //
    // Setup PWMxA to generate SPICLK
    //
    EPWM_setActionQualifierAction(myEPWM0_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(myEPWM0_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

    //
    // Force PWMxB low to ensure it goes high when CTR=Zero
    //
    EPWM_setActionQualifierSWAction(myEPWM0_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW);
    EPWM_forceActionQualifierSWAction(myEPWM0_BASE,EPWM_AQ_OUTPUT_B);

    //
    // Setup PWMxB to generate SCLK
    //
    EPWM_setActionQualifierAction(myEPWM0_BASE,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_TOGGLE,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
}

//
// Transmit and receive data using CLB; function will block execution
// until transfer completes.
//

void LED_HAL_clbTransferBlocking(uint16_t ccsiBusNum, uint32_t *xmtBuffer,
                                 uint32_t numXmtWords)
{
    uint32_t tileAddr = txTileBase[ccsiBusNum];
    uint16_t i;

    #if 0
    LED_HAL_setProfilingGPIO1();
    #endif

    //
    // Initialize variables to keep track of frame progress
    //
    xmtCount[ccsiBusNum] = 0;
    xmtDone[ccsiBusNum] = 0;

    xmtBufPtr[ccsiBusNum] = xmtBuffer;
    xmtXferSize[ccsiBusNum] = numXmtWords;

    // Load first data value directly to CLB HLC register
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R1,
                       *(xmtBufPtr[ccsiBusNum] + xmtCount[ccsiBusNum]++));

    // Load first 4 data values to CLB PULL FIFO
    for(i=0; i<CLBFIFOMAXWRITES && xmtCount[ccsiBusNum]<xmtXferSize[ccsiBusNum]; i++)
        HWREG(tileAddr + CLB_DATAEXCH + CLB_O_PULL(i)) =
                *(xmtBufPtr[ccsiBusNum] + xmtCount[ccsiBusNum]++);

    // Reset the CLB FIFO PULL pointer
    HWREG(tileAddr + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;

    // Reset the CLB xmt count & load total number of xmit words
    CLB_writeInterface(tileAddr, CLB_ADDR_COUNTER_0_LOAD, 0);
    CLB_writeInterface(tileAddr, CLB_ADDR_COUNTER_0_MATCH1,
                       xmtXferSize[ccsiBusNum]+1);  // +1 to account for START

    // Clear the flag registers in case there is anything pending
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R2, 0x0);
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R3, 0x0);

    #if 0
    LED_HAL_resetProfilingGPIO1();
    #endif

    // Signal CLB to start transmission
    CLB_setGPREG(txTileBase[ccsiBusNum], 1U << GPREG_TRANSFER_START);

    //
    // Wait for data transmit to complete
    //
    while(xmtDone[ccsiBusNum] == 0){}

//    // Signal CLB to go to IDLE state
//    CLB_setGPREG(CCSI1_TX_TILE_BASE, 0U << GPREG_TRANSFER_START);
}

//
// Transmit and receive data using CLB; function will NOT block execution.
//

void LED_HAL_clbTransferNonBlocking(uint16_t ccsiBusNum, uint32_t *xmtBuffer,
                                    uint32_t numXmtWords, bool syncXfer)
{
    uint32_t tileAddr = txTileBase[ccsiBusNum];
    uint16_t i;

    #if 0
    if(ccsiBusNum == 0)
        LED_HAL_setProfilingGPIO1();
    else
        LED_HAL_setProfilingGPIO2();
    #endif

    //
    // Initialize variables to keep track of frame progress
    //
    xmtCount[ccsiBusNum] = 0;
    xmtDone[ccsiBusNum] = 0;

    xmtBufPtr[ccsiBusNum] = xmtBuffer;
    xmtXferSize[ccsiBusNum] = (uint16_t) numXmtWords;

    // Load first data value directly to CLB HLC register
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R1,
                       *(xmtBufPtr[ccsiBusNum] + (uint32_t)xmtCount[ccsiBusNum]++));

    // Load first 4 data values to CLB PULL FIFO
    for(i=0; i<CLBFIFOMAXWRITES && xmtCount[ccsiBusNum]<xmtXferSize[ccsiBusNum]; i++)
        HWREG(tileAddr + CLB_DATAEXCH + CLB_O_PULL(i)) =
                *(xmtBufPtr[ccsiBusNum] + (uint32_t)xmtCount[ccsiBusNum]++);

    // Reset the CLB FIFO PULL pointer
    HWREG(tileAddr + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;

    // Reset the CLB xmt count & load total number of xmit words
    CLB_writeInterface(tileAddr, CLB_ADDR_COUNTER_0_LOAD, 0);
    CLB_writeInterface(tileAddr, CLB_ADDR_COUNTER_0_MATCH1,
                       xmtXferSize[ccsiBusNum]+1);  // +1 to account for START

    // Clear the flag registers in case there is anything pending
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R2, 0x0);
    CLB_writeInterface(tileAddr, CLB_ADDR_HLC_R3, 0x0);

    // Immediately start transmit if sync transfer is not needed
    if(syncXfer == false)
    {
        // Signal CLB to start transmission
        CLB_setGPREG(tileAddr, 1U << GPREG_TRANSFER_START);
    }

    #if 0
    if(ccsiBusNum == 0)
        LED_HAL_resetProfilingGPIO1();
    else
        LED_HAL_resetProfilingGPIO2();
    #endif
}

//
// Signal all TX CLBs to start transmit
// NOTE: CSSI1 TX tile acts as master for all TX tiles
//
void LED_HAL_clbSyncTransferStart(void)
{
    CLB_setGPREG(CCSI1_TX_TILE_BASE, 1U << GPREG_TRANSFER_START_SYNC);
}

void LED_HAL_enableClbReceiver(uint16_t ccsiBusNum)
{
    uint32_t gpRegVal = CLB_getGPREG(rxTileBase[ccsiBusNum]);

    // First check that receiver is in IDLE state
    while(HWREG(rxTileBase[ccsiBusNum] + CLB_LOGICCTL + CLB_O_DBG_OUT) &
            (CLB_DBG_OUT_FSM0_S1 | CLB_DBG_OUT_FSM0_S0)) {}

    // Enable receiver
    gpRegVal |= (1U << GPREG_ENABLE_RCVR);
    CLB_setGPREG(rxTileBase[ccsiBusNum], gpRegVal);
}

void LED_HAL_disableClbReceiver(uint16_t ccsiBusNum)
{
    uint32_t gpRegVal = CLB_getGPREG(rxTileBase[ccsiBusNum]);

    // First check that receiver is in IDLE state
    while(HWREG(rxTileBase[ccsiBusNum] + CLB_LOGICCTL + CLB_O_DBG_OUT) &
            (CLB_DBG_OUT_FSM0_S1 | CLB_DBG_OUT_FSM0_S0)) {}

    // Disable receiver
    gpRegVal &= ~(1U << GPREG_ENABLE_RCVR);
    CLB_setGPREG(rxTileBase[ccsiBusNum], gpRegVal);
}

// Resets the receive ring buffer pointers (trashes data)
void LED_HAL_ringBufferReset(uint16_t ccsiBusNum)
{
    ringBuffHead[ccsiBusNum] = 0;
    ringBuffTail[ccsiBusNum] = 0;
}

// Checks if ring buffer is empty
uint16_t LED_HAL_ringBufferEmpty(uint16_t ccsiBusNum)
{
    return ((ringBuffHead[ccsiBusNum] - ringBuffTail[ccsiBusNum]) == 0U) ? 1 : 0;
}

// Checks if ring buffer is full
uint16_t LED_HAL_ringBufferFull(uint16_t ccsiBusNum)
{
    return ((ringBuffHead[ccsiBusNum] - ringBuffTail[ccsiBusNum]) == RCV_BUF_SIZE) ? 1 : 0;
}

// Reads a single element from the ring buffer
int16_t LED_HAL_clbRxRingBuffRead (uint16_t ccsiBusNum, uint16_t *data)
{
    if(LED_HAL_ringBufferEmpty(ccsiBusNum) == 0)
    {
        uint16_t offset = (ringBuffTail[ccsiBusNum] & (RCV_BUF_SIZE-1));

        *data =  (uint16_t)(clbRxRingBuff[ccsiBusNum][offset]);
        ringBuffTail[ccsiBusNum]++;
        return 0;
    }
    else
    {
        return -1;
    }
}

// Writes a single element to the ring buffer
int16_t LED_HAL_clbRxRingBuffWrite (uint16_t ccsiBusNum, uint32_t *data)
{
    if(LED_HAL_ringBufferFull(ccsiBusNum) == 0)
    {
        uint16_t offset = (ringBuffHead[ccsiBusNum] & (RCV_BUF_SIZE-1));

        clbRxRingBuff[ccsiBusNum][offset] = *data;
        ringBuffHead[ccsiBusNum]++;
        return 0;
    }
    else
    {
        return -1;
    }
}

void LED_HAL_setupProfilingGPIO(void)
{
    GPIO_setDirectionMode(LED_GPIO_PROFILING1, GPIO_DIR_MODE_OUT);
    GPIO_setQualificationMode(LED_GPIO_PROFILING1, GPIO_QUAL_SYNC);
    GPIO_setPinConfig(LED_GPIO_PROFILING1_PIN_CONFIG);

    GPIO_setDirectionMode(LED_GPIO_PROFILING2, GPIO_DIR_MODE_OUT);
    GPIO_setQualificationMode(LED_GPIO_PROFILING2, GPIO_QUAL_SYNC);
    GPIO_setPinConfig(LED_GPIO_PROFILING2_PIN_CONFIG);
}

//
// CCSI1 RX ISR
// Note: RX CLB will generate a receive interrupt when it has received 4 16-bit words.
//       It will also generate an END interrupt when it detects an end of frame. If a
//       a frame is less than 4 words, the RX CLB will generate an END interrupt, but
//       no receive interrupt.
//

//
__interrupt void INT_CCSI1_RX_TILE_ISR(void)
{
    uint32_t chkIntFlag = CLB_getRegister(CCSI1_RX_TILE_BASE, CLB_REG_HLC_R1);
//    uint32_t rcvIntFlag = CLB_getRegister(CCSI1_RX_TILE_BASE, CLB_REG_HLC_R2);
//    uint32_t endIntFlag = CLB_getRegister(CCSI1_RX_TILE_BASE, CLB_REG_HLC_R3);

    uint32_t clbPushFifoPtr = 0;
    uint32_t clbPushFifoData;
    uint16_t i;

    #if 0
    LED_HAL_setProfilingGPIO1();
    #endif

    // Check for any data in FIFOs
    clbPushFifoPtr = (HWREG(CCSI1_RX_TILE_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) >> 16) & 0x00FF;

    //
    // Reset the PUSH FIFO pointer
    // Note: The register CLB_O_BUF_PTR contains both PUSH and PULL FIFO pointers.
    //       However PULL FIFO pointer is not used, so it's okay to overwrite it.
    //
    HWREG(CCSI1_RX_TILE_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;

    // Check for overrun
    if (clbPushFifoPtr > 4)
    {
        // TODO: Figure out what to do here
        ESTOP0;
    }
    else
    {
        // Copy data (if any) from FIFO to receive buffer
        for (i=0; i<clbPushFifoPtr; i++)
        {
            clbPushFifoData = HWREG(CCSI1_RX_TILE_BASE + CLB_DATAEXCH + CLB_O_PUSH(i));
            LED_HAL_clbRxRingBuffWrite(CCSIBUS1, &clbPushFifoData);
        }
    }

//    // Receive interrupt
//    if (rcvIntFlag & 0x1)
//    {
//        // Clear the receive interrupt flag register
//        CLB_writeInterface(CCSI1_RX_TILE_BASE, CLB_ADDR_HLC_R2, 0x0);
//    }
//
//    // End interrupt
//    if (endIntFlag & 0x1)
//    {
//        // Clear the end interrupt flag register
//        CLB_writeInterface(CCSI1_RX_TILE_BASE, CLB_ADDR_HLC_R3, 0x0);
//    }

    // Check error interrupt
    if (chkIntFlag & 0x1)
    {
        // TODO: Figure out what to do here

        // Clear the check error interrupt flag register
        CLB_writeInterface(CCSI1_RX_TILE_BASE, CLB_ADDR_HLC_R1, 0x0);
    }

    Interrupt_clearACKGroup(INT_CCSI1_RX_TILE_INTERRUPT_ACK_GROUP);

    #if 0
    LED_HAL_resetProfilingGPIO1();
    #endif
}


//
// CCSI1 TX ISR
//
__interrupt void INT_CCSI1_TX_TILE_ISR(void)
{
    uint32_t txIntFlag, doneIntFlag;
    volatile uint16_t xmtXferSizeLocal = xmtXferSize[CCSIBUS1];
    volatile uint16_t xmtCountLocal = xmtCount[CCSIBUS1];
    volatile uint32_t *xmtBufPtrLocal = xmtBufPtr[CCSIBUS1];

    // Read CLB status flags
    txIntFlag = CLB_getRegister(CCSI1_TX_TILE_BASE, CLB_REG_HLC_R3);

    // Transmit interrupt
    if(txIntFlag & 0x1)
    {
        if(xmtCountLocal < xmtXferSizeLocal) {
            HWREG(CCSI1_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(0)) =
                    *(xmtBufPtrLocal + xmtCountLocal++);

            // Clear the flag register
            CLB_writeInterface(CCSI1_TX_TILE_BASE, CLB_ADDR_HLC_R3, 0x0);

            if(xmtCountLocal < xmtXferSizeLocal) {
                HWREG(CCSI1_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(1)) =
                        *(xmtBufPtrLocal + xmtCountLocal++);

                if(xmtCountLocal < xmtXferSizeLocal) {
                    HWREG(CCSI1_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(2)) =
                            *(xmtBufPtrLocal + xmtCountLocal++);

                    if(xmtCountLocal < xmtXferSizeLocal) {
                        HWREG(CCSI1_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(3)) =
                                *(xmtBufPtrLocal + xmtCountLocal++);
                    }
                }
            }

            xmtCount[CCSIBUS1] = xmtCountLocal;
        }
    }
    doneIntFlag = CLB_getRegister(CCSI1_TX_TILE_BASE, CLB_REG_HLC_R2);

    // Signal to CPU when CLB is done transmitting payload
    if (doneIntFlag & 0x1)
    {
        // Put TX CLB in idle
        CLB_setGPREG(CCSI1_TX_TILE_BASE, 0U);

        // Clear done flag
        CLB_writeInterface(CCSI1_TX_TILE_BASE, CLB_ADDR_HLC_R2, 0x0);

        // Signal to application the transfer is done
        xmtDone[CCSIBUS1] = 1;

        #if 0
        LED_HAL_resetProfilingGPIO1();
        #endif
    }

    Interrupt_clearACKGroup(INT_CCSI1_TX_TILE_INTERRUPT_ACK_GROUP);
}

#ifdef INT_CCSI2_RX_TILE
//
// CCSI2 RX ISR
// Note: RX CLB will generate a receive interrupt when it has received 4 16-bit words.
//       It will also generate an END interrupt when it detects an end of frame. If a
//       a frame is less than 4 words, the RX CLB will generate an END interrupt, but
//       no receive interrupt.
//

//
__interrupt void INT_CCSI2_RX_TILE_ISR(void)
{
    uint32_t chkIntFlag = CLB_getRegister(CCSI2_RX_TILE_BASE, CLB_REG_HLC_R1);
//    uint32_t rcvIntFlag = CLB_getRegister(CCSI2_RX_TILE_BASE, CLB_REG_HLC_R2);
//    uint32_t endIntFlag = CLB_getRegister(CCSI2_RX_TILE_BASE, CLB_REG_HLC_R3);

    uint32_t clbPushFifoPtr = 0;
    uint32_t clbPushFifoData;
    uint16_t i;

    // Check for any data in FIFOs
    clbPushFifoPtr = (HWREG(CCSI2_RX_TILE_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) >> 16) & 0x00FF;

    //
    // Reset the PUSH FIFO pointer
    // Note: The register CLB_O_BUF_PTR contains both PUSH and PULL FIFO pointers.
    //       However PULL FIFO pointer is not used, so it's okay to overwrite it.
    //
    HWREG(CCSI2_RX_TILE_BASE + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;

    // Check for overrun
    if (clbPushFifoPtr > 4)
    {
        // TODO: Figure out what to do here
        ESTOP0;
    }
    else
    {
        // Copy data (if any) from FIFO to receive buffer
        for (i=0; i<clbPushFifoPtr; i++)
        {
            clbPushFifoData = HWREG(CCSI2_RX_TILE_BASE + CLB_DATAEXCH + CLB_O_PUSH(i));
            LED_HAL_clbRxRingBuffWrite(CCSIBUS2, &clbPushFifoData);
        }
    }

//    // Receive interrupt
//    if (rcvIntFlag & 0x1)
//    {
//        // Clear the receive interrupt flag register
//        CLB_writeInterface(CCSI2_RX_TILE_BASE, CLB_ADDR_HLC_R2, 0x0);
//    }
//
//    // End interrupt
//    if (endIntFlag & 0x1)
//    {
//        // Clear the end interrupt flag register
//        CLB_writeInterface(CCSI2_RX_TILE_BASE, CLB_ADDR_HLC_R3, 0x0);
//    }

    // Check error interrupt
    if (chkIntFlag & 0x1)
    {
        // TODO: Figure out what to do here

        // Clear the check error interrupt flag register
        CLB_writeInterface(CCSI2_RX_TILE_BASE, CLB_ADDR_HLC_R1, 0x0);
    }

    Interrupt_clearACKGroup(INT_CCSI2_RX_TILE_INTERRUPT_ACK_GROUP);
}
#endif

#ifdef CCSI2_TX_TILE_BASE
//
// CCSI2 TX ISR
//
__interrupt void INT_CCSI2_TX_TILE_ISR(void)
{
    uint32_t txIntFlag, doneIntFlag;
    volatile uint16_t xmtXferSizeLocal = xmtXferSize[CCSIBUS2];
    volatile uint16_t xmtCountLocal = xmtCount[CCSIBUS2];
    volatile uint32_t *xmtBufPtrLocal = xmtBufPtr[CCSIBUS2];

    // Read CLB status flags
    txIntFlag = CLB_getRegister(CCSI2_TX_TILE_BASE, CLB_REG_HLC_R3);

    // Transmit interrupt
    if(txIntFlag & 0x1)
    {
        if(xmtCountLocal < xmtXferSizeLocal) {
            HWREG(CCSI2_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(0)) =
                    *(xmtBufPtrLocal + xmtCountLocal++);

            // Clear the flag register
            CLB_writeInterface(CCSI2_TX_TILE_BASE, CLB_ADDR_HLC_R3, 0x0);

            if(xmtCountLocal < xmtXferSizeLocal) {
                HWREG(CCSI2_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(1)) =
                        *(xmtBufPtrLocal + xmtCountLocal++);

                if(xmtCountLocal < xmtXferSizeLocal) {
                    HWREG(CCSI2_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(2)) =
                            *(xmtBufPtrLocal + xmtCountLocal++);

                    if(xmtCountLocal < xmtXferSizeLocal) {
                        HWREG(CCSI2_TX_TILE_BASE + CLB_DATAEXCH + CLB_O_PULL(3)) =
                                *(xmtBufPtrLocal + xmtCountLocal++);
                    }
                }
            }

            xmtCount[CCSIBUS2] = xmtCountLocal;
        }
    }
    doneIntFlag = CLB_getRegister(CCSI2_TX_TILE_BASE, CLB_REG_HLC_R2);

    // Signal to CPU when CLB is done transmitting payload
    if (doneIntFlag & 0x1)
    {
        // Put TX CLB in idle
        CLB_setGPREG(CCSI2_TX_TILE_BASE, 0U);

        // Clear done flag
        CLB_writeInterface(CCSI2_TX_TILE_BASE, CLB_ADDR_HLC_R2, 0x0);

        // Signal to application the transfer is done
        xmtDone[CCSIBUS2] = 1;

        #if 0
        LED_HAL_resetProfilingGPIO2();
        #endif
    }

    Interrupt_clearACKGroup(INT_CCSI2_TX_TILE_INTERRUPT_ACK_GROUP);
}
#endif
