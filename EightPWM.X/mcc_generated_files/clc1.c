/**
  CLC1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    clc1.c

  @Summary
    This is the generated driver implementation file for the CLC1 driver using MPLAB� Code Configurator

  @Description
    This source file provides implementations for driver APIs for CLC1.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  2.00
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
#include "clc1.h"

/**
  Section: CLC1 APIs
 */

void CLC1_Initialize(void) {
    // Set the CLC1 to the options selected in the User Interface

    // LC1G2POL not_inverted; LC1G1POL not_inverted; LC1POL not_inverted; LC1G3POL not_inverted; LC1G4POL not_inverted; 
    CLC1POL = 0x00;

    // LC1D1S T2_postscaled_out; 
    CLC1SEL0 = 0x18;

    // LC1D2S CLCIN0 (CLCIN0PPS); 
    CLC1SEL1 = 0x00;

    // LC1D3S T4_postscaled_out; 
    CLC1SEL2 = 0x1A;

    // LC1D4S CLCIN0 (CLCIN0PPS); 
    CLC1SEL3 = 0x00;

    // LC1G1D4N disabled; LC1G1D3N disabled; LC1G1D2N disabled; LC1G1D1N disabled; LC1G1D4T disabled; LC1G1D3T disabled; LC1G1D2T disabled; LC1G1D1T enabled; 
    CLC1GLS0 = 0x02;

    // LC1G2D4N disabled; LC1G2D3N disabled; LC1G2D2N disabled; LC1G2D1N disabled; LC1G2D4T disabled; LC1G2D3T disabled; LC1G2D2T disabled; LC1G2D1T disabled; 
    CLC1GLS1 = 0x00;

    // LC1G3D3N disabled; LC1G3D4N disabled; LC1G3D1N disabled; LC1G3D2N disabled; LC1G3D3T enabled; LC1G3D4T disabled; LC1G3D1T disabled; LC1G3D2T disabled; 
    CLC1GLS2 = 0x20;

    // LC1G4D2N disabled; LC1G4D3N disabled; LC1G4D1N disabled; LC1G4D4T disabled; LC1G4D2T disabled; LC1G4D3T disabled; LC1G4D1T disabled; LC1G4D4N disabled; 
    CLC1GLS3 = 0x00;

    // INTP disabled; LC1OUT disabled; MODE SR latch; INTN disabled; LC1EN enabled; 
    CLC1CON = 0x83;

}

bool CLC1_OutputStatusGet(void) {
    return (CLC1CONbits.LC1OUT);

}
/**
 End of File
 */