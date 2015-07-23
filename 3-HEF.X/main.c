/* Project:     High Endurance Flash (HEF)
 * Device:      PIC16F1619
 */
#include "mcc_generated_files/mcc.h"
#include "HEF.h"
#include "string.h"
#include "assert.h"

void _fassert( int line, const char *file, const char *expr)
{   char i;
    while( 1)        {              // flashing LED to signal error
        for ( i=0; i<(line & 0x07)*2; i++){
            LED_Toggle();           // flash the last 3 bit of the line number
            __delay_ms( 100);
        }
        __delay_ms( 1000);
    }
}


void main(void)
{
    uint8_t r;
    char    buffer[ HEF_BLOCKSIZE];
    typedef struct { 
        uint16_t ID; 
        char     Name[20]; 
        int32_t  Amount;
    } Record;
    // a block of data that needs saving -- fast!
    Record data = { 0x1234, "High Endurance", 42};
    
    SYSTEM_Initialize();
    
    // write data to HEF block 1 (2ms!)
    r = HEF_ByteArrayWrite( 1, (void *) &data, sizeof( data));
    assert( r==0);
    
    // empty the buffer
    memset( &data, 0, sizeof( data));

    // read back its contents
    r = HEF_ByteArrayRead( (void *) &data, 1, sizeof( data));
    assert( data.Amount == 42 );
    
    // read a single byte
    r = HEF_ByteRead( 1, 5);

    LED_SetHigh();      // success
    while( 1);
}


