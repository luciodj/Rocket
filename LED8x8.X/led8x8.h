/*
 *  LED8x8 click driver
 */

extern uint8_t matrix[8];

// send register / value pair
void LED8x8_Write( uint8_t reg, uint8_t b);
void LED8x8_Initialize( uint8_t intensity);
void LED8x8_Display( uint8_t *m);
void LED8x8_Putch( uint8_t *m, char c);
void LED8x8_ShiftLeft( uint8_t *m, char c, uint8_t n);
void LED8x8_ShiftUp( uint8_t *m, char c, uint8_t n);
void LED8x8_RotateUp( uint8_t *m);
void LED8x8_RotateLeft( uint8_t *m);
void LED8x8_HorMessage( uint8_t *m, char *s);
void LED8x8_VerMessage( uint8_t *m, char *s);
