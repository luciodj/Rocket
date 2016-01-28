/* 
 * File:   NeoPixel.h
 * 
 * Author: Lucio Di Jasio
 *
 */

#include "mcc_generated_files/mcc.h"

#ifndef NEOPIXEL_H
#define	NEOPIXEL_H

#define NEO_ROWS        8
#define NEO_COLUMNS     32
#define N_LED           NEO_ROWS * NEO_COLUMNS
#define POWER_LIMIT     0x1f    // mask to reduce max current draw 

#define NEO_RED     0x100000    // 0RGB
#define NEO_BLUE    0x000004
#define NEO_GREEN   0x000400 
#define NEO_PURPLE  0x100010
#define NEO_BLACK   0x000000
#define NEO_ORANGE  0x080400
#define NEO_YELLOW  0x080800


void NeoPixel_Send(uint8_t *p, uint8_t count);
void NeoPixel_Clear(uint32_t color);
void NeoPixel_SetColumn(uint8_t);
void NeoPixel_Putchar(char c, uint32_t color);
void NeoPixel_Puts(uint8_t x, char *s, uint32_t color);
void NeoPixel_ShiftLeft(void);
void NeoPixel_Show(void);
void NeoPixel_Scroll(char *, uint32_t, bool);
void NeoPixel_Gradient(uint32_t color, uint32_t grad);

#endif	/* NEOPIXEL_H */

