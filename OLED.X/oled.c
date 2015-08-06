/*
 *   Basic OLED SSD1306 display driver
 */
#include "mcc_generated_files/mcc.h"
#include "oled.h"

#define SPI_Write( x)       SPI_Exchange8bit( x)


void OLED_Command( uint8_t temp){
  OLED_CS_LAT = 0;
  OLED_DC_LAT = 0;
  SPI_Write( temp);
  OLED_CS_LAT = 1;
}


void OLED_Data( uint8_t temp){
  OLED_CS_LAT = 0;
  OLED_DC_LAT = 1;
  SPI_Write( temp);
  OLED_CS_LAT = 1;
}


void OLED_Initialize( void)
{
    OLED_RST_LAT = 0;
    __delay_ms(1000);
    OLED_RST_LAT = 1;
    __delay_ms(1000);
    OLED_Command(SSD1306_DISPLAYOFF);             //0xAE  Set OLED Display Off
    OLED_Command(SSD1306_SETDISPLAYCLOCKDIV);     //0xD5  Set Display Clock Divide Ratio/Oscillator Frequency
    OLED_Command(0x80);
    OLED_Command(SSD1306_SETMULTIPLEX);           //0xA8  Set Multiplex Ratio
    OLED_Command(0x27);
    OLED_Command(SSD1306_SETDISPLAYOFFSET);       //0xD3  Set Display Offset
    OLED_Command(0x00);
    OLED_Command(SSD1306_SETSTARTLINE);           //0x40  Set Display Start Line
    OLED_Command(SSD1306_CHARGEPUMP);             //0x8D  Set Charge Pump
    OLED_Command(0x14);                           //0x14  Enable Charge Pump
    OLED_Command(SSD1306_SETSTARTLINE | 0x0);     //40
    OLED_Command(SSD1306_COMSCANDEC);             //0xC8  Set COM Output Scan Direction
    OLED_Command(SSD1306_SETCOMPINS);             //0xDA  Set COM Pins Hardware Configuration
    OLED_Command(0x12);
    OLED_Command(SSD1306_SETCONTRAST);            //0x81   Set Contrast Control
    OLED_Command(0xAF);
    OLED_Command(SSD1306_SETPRECHARGE);           //0xD9   Set Pre-Charge Period
    OLED_Command(0x25);
    OLED_Command(SSD1306_SETVCOMDETECT);          //0xDB   Set VCOMH Deselect Level
    OLED_Command(0x20);
    OLED_Command(SSD1306_DISPLAYALLON_RESUME);    //0xA4   Set Entire Display On/Off
    OLED_Command(SSD1306_NORMALDISPLAY);          //0xA6   Set Normal/Inverse Display
    OLED_Command(SSD1306_DISPLAYON);              //0xAF   Set OLED Display On
} // OLED_Initialize


// Set page address for Page Addressing Mode
void OLED_SetRow( uint8_t add)
{
    add = 0xB0 | add;
    OLED_Command( add);
}


// Set column address for Page Addressing Mode
void OLED_SetColumn( uint8_t add)
{
    OLED_Command(( SSD1306_SETHIGHCOLUMN | (add >> 4))); // SET_HIGH_COLUMN
    OLED_Command(( 0x0f & add));        // SET LOW_COLUMN
}


// Display picture for Page  Addressing Mode
void OLED_PutPicture( const uint8_t *pic)
{
    unsigned char i,j;
    for( i=0; i<5; i++) // 5*8=40 pixel rows (actually 39)
    {
        OLED_SetRow( i);
        OLED_SetColumn( 0);
//        OLED_Command( SSD1306_SETHIGHCOLUMN);
        // SET_STARTLINE
        OLED_Command( SSD1306_SETSTARTLINE);    
        for( j=0; j<96; j++)  // 96 pixel columns
        {
            OLED_Data( pic[ i * 96 + j]);
        }
    }
}


void OLED_SetContrast( uint8_t temp)
{
    OLED_Command( SSD1306_SETCONTRAST);  
    OLED_Command( temp);                  // contrast step 1 to 256
}
