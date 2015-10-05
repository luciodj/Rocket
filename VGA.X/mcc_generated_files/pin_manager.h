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

// get/set VS aliases
#define VS_TRIS               TRISB4
#define VS_LAT                LATB4
#define VS_PORT               RB4
#define VS_ANS                ANSB4
#define VS_SetHigh()    do { LATB4 = 1; } while(0)
#define VS_SetLow()   do { LATB4 = 0; } while(0)
#define VS_Toggle()   do { LATB4 = ~LATB4; } while(0)
#define VS_GetValue()         RB4
#define VS_SetDigitalInput()    do { TRISB4 = 1; } while(0)
#define VS_SetDigitalOutput()   do { TRISB4 = 0; } while(0)

#define VS_SetAnalogMode()   do { ANSB4 = 1; } while(0)
#define VS_SetDigitalMode()   do { ANSB4 = 0; } while(0)
// get/set PWM4 aliases
#define PWM4_TRIS               TRISB5
#define PWM4_LAT                LATB5
#define PWM4_PORT               RB5
#define PWM4_ANS                ANSB5
#define PWM4_SetHigh()    do { LATB5 = 1; } while(0)
#define PWM4_SetLow()   do { LATB5 = 0; } while(0)
#define PWM4_Toggle()   do { LATB5 = ~LATB5; } while(0)
#define PWM4_GetValue()         RB5
#define PWM4_SetDigitalInput()    do { TRISB5 = 1; } while(0)
#define PWM4_SetDigitalOutput()   do { TRISB5 = 0; } while(0)

#define PWM4_SetAnalogMode()   do { ANSB5 = 1; } while(0)
#define PWM4_SetDigitalMode()   do { ANSB5 = 0; } while(0)
// get/set DT aliases
#define DT_TRIS               TRISB6
#define DT_LAT                LATB6
#define DT_PORT               RB6
#define DT_WPU                WPUB6
#define DT_SetHigh()    do { LATB6 = 1; } while(0)
#define DT_SetLow()   do { LATB6 = 0; } while(0)
#define DT_Toggle()   do { LATB6 = ~LATB6; } while(0)
#define DT_GetValue()         RB6
#define DT_SetDigitalInput()    do { TRISB6 = 1; } while(0)
#define DT_SetDigitalOutput()   do { TRISB6 = 0; } while(0)

#define DT_SetPullup()    do { WPUB6 = 1; } while(0)
#define DT_ResetPullup()   do { WPUB6 = 0; } while(0)
// get/set IO_RC2 aliases
#define IO_RC2_TRIS               TRISC2
#define IO_RC2_LAT                LATC2
#define IO_RC2_PORT               RC2
#define IO_RC2_ANS                ANSC2
#define IO_RC2_SetHigh()    do { LATC2 = 1; } while(0)
#define IO_RC2_SetLow()   do { LATC2 = 0; } while(0)
#define IO_RC2_Toggle()   do { LATC2 = ~LATC2; } while(0)
#define IO_RC2_GetValue()         RC2
#define IO_RC2_SetDigitalInput()    do { TRISC2 = 1; } while(0)
#define IO_RC2_SetDigitalOutput()   do { TRISC2 = 0; } while(0)

#define IO_RC2_SetAnalogMode()   do { ANSC2 = 1; } while(0)
#define IO_RC2_SetDigitalMode()   do { ANSC2 = 0; } while(0)

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