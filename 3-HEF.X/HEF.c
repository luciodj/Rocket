/*
 *  File: HEF.c
 *
 *  High Endurance Flash support functions
 *
 *  Author: Lucio Di Jasio
 *
 *  Updated on July 20, 2015 for use with MCC
 */
#include "HEF.h"

int8_t HEF_ByteArrayWrite(uint8_t block, uint8_t *byteArray, uint8_t count) {
    uint16_t blockAddr;
    uint8_t GIEBitValue = INTCONbits.GIE; // Save interrupt enable
    uint8_t i;

    if ( ( block >= HEF_BLOCKNUM) || ( count > HEF_BLOCKSIZE))
        return -1;
    
    blockAddr =  HEF_START + block * HEF_BLOCKSIZE;    
    INTCONbits.GIE = 0;         // Disable interrupts

    FLASH_EraseBlock( blockAddr);

    // Block write sequence
    PMCON1bits.CFGS = 0;        // Deselect Configuration space
    PMCON1bits.WREN = 1;        // Enable writes
    PMCON1bits.LWLO = 1;        // Load write latches

    while ( count > 0) {
        PMADR = blockAddr++;
        // Load data in current address
        PMDATL = *byteArray++;
        PMDATH = 0;
        if ( count == 1) {      // Start actual Flash program memory Write
            PMCON1bits.LWLO = 0;
        }
        // Flash unlock sequence
        PMCON2 = 0x55;
        PMCON2 = 0xAA;
        PMCON1bits.WR = 1;          // set write bit to begin Write
        NOP();
        NOP();
        count--;
    }

    PMCON1bits.WREN = 0;            // Disable writes
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable

    return 0;
} // HEF_ByteArrayWrite

int8_t HEF_ByteArrayRead( uint8_t *bArray, uint8_t block, uint8_t count)
{
    // 1. obtain absolute address in HE FLASH row
    uint16_t wordAdd = block * HEF_BLOCKSIZE + HEF_START;

    // 2. check input parameters
    if ( ( count > HEF_BLOCKSIZE) || ( block >= HEF_BLOCKNUM))
        return -1;

    // 3. read content (taking only LSB)
    while ( count > 0)
    {
        *bArray++ = (uint8_t) FLASH_ReadWord( wordAdd++);
        count--;
    }
    
    // 4. success
    return 0;
} // HEFLASH_ByteArrayRead


uint8_t HEF_ByteRead( uint8_t block, uint8_t offset)
{
    // 1. offset into HEF Flash memory
    uint16_t wordAdd = block * HEF_BLOCKSIZE + HEF_START + offset;

    // 2. read content
    return (uint8_t) FLASH_ReadWord( wordAdd);
} 


