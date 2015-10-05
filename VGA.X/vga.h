/*
 * VGA.h
 */
#define XMAX    19
#define YMAX    24

extern char cx, cy;            // cursor

void initVGA( void);    // intialization
void putc( char);      // print a character on screen
//void puts( char*);      // print a string on screen
void putsr( const char*);      // print a string on screen
void clear( void);    
