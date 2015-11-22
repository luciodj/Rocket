/**
  AT1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    at1.h

  @Summary
    This is the generated header file for the AT1 driver using MPLAB� Code Configurator

  @Description
    This header file provides APIs for driver for AT1.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  1.0
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

#ifndef _AT1_H
#define _AT1_H

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
      Section: ADC Module APIs
     */

    /**
      @Summary
        Initializes the AT1

      @Description
        This routine initializes the Initializes the AT1.
        This routine must be called before any other AT routine is called.
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
    
        </code>
     */
    void AT1_Initialize(void);

    /**
      @Summary
        Load the required 10bit Phase counter resolution 

      @Description
        Set the required resolution for the incoming signal

      @Preconditions
        None

      @Param
        Pass 10 bit Phase counter resolution 

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_ResolutionSet(uint16_t resolution);

    /**
      @Summary
        Load required 15 bit missing pulse delay

      @Description
        In multiple pulse mode. set the expected missing pulses

      @Preconditions
        None

      @Param
        Pass 15 bit missing pulse delay

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_MissingPulseDelaySet(int16_t missingPulse);

    /**
      @Summary
        Load threshold period for error calculation 

      @Description
        Set the acceptable threshold point for the period

      @Preconditions
        None

      @Param
        Pass 16 bit set point

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_SetPointLoad(uint16_t thresholdPeriod);

    /**
      @Summary
        Read measured period count

      @Description    
        Read the measured period count
     * 
      @Preconditions
        None

      @Param
        None

      @Returns
        Return 15 bit period count 

      @Example
        <code>
    
        </code>
     */
    uint16_t AT1_PeriodGet(void);

    /**
      @Summary
        Read measured phase count

      @Description    
        Read measured phase count

      @Preconditions
        None

      @Param
        None

      @Returns
        Return 10 bit Phase count

      @Example
        <code>
    
        </code>
     */
    uint16_t AT1_PhaseGet(void);

    /**
      @Summary
        Read error of the measured period value compared to the threshold setting (ATxPER ? ATxSTP = ATxERR)

      @Description    
        Read calculated error from the difference of period with set point

      @Preconditions
        None

      @Param
        None

      @Returns
        Return 16 bit error count

      @Example
        <code>
    
        </code>
     */
    int16_t AT1_SetPointErrorGet(void);

    /**
      @Summary
        Returns comparison result of current period value with previous value.

      @Description    
        Compares with the previous value of period register and displays result as less than or greater than 	

      @Preconditions
        None

      @Param
        None

      @Returns
        true - The value currently in AT1PER is less than the previous value
        false - The value currently in AT1PER is greater than or equal to the previous value

      @Example
        <code>
    
        </code>
     */
    bool AT1_CheckPeriodValue(void);

    /**
      @Summary
        Returns status after verifying ATxPER(period) and ATxPHS(phase) input cycles.

      @Description    
        Confirms weather result obtained in period and phase registers are valid of invalid

      @Preconditions
        None

      @Param
        None

      @Returns
        true - valid input cycles
        false - Invalid input cycles( not enough input cycles)

      @Example
        <code>
    
        </code>
     */
    bool AT1_IsMeasurementValid(void);

    /**
      @Summary
        Returns status of ATxPER(period counter).

      @Description    
        Confirms if the result obtained in period register is rolled over or not

      @Preconditions
        None

      @Param
        None

      @Returns
        true - Counter rolled over one or more times during measurement
        false - Value shown by ATxPER is valid

      @Example
        <code>
    
        </code>
     */
    bool AT1_IsPeriodCounterOverflowOccured(void);

    /**
      @Summary
        Returns status of Phase interrupt flag bit (PHSIF ).

      @Description    
        Calculated phase count will be available in this register

      @Preconditions
        None

      @Param
        None

      @Returns
        true - phase count is available in register
        false - phase count is not available

      @Example
        <code>
    
        </code>
     */
    bool AT1_IsPhaseCountAvailable(void);

    /**
      @Summary
        Returns status of Missed Pulse interrupt flag bit (MSSIF ).

      @Description
    

      @Preconditions
        None

      @Param
        None

      @Returns
        true -  Missing pulse count is available
        false - Missing pulse count is not available

      @Example
        <code>
    
        </code>
     */
    bool AT1_IsMissedPulseCountAvailable(void);

    /**
      @Summary
        Returns status of Period interrupt flag bit (PERIF )

      @Description
     

      @Preconditions
        None

      @Param
        None

      @Returns
        true - Period count available
        false - Period count not available

      @Example
        <code>
    
        </code>
     */
    bool AT1_IsPeriodCountAvailable(void);


    /**
      @Summary
        Load count to ATxCCy register to compare with ATxPHS

      @Description
        This routine loads the 16 bit compare value.

      @Preconditions
        None

      @Param
        Pass in 16bit compare value

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_CC1_Compare_SetCount(uint16_t compareCount);

    /**
      @Summary
        Returns the status of comparison between ATxCCy and ATxPHS register

      @Description
        This routine is used to determine if data capture is completed.
        When data capture is complete routine returns 1. It returns 0 otherwise.

      @Preconditions
        None

      @Param
        None

      @Returns
        true - Indicates compare is complete
       false - Indicates compare is not complete
   
      @Example
        <code>
    
        </code>
     */
    bool AT1_CC1_Compare_IsMatchOccured(void);


    /**
      @Summary
        Load count to ATxCCy register to compare with ATxPHS

      @Description
        This routine loads the 16 bit compare value.

      @Preconditions
        None

      @Param
        Pass in 16bit compare value

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_CC2_Compare_SetCount(uint16_t compareCount);

    /**
      @Summary
        Returns the status of comparison between ATxCCy and ATxPHS register

      @Description
        This routine is used to determine if data capture is completed.
        When data capture is complete routine returns 1. It returns 0 otherwise.

      @Preconditions
        None

      @Param
        None

      @Returns
        true - Indicates compare is complete
       false - Indicates compare is not complete
   
      @Example
        <code>
    
        </code>
     */
    bool AT1_CC2_Compare_IsMatchOccured(void);


    /**
      @Summary
        Load count to ATxCCy register to compare with ATxPHS

      @Description
        This routine loads the 16 bit compare value.

      @Preconditions
        None

      @Param
        Pass in 16bit compare value

      @Returns
        None

      @Example
        <code>
    
        </code>
     */
    void AT1_CC3_Compare_SetCount(uint16_t compareCount);

    /**
      @Summary
        Returns the status of comparison between ATxCCy and ATxPHS register

      @Description
        This routine is used to determine if data capture is completed.
        When data capture is complete routine returns 1. It returns 0 otherwise.

      @Preconditions
        None

      @Param
        None

      @Returns
        true - Indicates compare is complete
       false - Indicates compare is not complete
   
      @Example
        <code>
    
        </code>
     */
    bool AT1_CC3_Compare_IsMatchOccured(void);


#endif	/* AT1_H */
    /**
     End of File
     */
