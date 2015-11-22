
/**
  AT1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    at1.c

  @Summary
    This is the generated driver implementation file for the AT1 driver using MPLAB� Code Configurator

  @Description
    This header file provides implementations for driver APIs for AT1.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  1.0
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
#include "at1.h"

/**
  Section: AT1 APIs
 */

void AT1_Initialize(void) {
    // set the AT1 to the options selected in the User Interface
    // AT1CS HFINTOSC 16Mhz; 
    AT1CLK = 0x01;
    // AT1SSEL CLC1_out; 
    AT1SIG = 0x04;
    // AT1RES8 3; 
    AT1RESH = 0x03;
    // AT1RES0 254; 
    AT1RESL = 0xFE;
    // AT1MISS8 0; 
    AT1MISSH = 0x00;
    // AT1MISS0 1; 
    AT1MISSL = 0x01;
    // AT1MODE Single Pulse mode with missing pulse; AT1PREC not reset; AT1PS AT1CLK; AT1EN enabled; AT1POL rising edge; 
    AT1CON0 = 0x80;
    // AT1VALID Not valid; AT1PRP Active high; AT1PHP Active low; AT1ACCS Lower; AT1MPP Active high; 
    AT1CON1 = 0x40;
    // AT1PERIF Period Interrupt not occured; AT1MISSIF Missed Pulse Interrupt not occured; AT1PHSIF Angle Interrupt not occured; 
    AT1IR0 = 0x00;
    // AT1MISSIE disabled; AT1PERIE disabled; AT1PHSIE disabled; 
    AT1IE0 = 0x00;
    // AT1CC1IF Not occured; AT1CC2IF Not occured; AT1CC3IF Not occured; 
    AT1IR1 = 0x00;
    // AT1CC2IE disabled; AT1CC3IE disabled; AT1CC1IE disabled; 
    AT1IE1 = 0x00;
    // AT1STPT8 0; 
    AT1STPTH = 0x00;
    // AT1STPT0 1; 
    AT1STPTL = 0x01;
    // AT1CC18 2; 
    AT1CC1H = 0x02;
    // AT1CC10 188; 
    AT1CC1L = 0xBC;
    // AT1CC1MODE compare mode; AT1CAP1P rising edge; AT1CC1EN enabled; AT1CC1POL Active high; 
    AT1CCON1 = 0x80;
    // AT1CP1S AT1CAP pin; 
    AT1CSEL1 = 0x00;
    // AT1CC28 3; 
    AT1CC2H = 0x03;
    // AT1CC20 32; 
    AT1CC2L = 0x20;
    // AT1CAP2P rising edge; AT1CC2POL Active high; AT1CC2EN enabled; AT1CC2MODE compare mode; 
    AT1CCON2 = 0x80;
    // AT1CP2S AT1CAP pin; 
    AT1CSEL2 = 0x00;
    // AT1CC38 3; 
    AT1CC3H = 0x03;
    // AT1CC30 132; 
    AT1CC3L = 0x84;
    // AT1CC3MODE compare mode; AT1CAP3P rising edge; AT1CC3POL Active high; AT1CC3EN enabled; 
    AT1CCON3 = 0x80;
    // AT1CP3S AT1CAP pin; 
    AT1CSEL3 = 0x00;

}

void AT1_ResolutionSet(uint16_t resolution) {
    AT1RESH = ((resolution & 0x0300) >> 8); //writing 2 MSBs to AT1RESH register
    AT1RESL = (resolution & 0x00FF); //writing 8 LSBs to AT1RESL register	
}

void AT1_MissingPulseDelaySet(int16_t missingPulse) {
    AT1MISSH = ((missingPulse & 0xFF00) >> 8); //writing 8 MSBs to AT1MISSH register
    AT1MISSL = (missingPulse & 0x00FF); //writing 8 LSBs to AT1MISSL register
}

void AT1_SetPointLoad(uint16_t thresholdPeriod) {
    AT1STPTH = ((thresholdPeriod & 0x7F00) >> 8); //writing 7 MSBs to AT1STPTH register
    AT1STPTL = (thresholdPeriod & 0x00FF); //writing 8 LSBs to AT1STPTL register
}

uint16_t AT1_PeriodGet(void) {
    return ((AT1PERH << 8) | AT1PERL); //return 15 bit (Period) AT1PER register
}

uint16_t AT1_PhaseGet(void) {
    return ((AT1PHSH << 8) | AT1PHSL); //return 10 bit (Phase)AT1PHS register
}

int16_t AT1_SetPointErrorGet(void) {
    return ((AT1ERRH << 8) | AT1ERRL); //return 16 bit AT1ERR register
}

bool AT1_CheckPeriodValue(void) {
    return (AT1CON1bits.AT1ACCS);
}

bool AT1_IsMeasurementValid(void) {
    return (AT1CON1bits.AT1VALID);
}

bool AT1_IsPeriodCounterOverflowOccured(void) {
    return (AT1PERHbits.AT1POV);
}

bool AT1_IsPeriodCountAvailable(void) {
    return (AT1IR0bits.AT1PERIF);
}

bool AT1_IsPhaseCountAvailable(void) {
    return (AT1IR0bits.AT1PHSIF);
}

bool AT1_IsMissedPulseCountAvailable(void) {
    return (AT1IR0bits.AT1MISSIF);
}

void AT1_CC1_Compare_SetCount(uint16_t compareCount) {
    AT1CC1H = ((compareCount & 0x0300) >> 8);
    AT1CC1L = (compareCount & 0x00FF);
}

bool AT1_CC1_Compare_IsMatchOccured(void) {
    return (AT1IR1bits.AT1CC1IF);
}

void AT1_CC2_Compare_SetCount(uint16_t compareCount) {
    AT1CC2H = ((compareCount & 0x0300) >> 8);
    AT1CC2L = (compareCount & 0x00FF);
}

bool AT1_CC2_Compare_IsMatchOccured(void) {
    return (AT1IR1bits.AT1CC2IF);
}

void AT1_CC3_Compare_SetCount(uint16_t compareCount) {
    AT1CC3H = ((compareCount & 0x0300) >> 8);
    AT1CC3L = (compareCount & 0x00FF);
}

bool AT1_CC3_Compare_IsMatchOccured(void) {
    return (AT1IR1bits.AT1CC3IF);
}


/**
 End of File
 */
