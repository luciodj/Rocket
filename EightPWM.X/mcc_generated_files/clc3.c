/**
  CLC3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    clc3.c

  @Summary
    This is the generated driver implementation file for the CLC3 driver using MPLAB® Code Configurator

  @Description
    This source file provides implementations for driver APIs for CLC3.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
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
#include "clc3.h"

/**
  Section: CLC3 APIs
 */

void CLC3_Initialize(void) {
    // Set the CLC3 to the options selected in the User Interface

    // LC3G2POL not_inverted; LC3G1POL not_inverted; LC3G3POL not_inverted; LC3G4POL not_inverted; LC3POL not_inverted; 
    CLC3POL = 0x00;

    // LC3D1S T2_postscaled_out; 
    CLC3SEL0 = 0x18;

    // LC3D2S CLCIN0 (CLCIN0PPS); 
    CLC3SEL1 = 0x00;

    // LC3D3S Angular TMR Comp1; 
    CLC3SEL2 = 0x10;

    // LC3D4S CLCIN0 (CLCIN0PPS); 
    CLC3SEL3 = 0x00;

    // LC3G1D1T enabled; LC3G1D2T disabled; LC3G1D3N disabled; LC3G1D4N disabled; LC3G1D1N disabled; LC3G1D2N disabled; LC3G1D3T disabled; LC3G1D4T disabled; 
    CLC3GLS0 = 0x02;

    // LC3G2D1T disabled; LC3G2D4N disabled; LC3G2D1N disabled; LC3G2D3N disabled; LC3G2D2N disabled; LC3G2D3T disabled; LC3G2D2T disabled; LC3G2D4T disabled; 
    CLC3GLS1 = 0x00;

    // LC3G3D4N disabled; LC3G3D3N disabled; LC3G3D2N disabled; LC3G3D1N disabled; LC3G3D2T disabled; LC3G3D1T disabled; LC3G3D4T disabled; LC3G3D3T enabled; 
    CLC3GLS2 = 0x20;

    // LC3G4D2N disabled; LC3G4D3N disabled; LC3G4D4N disabled; LC3G4D4T disabled; LC3G4D1N disabled; LC3G4D1T disabled; LC3G4D2T disabled; LC3G4D3T disabled; 
    CLC3GLS3 = 0x00;

    // INTP disabled; MODE SR latch; LC3OUT disabled; LC3EN enabled; INTN disabled; 
    CLC3CON = 0x83;

}

bool CLC3_OutputStatusGet(void) {
    return (CLC3CONbits.LC3OUT);

}
/**
 End of File
 */