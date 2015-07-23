/**
  MEMORY Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    memory.c

  @Summary
    This is the generated driver implementation file for the MEMORY driver using MPLAB® Code Configurator

  @Description
    This file provides implementations of driver APIs for MEMORY.
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

/**
  Section: Included Files
 */

#include <xc.h>
#include "memory.h"

/**
  Section: Flash Module APIs
 */

uint16_t FLASH_ReadWord(uint16_t flashAddr) {
    uint8_t GIEBitValue = INTCONbits.GIE; // Save interrupt enable

    INTCONbits.GIE = 0; // Disable interrupts
    PMADRL = (flashAddr & 0x00FF);
    PMADRH = ((flashAddr & 0xFF00) >> 8);

    PMCON1bits.CFGS = 0; // Deselect Configuration space
    PMCON1bits.RD = 1; // Initiate Read
    NOP();
    NOP();
    INTCONbits.GIE = GIEBitValue; // Restore interrupt enable

    return ((PMDATH << 8) | PMDATL);
}

void FLASH_WriteWord(uint16_t flashAddr, uint16_t *ramBuf, uint16_t word) {
    uint16_t blockStartAddr = (uint16_t) (flashAddr & ((END_FLASH - 1) ^ (ERASE_FLASH_BLOCKSIZE - 1)));
    uint8_t offset = (uint8_t) (flashAddr & (ERASE_FLASH_BLOCKSIZE - 1));
    uint8_t i;

    // Entire row will be erased, read and save the existing data
    for (i = 0; i < ERASE_FLASH_BLOCKSIZE; i++) {
        ramBuf[i] = FLASH_ReadWord((blockStartAddr + i));
    }

    // Write at offset
    ramBuf[offset] = word;

    // Writes ramBuf to current block
    FLASH_WriteBlock(blockStartAddr, ramBuf);
}

int8_t FLASH_WriteBlock(uint16_t writeAddr, uint16_t *flashWordArray) {
    uint16_t blockStartAddr = (uint16_t) (writeAddr & ((END_FLASH - 1) ^ (ERASE_FLASH_BLOCKSIZE - 1)));
    uint8_t GIEBitValue = INTCONbits.GIE; // Save interrupt enable
    uint8_t i;

    // Flash write must start at the beginning of a row
    if (writeAddr != blockStartAddr) {
        return -1;
    }

    INTCONbits.GIE = 0; // Disable interrupts

    // Block erase sequence
    FLASH_EraseBlock(writeAddr);

    // Block write sequence
    PMCON1bits.CFGS = 0; // Deselect Configuration space
    PMCON1bits.WREN = 1; // Enable wrties
    PMCON1bits.LWLO = 1; // Only load write latches

    for (i = 0; i < WRITE_FLASH_BLOCKSIZE; i++) {
        // Load lower 8 bits of write address
        PMADRL = (writeAddr & 0xFF);
        // Load upper 6 bits of write address
        PMADRH = ((writeAddr & 0xFF00) >> 8);

        // Load data in current address
        PMDATL = flashWordArray[i];
        PMDATH = ((flashWordArray[i] & 0xFF00) >> 8);

        if (i == (WRITE_FLASH_BLOCKSIZE - 1)) {
            // Start Flash program memory write
            PMCON1bits.LWLO = 0;
        }

        PMCON2 = 0x55;
        PMCON2 = 0xAA;
        PMCON1bits.WR = 1;
        NOP();
        NOP();

        writeAddr++;
    }

    PMCON1bits.WREN = 0; // Disable writes
    INTCONbits.GIE = GIEBitValue; // Restore interrupt enable

    return 0;
}

void FLASH_EraseBlock(uint16_t startAddr) {
    uint8_t GIEBitValue = INTCONbits.GIE; // Save interrupt enable


    INTCONbits.GIE = 0; // Disable interrupts
    // Load lower 8 bits of erase address boundary
    PMADRL = (startAddr & 0xFF);
    // Load upper 6 bits of erase address boundary
    PMADRH = ((startAddr & 0xFF00) >> 8);

    // Block erase sequence
    PMCON1bits.CFGS = 0; // Deselect Configuration space
    PMCON1bits.FREE = 1; // Specify an erase operation
    PMCON1bits.WREN = 1; // Allows erase cycles

    // Start of required sequence to initiate erase
    PMCON2 = 0x55;
    PMCON2 = 0xAA;
    PMCON1bits.WR = 1; // Set WR bit to begin erase
    NOP();
    NOP();

    PMCON1bits.WREN = 0; // Disable writes
    INTCONbits.GIE = GIEBitValue; // Restore interrupt enable
}
/**
 End of File
 */
