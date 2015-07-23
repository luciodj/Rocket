/*
 * HEF.h
 *
 */
#include "mcc_generated_files/mcc.h"

#define HEF_SIZE        128        // all devices to date have the same HEF!
#define HEF_END         END_FLASH                    // defined in memory.h
#define HEF_BLOCKSIZE   WRITE_FLASH_BLOCKSIZE
// the start address depends on the total flash array size (always last top blocks)
#define HEF_START       END_FLASH - HEF_SIZE
// the number of blocks available depends on block size and ultimately on the 
//   size of the entire flash array (8 blocks for up to 2k words, 4 above..)
#define HEF_BLOCKNUM    HEF_SIZE / HEF_BLOCKSIZE

/******************************************************************************
 * High Endurance Flash functions 
 * For convenience all HEF functions operate on a block number 0..3 (or 0..7)
 * Also, all functions operate on byte arrays for "data storage"
 * as opposed to regular Flash (memory.h) functions that are designed for "code 
 * storage" and therefore operate on word arrays.
 */

/**
 * Write a block of data (byte array) to High Endurance Flash
 *      the entire block must fit within a erase/write block (WRITE_FLASH_BLOCKSIZE)
 *
 * @param block     destination HEF block number (0 .. HEF_BLOCKNUM-1)
 * @param bArray    source byte array
 * @param count     number of bytes to write to block (< HEF_BLOCKSIZE)
 * @return          0 if successful, -1 if parameter error, -2 if write error
 */
int8_t HEF_ByteArrayWrite( uint8_t block, uint8_t *bArray, uint8_t count);


/**
 * Read a block of data (byte array) from High Endurance Flash
 *
 * @param bArray    destination byte array (must be sufficiently large)
 * @param block     source HEF block number (0 .. HEF_BLOCKNUM-1)
 * @param count     number of bytes to be retrieved (< HEF_BLOCKSIZE)
 * @return          0 if successful, -1 if parameter error
 */
int8_t HEF_ByteArrayRead( uint8_t *bArray, uint8_t block, uint8_t count);


/**
 * Read a byte of data from High Endurance Flash
 *
 * @param block     source HEF block number (0 .. HEF_BLOCKNUM-1)
 * @param offset    relative offset within the HEF block (< HEF_BLOCKSIZE)
 * @return          byte of data retrieved
 */
uint8_t HEF_ByteRead( uint8_t block, uint8_t offset);

