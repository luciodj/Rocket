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

// get/set CCP1 aliases
#define CCP1_TRIS               TRISC0
#define CCP1_LAT                LATC0
#define CCP1_PORT               RC0
#define CCP1_ANS                ANSC0
#define CCP1_SetHigh()    do { LATC0 = 1; } while(0)
#define CCP1_SetLow()   do { LATC0 = 0; } while(0)
#define CCP1_Toggle()   do { LATC0 = ~LATC0; } while(0)
#define CCP1_GetValue()         RC0
#define CCP1_SetDigitalInput()    do { TRISC0 = 1; } while(0)
#define CCP1_SetDigitalOutput()   do { TRISC0 = 0; } while(0)

#define CCP1_SetAnalogMode()   do { ANSC0 = 1; } while(0)
#define CCP1_SetDigitalMode()   do { ANSC0 = 0; } while(0)
// get/set CCP2 aliases
#define CCP2_TRIS               TRISC1
#define CCP2_LAT                LATC1
#define CCP2_PORT               RC1
#define CCP2_ANS                ANSC1
#define CCP2_SetHigh()    do { LATC1 = 1; } while(0)
#define CCP2_SetLow()   do { LATC1 = 0; } while(0)
#define CCP2_Toggle()   do { LATC1 = ~LATC1; } while(0)
#define CCP2_GetValue()         RC1
#define CCP2_SetDigitalInput()    do { TRISC1 = 1; } while(0)
#define CCP2_SetDigitalOutput()   do { TRISC1 = 0; } while(0)

#define CCP2_SetAnalogMode()   do { ANSC1 = 1; } while(0)
#define CCP2_SetDigitalMode()   do { ANSC1 = 0; } while(0)
// get/set PWM3 aliases
#define PWM3_TRIS               TRISC2
#define PWM3_LAT                LATC2
#define PWM3_PORT               RC2
#define PWM3_ANS                ANSC2
#define PWM3_SetHigh()    do { LATC2 = 1; } while(0)
#define PWM3_SetLow()   do { LATC2 = 0; } while(0)
#define PWM3_Toggle()   do { LATC2 = ~LATC2; } while(0)
#define PWM3_GetValue()         RC2
#define PWM3_SetDigitalInput()    do { TRISC2 = 1; } while(0)
#define PWM3_SetDigitalOutput()   do { TRISC2 = 0; } while(0)

#define PWM3_SetAnalogMode()   do { ANSC2 = 1; } while(0)
#define PWM3_SetDigitalMode()   do { ANSC2 = 0; } while(0)
// get/set PWM4 aliases
#define PWM4_TRIS               TRISC3
#define PWM4_LAT                LATC3
#define PWM4_PORT               RC3
#define PWM4_ANS                ANSC3
#define PWM4_SetHigh()    do { LATC3 = 1; } while(0)
#define PWM4_SetLow()   do { LATC3 = 0; } while(0)
#define PWM4_Toggle()   do { LATC3 = ~LATC3; } while(0)
#define PWM4_GetValue()         RC3
#define PWM4_SetDigitalInput()    do { TRISC3 = 1; } while(0)
#define PWM4_SetDigitalOutput()   do { TRISC3 = 0; } while(0)

#define PWM4_SetAnalogMode()   do { ANSC3 = 1; } while(0)
#define PWM4_SetDigitalMode()   do { ANSC3 = 0; } while(0)
// get/set CLC1OUT aliases
#define CLC1OUT_TRIS               TRISC4
#define CLC1OUT_LAT                LATC4
#define CLC1OUT_PORT               RC4
#define CLC1OUT_WPU                WPUC4
#define CLC1OUT_SetHigh()    do { LATC4 = 1; } while(0)
#define CLC1OUT_SetLow()   do { LATC4 = 0; } while(0)
#define CLC1OUT_Toggle()   do { LATC4 = ~LATC4; } while(0)
#define CLC1OUT_GetValue()         RC4
#define CLC1OUT_SetDigitalInput()    do { TRISC4 = 1; } while(0)
#define CLC1OUT_SetDigitalOutput()   do { TRISC4 = 0; } while(0)

#define CLC1OUT_SetPullup()    do { WPUC4 = 1; } while(0)
#define CLC1OUT_ResetPullup()   do { WPUC4 = 0; } while(0)
// get/set CLC2OUT aliases
#define CLC2OUT_TRIS               TRISC5
#define CLC2OUT_LAT                LATC5
#define CLC2OUT_PORT               RC5
#define CLC2OUT_WPU                WPUC5
#define CLC2OUT_SetHigh()    do { LATC5 = 1; } while(0)
#define CLC2OUT_SetLow()   do { LATC5 = 0; } while(0)
#define CLC2OUT_Toggle()   do { LATC5 = ~LATC5; } while(0)
#define CLC2OUT_GetValue()         RC5
#define CLC2OUT_SetDigitalInput()    do { TRISC5 = 1; } while(0)
#define CLC2OUT_SetDigitalOutput()   do { TRISC5 = 0; } while(0)

#define CLC2OUT_SetPullup()    do { WPUC5 = 1; } while(0)
#define CLC2OUT_ResetPullup()   do { WPUC5 = 0; } while(0)
// get/set CLC3OUT aliases
#define CLC3OUT_TRIS               TRISC6
#define CLC3OUT_LAT                LATC6
#define CLC3OUT_PORT               RC6
#define CLC3OUT_ANS                ANSC6
#define CLC3OUT_SetHigh()    do { LATC6 = 1; } while(0)
#define CLC3OUT_SetLow()   do { LATC6 = 0; } while(0)
#define CLC3OUT_Toggle()   do { LATC6 = ~LATC6; } while(0)
#define CLC3OUT_GetValue()         RC6
#define CLC3OUT_SetDigitalInput()    do { TRISC6 = 1; } while(0)
#define CLC3OUT_SetDigitalOutput()   do { TRISC6 = 0; } while(0)

#define CLC3OUT_SetAnalogMode()   do { ANSC6 = 1; } while(0)
#define CLC3OUT_SetDigitalMode()   do { ANSC6 = 0; } while(0)
// get/set CLC4OUT aliases
#define CLC4OUT_TRIS               TRISC7
#define CLC4OUT_LAT                LATC7
#define CLC4OUT_PORT               RC7
#define CLC4OUT_ANS                ANSC7
#define CLC4OUT_SetHigh()    do { LATC7 = 1; } while(0)
#define CLC4OUT_SetLow()   do { LATC7 = 0; } while(0)
#define CLC4OUT_Toggle()   do { LATC7 = ~LATC7; } while(0)
#define CLC4OUT_GetValue()         RC7
#define CLC4OUT_SetDigitalInput()    do { TRISC7 = 1; } while(0)
#define CLC4OUT_SetDigitalOutput()   do { TRISC7 = 0; } while(0)

#define CLC4OUT_SetAnalogMode()   do { ANSC7 = 1; } while(0)
#define CLC4OUT_SetDigitalMode()   do { ANSC7 = 0; } while(0)

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