/**
  MEMORY Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    memory.h

  @Summary
    This is the generated header file for the MEMORY driver using MPLAB® Code Configurator

  @Description
    This header file provides APIs for driver for MEMORY.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC16F1619
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 *******************************************************************************/

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

#ifndef _MEMORY_H
#define _MEMORY_H

/**
  Section: Included Files
 */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

    /**
      Section: Macro Declarations
     */

#define WRITE_FLASH_BLOCKSIZE    32
#define ERASE_FLASH_BLOCKSIZE    32
#define END_FLASH                8192

    /**
      Section: Flash Module APIs
     */

    /**
      @Summary
        Reads a word from Flash

      @Description
        This routine reads a word from given Flash address

      @Preconditions
        None

      @Param
        flashAddr - Flash program memory location from which data has to be read

      @Returns
        Data word read from given Flash address

      @Example
        <code>
        uint16_t    readWord;
        uint16_t    flashAddr = 0x01C0;

        readWord = FLASH_ReadWord(flashAddr);
        </code>
     */
    uint16_t FLASH_ReadWord(uint16_t flashAddr);

    /**
      @Summary
        Writes a word into Flash

      @Description
        This routine writes the given word into mentioned Flash address

      @Preconditions
        None

      @Param
        flashAddr - Flash program memory location to which data has to be written
     *ramBuf   - Pointer to an array of size 'ERASE_FLASH_BLOCKSIZE' at least
        word      - Word to be written in Flash

      @Returns
        None

      @Example
        <code>
        uint16_t    writeData = 0x55AA;
        uint16_t    flashAddr = 0x01C0;
        uint16_t    Buf[ERASE_FLASH_BLOCKSIZE];

        FLASH_WriteWord(flashAddr, Buf, writeData);
        </code>
     */
    void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word);

    /**
      @Summary
        Writes data to complete block of Flash

      @Description
        This routine writes data words to complete block in Flash program memory

      @Preconditions
        None

      @Param
        writeAddr         - A valid block starting address in Flash
     *flashWordArray   - Pointer to an array of size 'WRITE_FLASH_BLOCKSIZE' at least

      @Returns
        -1, if the given address is not a valid block starting address of Flash
        0, in case of valid block starting address

      @Example
        <code>
        #define FLASH_ROW_ADDRESS     0x01C0

        uint16_t wrBlockData[] =
        {
            0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
            0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000D, 0x000F,
            0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
            0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F
        }

        // write to Flash memory block
        FLASH_WriteBlock((uint16_t)FLASH_ROW_ADDRESS, (uint16_t*)wrBlockData);
        </code>
     */
    int8_t FLASH_WriteBlock(uint16_t writeAddr, uint16_t *flashWordArray);

    /**
      @Summary
        Erases complete Flash program memory block

      @Description
        This routine erases complete Flash program memory block

      @Preconditions
        None

      @Param
        startAddr - A valid block starting address in Flash program memory

      @Returns
        None

      @Example
        <code>
        uint16_t    flashBlockStartAddr = 0x01C0;

        FLASH_EraseBlock(flashBlockStartAddr);
        </code>
     */
    void FLASH_EraseBlock(uint16_t startAddr);

#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif // _MEMORY_H
/**
 End of File
 */

