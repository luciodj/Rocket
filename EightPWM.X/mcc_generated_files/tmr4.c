/**
  TMR4 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated driver implementation file for the TMR4 driver using MPLAB� Code Configurator

  @Description
    This source file provides APIs for TMR4.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  1.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

/**
  Section: Included Files
 */

#include <xc.h>
#include "tmr4.h"

/**
  Section: TMR4 APIs
 */

void TMR4_Initialize(void) {
    // Set TMR4 to the options selected in the User Interface

    // TMR4ON off; T4CKPS 1:64; T4OUTPS 1:1; 
    T4CON = 0x60;

    // T4CS FOSC/4; 
    T4CLKCON = 0x00;

    // T4CKSYNC Not Synchronized; T4MODE Software control; T4PSYNC Not Synchronized; T4CKPOL Rising Edge; 
    T4HLT = 0x00;

    // T4RSEL TMR2_Postscaled; 
    T4RST = 0x05;

    // T4PR 3; 
    T4PR = 0x03;

    // TMR4 0x0; 
    T4TMR = 0x00;

    // Clearing IF flag.
    PIR2bits.TMR4IF = 0;

    // Start TMR4
    TMR4_Start();
}

void TMR4_ModeSet(TMR4_HLT_MODE mode) {
    // Configure different types HLT mode
    T4HLTbits.MODE = mode;
}

void TMR4_ExtResetSourceSet(TMR4_HLT_EXT_RESET_SOURCE reset) {
    //Configure different types of HLT external reset source
    T4RSTbits.RSEL = reset;
}

void TMR4_Start(void) {
    // Start the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 1;
}

void TMR4_Stop(void) {
    // Stop the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 0;
}

uint8_t TMR4_Counter8BitGet(void) {
    uint8_t readVal;

    readVal = TMR4;

    return readVal;
}

void TMR4_Counter8BitSet(uint8_t timerVal) {
    // Write to the Timer4 register
    TMR4 = timerVal;
}

void TMR4_Period8BitSet(uint8_t periodVal) {
    PR4 = periodVal;
}

bool TMR4_HasOverflowOccured(void) {
    // check if  overflow has occurred by checking the TMRIF bit
    bool status = PIR2bits.TMR4IF;
    if (status) {
        // Clearing IF flag.
        PIR2bits.TMR4IF = 0;
    }
    return status;
}
/**
  End of File
 */