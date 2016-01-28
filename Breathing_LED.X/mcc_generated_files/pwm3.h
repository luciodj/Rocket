/**
  PWM3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm3.c

  @Summary
    This is the generated driver implementation file for the PWM3 driver using MPLAB® Code Configurator

  @Description
    This header file provides implementations for driver APIs for PWM3.
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

#ifndef _PWM3_H
#define _PWM3_H

/**
  Section: Included Files
 */

#include <xc.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

    /**
      Section: Macro Declarations
     */

#define PWM3_INITIALIZE_DUTY_VALUE    511

    /**
      Section: PWM Module APIs
     */

    /**
      @Summary
        Initializes the PWM3

      @Description
        This routine initializes the EPWM3_Initialize
        This routine must be called before any other PWM3 routine is called.
        This routine should only be called once during system initialization.

      @Preconditions
        None

      @Param
        None

      @Returns
        None

      @Comment
    

     @Example
        <code>
        uint16_t dutycycle;

        PWM3_Initialize();
        PWM3_LoadDutyValue(dutycycle);
        </code>
     */
    void PWM3_Initialize(void);

    /**
      @Summary
        Loads 16-bit duty cycle.

      @Description
        This routine loads the 16 bit duty cycle value.

      @Preconditions
        PWM3_Initialize() function should have been called 
             before calling this function.

      @Param
        Pass 16bit duty cycle value.

      @Returns
        None

      @Example
        <code>
        uint16_t dutycycle;

        PWM3_Initialize();
        PWM3_LoadDutyValue(dutycycle);
        </code>
     */
    void PWM3_LoadDutyValue(uint16_t dutyValue);

#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif	//PWM3_H
/**
 End of File
 */

