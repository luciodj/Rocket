/**
  8x32 NeoPixel driver for Curiosity

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
 Test code for the Adafruit 8x32 Neopixel display

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
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

#include "NeoPixel.h"


/*
                         Main application
 */
void main(void)
{
    uint8_t delay = 0;
    uint8_t index = 0;
    
    // initialize the device
    SYSTEM_Initialize();
    NeoPixel_Clear(NEO_BLACK);  
//    NeoPixel_Gradient( 0x000000, 0x000100);
    NeoPixel_Puts(0, "XPRESS", NEO_BLACK);
//    NeoPixel_Show();
//    while(1);
    NeoPixel_Scroll(" ", NEO_GREEN, true);
    
    while (1)
    {
        if ( TMR2_HasOverflowOccured())     // 10ms
        {   
            if ( delay > 0) {
                delay--;
            } else {
                NeoPixel_Scroll("  ** Express ** ", NEO_RED, false);
                delay = 2;
            }
        }
    }
}

/**
 End of File
 */