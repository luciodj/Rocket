/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB� Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Version           :  1.01
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set OLED_RST aliases
#define OLED_RST_TRIS               TRISA4
#define OLED_RST_LAT                LATA4
#define OLED_RST_PORT               RA4
#define OLED_RST_ANS                ANSA4
#define OLED_RST_SetHigh()    do { LATA4 = 1; } while(0)
#define OLED_RST_SetLow()   do { LATA4 = 0; } while(0)
#define OLED_RST_Toggle()   do { LATA4 = ~LATA4; } while(0)
#define OLED_RST_GetValue()         RA4
#define OLED_RST_SetDigitalInput()    do { TRISA4 = 1; } while(0)
#define OLED_RST_SetDigitalOutput()   do { TRISA4 = 0; } while(0)

#define OLED_RST_SetAnalogMode()   do { ANSA4 = 1; } while(0)
#define OLED_RST_SetDigitalMode()   do { ANSA4 = 0; } while(0)
// get/set SCK aliases
#define SCK_TRIS               TRISB6
#define SCK_LAT                LATB6
#define SCK_PORT               RB6
#define SCK_WPU                WPUB6
#define SCK_SetHigh()    do { LATB6 = 1; } while(0)
#define SCK_SetLow()   do { LATB6 = 0; } while(0)
#define SCK_Toggle()   do { LATB6 = ~LATB6; } while(0)
#define SCK_GetValue()         RB6
#define SCK_SetDigitalInput()    do { TRISB6 = 1; } while(0)
#define SCK_SetDigitalOutput()   do { TRISB6 = 0; } while(0)

#define SCK_SetPullup()    do { WPUB6 = 1; } while(0)
#define SCK_ResetPullup()   do { WPUB6 = 0; } while(0)
// get/set SW1 aliases
#define SW1_TRIS               TRISC4
#define SW1_LAT                LATC4
#define SW1_PORT               RC4
#define SW1_WPU                WPUC4
#define SW1_SetHigh()    do { LATC4 = 1; } while(0)
#define SW1_SetLow()   do { LATC4 = 0; } while(0)
#define SW1_Toggle()   do { LATC4 = ~LATC4; } while(0)
#define SW1_GetValue()         RC4
#define SW1_SetDigitalInput()    do { TRISC4 = 1; } while(0)
#define SW1_SetDigitalOutput()   do { TRISC4 = 0; } while(0)

#define SW1_SetPullup()    do { WPUC4 = 1; } while(0)
#define SW1_ResetPullup()   do { WPUC4 = 0; } while(0)
// get/set OLED_DC aliases
#define OLED_DC_TRIS               TRISC5
#define OLED_DC_LAT                LATC5
#define OLED_DC_PORT               RC5
#define OLED_DC_WPU                WPUC5
#define OLED_DC_SetHigh()    do { LATC5 = 1; } while(0)
#define OLED_DC_SetLow()   do { LATC5 = 0; } while(0)
#define OLED_DC_Toggle()   do { LATC5 = ~LATC5; } while(0)
#define OLED_DC_GetValue()         RC5
#define OLED_DC_SetDigitalInput()    do { TRISC5 = 1; } while(0)
#define OLED_DC_SetDigitalOutput()   do { TRISC5 = 0; } while(0)

#define OLED_DC_SetPullup()    do { WPUC5 = 1; } while(0)
#define OLED_DC_ResetPullup()   do { WPUC5 = 0; } while(0)
// get/set OLED_CS aliases
#define OLED_CS_TRIS               TRISC6
#define OLED_CS_LAT                LATC6
#define OLED_CS_PORT               RC6
#define OLED_CS_ANS                ANSC6
#define OLED_CS_SetHigh()    do { LATC6 = 1; } while(0)
#define OLED_CS_SetLow()   do { LATC6 = 0; } while(0)
#define OLED_CS_Toggle()   do { LATC6 = ~LATC6; } while(0)
#define OLED_CS_GetValue()         RC6
#define OLED_CS_SetDigitalInput()    do { TRISC6 = 1; } while(0)
#define OLED_CS_SetDigitalOutput()   do { TRISC6 = 0; } while(0)

#define OLED_CS_SetAnalogMode()   do { ANSC6 = 1; } while(0)
#define OLED_CS_SetDigitalMode()   do { ANSC6 = 0; } while(0)
// get/set SDO aliases
#define SDO_TRIS               TRISC7
#define SDO_LAT                LATC7
#define SDO_PORT               RC7
#define SDO_ANS                ANSC7
#define SDO_SetHigh()    do { LATC7 = 1; } while(0)
#define SDO_SetLow()   do { LATC7 = 0; } while(0)
#define SDO_Toggle()   do { LATC7 = ~LATC7; } while(0)
#define SDO_GetValue()         RC7
#define SDO_SetDigitalInput()    do { TRISC7 = 1; } while(0)
#define SDO_SetDigitalOutput()   do { TRISC7 = 0; } while(0)

#define SDO_SetAnalogMode()   do { ANSC7 = 1; } while(0)
#define SDO_SetDigitalMode()   do { ANSC7 = 0; } while(0)

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    GPIO and peripheral I/O initialization
 * @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize(void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);

#endif // PIN_MANAGER_H
/**
 End of File
 */