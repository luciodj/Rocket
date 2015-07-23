/**
  CCP1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp1.h

  @Summary
    This is the generated driver implementation file for the CCP1 driver using MPLAB� Code Configurator

  @Description
    This header file provides implementations for driver APIs for CCP1.
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

#ifndef _PWM1_H
#define _PWM1_H

/**
  Section: Included Files
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

    /**
      Section: PWM Module APIs
     */

    /**
      @Summary
        Initializes the CCP1

      @Description
        This routine initializes the CCP1 module.
        This routine must be called before any other CCP1 routine is called.
        This routine should only be called once during system initialization.

      @Preconditions
        None

      @Param
        None

      @Returns
        None

      @Comment
        8ms period with 0.4 to 1.4 ms duty cycle to control servo 

     @Example
        <code>
        uint16_t dutycycle;

        PWM1_Initialize();
        PWM1_DutyValueSet(dutycycle);
        </code>
     */
    void PWM1_Initialize(void);

    /**
      @Summary
        Loads 16-bit duty cycle.

      @Description
        This routine loads the 16 bit duty cycle value.

      @Preconditions
        PWM1_Initialize() function should have been called
        before calling this function.

      @Param
        Pass 16bit duty cycle value.

      @Returns
        None

      @Example
        <code>
        uint16_t dutycycle;

        PWM1_Initialize();
        PWM1_DutyValueSet(dutycycle);
        </code>
     */
    void PWM1_DutyValueSet(uint16_t dutyValue);

    /**
      @Summary
        Read pwm output status.

      @Description
        This routine returns the pwm output status.

      @Preconditions
        PWM1_Initialize() function should have been
        called before calling this function.

      @Param
        None

      @Returns
        true : output high
        false: output low

      @Example
        <code>
        uint16_t dutyCycle;
        bool status;
        PWM1_Initialize();
        PWM1_DutyValueSet(dutyCycle);
        while(1)
        {
                status = PWM1_OutputStatusGet();
        }
        </code>
     */
    bool PWM1_OutputStatusGet(void);

#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif	//PWM1_H
/**
 End of File
 */
