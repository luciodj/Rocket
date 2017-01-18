/**
  Generated Main Source File

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v3.26
        Device            :  PIC16F1619
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.40
        MPLAB             :  MPLAB X 3.50
 */

/*
Copyright (c) 2013 - 2014 released Microchip Technology Inc.  All rights reserved.

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

#include "mcc_generated_files/mcc.h"

#define TCLK            _XTAL_FREQ   // 8 MHz, defined in mcc.h
#define PERIOD          (unsigned)(TCLK /64 / 4 /125)     // 8ms = 125Hz
#define DCMAX           (unsigned)(PERIOD * 4)
#define SERVO_MIDDLE    (unsigned)(DCMAX * 14/80)         // 1.4ms
#define SERVO_MIN       (unsigned)(DCMAX *  4/80)         // 0.4ms
#define SERVO_MAX       (unsigned)(DCMAX * 24/80)         // 2.4ms

void main(void) {
    // configure ADC to trigger from Timer2 and generate an interrupt
    // configure PWM1 for an 8ms period, output on pin RC5
    SYSTEM_Initialize();

    // Enable Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1) {
    }
}

// remove or comment out default ISR generated in adc.c
// read potentiometer value and translate to servo angle
//
void ADC_ISR(void) {
    uint16_t duty;
    duty = SERVO_MIN + (ADC_GetConversion(Potentiometer) >> 2);
    if (duty > SERVO_MAX)
        duty = SERVO_MAX;
    PWM1_LoadDutyValue(duty);

    // Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
}
/**
 End of File
 */