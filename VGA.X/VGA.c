/*
** VGA.c
**
** VGA video output
**
** Text page for PIC16F1
*/

#include <xc.h>
#include <string.h>
#include "vga.h"

#include "rfont.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "tmr2.h"
#include "pwm4.h"
#include "eusart.h"

#define VGA_CLK     80000000                // Hz
#define VGA_LINE_T  0.00003177              // 31.777 us
#define LINE_C      (VGA_LINE_T * VGA_CLK)  // clock pulses in a line
#define LINE_N      525                     // number of lines in VGA frame

#define VRES    ((YMAX+1)*8)                // each line repeat twice ->400 (< LINE_N)
#define HRES    ((XMAX+1)*8)                // must be < LINE_C

// timing for VGA video vertical state machine

// count the number of remaining black lines top+bottom
#define VSYNC_N  2                          // V sync lines 
#define VBLANK_N (LINE_N -VRES*2 -VSYNC_N)
#define PREEQ_N  VBLANK_N/2                 // preeq + bottom blank
#define POSTEQ_N VBLANK_N -PREEQ_N          // posteq + top blank

// definition of the vertical sync state machine
#define SV_PREEQ    0
#define SV_SYNC     1
#define SV_POSTEQ   2
#define SV_LINE     3

char VMap[ VRES/8 * (HRES/8)]; // image buffer

char *VPtr, *LPtr;
const char* pRFont;
volatile short VCount;
volatile char VState;

// next state table
char VS[4] = { SV_SYNC, SV_POSTEQ, SV_LINE, SV_PREEQ};
// next counter table
short VC[4] = { VSYNC_N,  POSTEQ_N, VRES*2,  PREEQ_N};

//  TX1REG = pRFont[*VPtr++];
#define mSendByte()     asm("moviw FSR1++"); \
                        asm("addwf 0x7F,w"); \
                        asm("movwf FSR0L"); \
                        asm("movf  INDF0,w"); \
                        asm("movwf TX1REG &0x7f"); \
                        NOP(); \
                        NOP();

void interrupt VGA( void)
{
    static char line = 16;  
    switch ( VState) {
        case SV_LINE:       // 3   serialize the current line
            VPtr = LPtr;    // reset text pointer
            // pre load FSR0 with _pRFont (pointer into the font)   
            asm("movlb 0");
            asm("movf _pRFont, w");
            asm("movwf 0x7F");
            asm("movf _pRFont+1, w");
            asm("movwf FSR0H");
            // pre load FSR1 with _VPtr (pointer into the screen matrix) 
            asm("movf _VPtr, w");
            asm("movwf FSR1L" );
            asm("movf _VPtr+1,w"); 
            asm("movwf FSR1H ");
            asm("movlb 3");
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();
            mSendByte();    
            // prepare for next scan line
            line--;
            if (line & 1)           // odd lines must repeat
                break;
            if (line)
                pRFont += F_SIZE;   // advance font pointer
            else {
                pRFont = Font8x8;   // reset font pointer
                line = 16;          // reset font row counter
                LPtr += HRES/8;     // advance text pointer
            }
            break;
        case SV_PREEQ:      // 0
            VPtr = VMap;    // prepare for the new frame
            break;
        case SV_SYNC:       // 1
            VS_SetLow();    // vertical sync pulse
            break;
        case SV_POSTEQ:     // 2
            VS_SetHigh();   // horizontal sync pulse
            LPtr = VMap;           // reset text pointer to top of page
            break;
    } //switch
    // advance the state machine
    if ( --VCount == 0) {
        VCount = VC[ VState & 3];
        VState = VS[ VState & 3];
    }
    PIR1bits.TMR2IF = 0;
} // VGA state machine

void initVGA( void)
{
    // 1. init Timer 2
    TMR2_Period8BitSet( 253);
    
    // 2. UART baudrate
    SPBRGL = 0;

    // 3. PWM intialize
    PWM4_LoadDutyValue( 60);

    // 5. init the vertical sync state machine
    VState = SV_PREEQ; 
    VCount = 1;                 

    TMR2_Start();
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

} // initVGA

char cx, cy;

void clear( void)
{ 
    memset( VMap, ' ', (YMAX+1)*( XMAX+1));
    cx = cy = 0;
} 

void putch( char a)
{
    unsigned short i;
    
    // 0. check for backspace
    if ( a == 0x8)  {
        if ( cx > 0) {
            cx--; VMap[ cy * HRES + cx] = ' ';
        }
        return;
    }     
    if ( a == '\n') {
        cx = 0; cy++;
        return;
    } 
    // 1. check if char in range
    if ( a >= F_SIZE)   
        return;
     
    // 2. check page boundaries and wrap or scroll as necessary
    if ( cx > XMAX)          // wrap around x
    {
        cx = 0;
        cy++;   
    } 
    if ( cy > YMAX)          //  scroll up y
    {
        char *pd = VMap;
        char *ps = pd + XMAX + 1;
        for( i=0; i<(XMAX+1)*YMAX; i++)
            *pd++ = *ps++;
        for( i=0; i<XMAX; i++)
            *pd++ = ' ';
        // keep cursor within boundary
        cy = YMAX;
    }
    
    // 3. set pointer to word in the video map
    VMap[ cy * (XMAX+1) + cx] = a;
    
    // 5. advance cursor position
    cx++;
} // putch
