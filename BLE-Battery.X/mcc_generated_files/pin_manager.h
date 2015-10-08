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

// get/set LED aliases
#define LED_TRIS               TRISA2
#define LED_LAT                LATA2
#define LED_PORT               RA2
#define LED_ANS                ANSA2
#define LED_SetHigh()    do { LATA2 = 1; } while(0)
#define LED_SetLow()   do { LATA2 = 0; } while(0)
#define LED_Toggle()   do { LATA2 = ~LATA2; } while(0)
#define LED_GetValue()         RA2
#define LED_SetDigitalInput()    do { TRISA2 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISA2 = 0; } while(0)

#define LED_SetAnalogMode()   do { ANSA2 = 1; } while(0)
#define LED_SetDigitalMode()   do { ANSA2 = 0; } while(0)
// get/set WAKE_SW aliases
#define WAKE_SW_TRIS               TRISA4
#define WAKE_SW_LAT                LATA4
#define WAKE_SW_PORT               RA4
#define WAKE_SW_ANS                ANSA4
#define WAKE_SW_SetHigh()    do { LATA4 = 1; } while(0)
#define WAKE_SW_SetLow()   do { LATA4 = 0; } while(0)
#define WAKE_SW_Toggle()   do { LATA4 = ~LATA4; } while(0)
#define WAKE_SW_GetValue()         RA4
#define WAKE_SW_SetDigitalInput()    do { TRISA4 = 1; } while(0)
#define WAKE_SW_SetDigitalOutput()   do { TRISA4 = 0; } while(0)

#define WAKE_SW_SetAnalogMode()   do { ANSA4 = 1; } while(0)
#define WAKE_SW_SetDigitalMode()   do { ANSA4 = 0; } while(0)
// get/set RX aliases
#define RX_TRIS               TRISB5
#define RX_LAT                LATB5
#define RX_PORT               RB5
#define RX_ANS                ANSB5
#define RX_SetHigh()    do { LATB5 = 1; } while(0)
#define RX_SetLow()   do { LATB5 = 0; } while(0)
#define RX_Toggle()   do { LATB5 = ~LATB5; } while(0)
#define RX_GetValue()         RB5
#define RX_SetDigitalInput()    do { TRISB5 = 1; } while(0)
#define RX_SetDigitalOutput()   do { TRISB5 = 0; } while(0)

#define RX_SetAnalogMode()   do { ANSB5 = 1; } while(0)
#define RX_SetDigitalMode()   do { ANSB5 = 0; } while(0)
// get/set TX aliases
#define TX_TRIS               TRISB7
#define TX_LAT                LATB7
#define TX_PORT               RB7
#define TX_WPU                WPUB7
#define TX_SetHigh()    do { LATB7 = 1; } while(0)
#define TX_SetLow()   do { LATB7 = 0; } while(0)
#define TX_Toggle()   do { LATB7 = ~LATB7; } while(0)
#define TX_GetValue()         RB7
#define TX_SetDigitalInput()    do { TRISB7 = 1; } while(0)
#define TX_SetDigitalOutput()   do { TRISB7 = 0; } while(0)

#define TX_SetPullup()    do { WPUB7 = 1; } while(0)
#define TX_ResetPullup()   do { WPUB7 = 0; } while(0)
// get/set POT aliases
#define POT_TRIS               TRISC0
#define POT_LAT                LATC0
#define POT_PORT               RC0
#define POT_ANS                ANSC0
#define POT_SetHigh()    do { LATC0 = 1; } while(0)
#define POT_SetLow()   do { LATC0 = 0; } while(0)
#define POT_Toggle()   do { LATC0 = ~LATC0; } while(0)
#define POT_GetValue()         RC0
#define POT_SetDigitalInput()    do { TRISC0 = 1; } while(0)
#define POT_SetDigitalOutput()   do { TRISC0 = 0; } while(0)

#define POT_SetAnalogMode()   do { ANSC0 = 1; } while(0)
#define POT_SetDigitalMode()   do { ANSC0 = 0; } while(0)
// get/set CONN aliases
#define CONN_TRIS               TRISC2
#define CONN_LAT                LATC2
#define CONN_PORT               RC2
#define CONN_ANS                ANSC2
#define CONN_SetHigh()    do { LATC2 = 1; } while(0)
#define CONN_SetLow()   do { LATC2 = 0; } while(0)
#define CONN_Toggle()   do { LATC2 = ~LATC2; } while(0)
#define CONN_GetValue()         RC2
#define CONN_SetDigitalInput()    do { TRISC2 = 1; } while(0)
#define CONN_SetDigitalOutput()   do { TRISC2 = 0; } while(0)

#define CONN_SetAnalogMode()   do { ANSC2 = 1; } while(0)
#define CONN_SetDigitalMode()   do { ANSC2 = 0; } while(0)
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
// get/set CMD aliases
#define CMD_TRIS               TRISC5
#define CMD_LAT                LATC5
#define CMD_PORT               RC5
#define CMD_WPU                WPUC5
#define CMD_SetHigh()    do { LATC5 = 1; } while(0)
#define CMD_SetLow()   do { LATC5 = 0; } while(0)
#define CMD_Toggle()   do { LATC5 = ~LATC5; } while(0)
#define CMD_GetValue()         RC5
#define CMD_SetDigitalInput()    do { TRISC5 = 1; } while(0)
#define CMD_SetDigitalOutput()   do { TRISC5 = 0; } while(0)

#define CMD_SetPullup()    do { WPUC5 = 1; } while(0)
#define CMD_ResetPullup()   do { WPUC5 = 0; } while(0)
// get/set PWM4 aliases
#define PWM4_TRIS               TRISC7
#define PWM4_LAT                LATC7
#define PWM4_PORT               RC7
#define PWM4_ANS                ANSC7
#define PWM4_SetHigh()    do { LATC7 = 1; } while(0)
#define PWM4_SetLow()   do { LATC7 = 0; } while(0)
#define PWM4_Toggle()   do { LATC7 = ~LATC7; } while(0)
#define PWM4_GetValue()         RC7
#define PWM4_SetDigitalInput()    do { TRISC7 = 1; } while(0)
#define PWM4_SetDigitalOutput()   do { TRISC7 = 0; } while(0)

#define PWM4_SetAnalogMode()   do { ANSC7 = 1; } while(0)
#define PWM4_SetDigitalMode()   do { ANSC7 = 0; } while(0)

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