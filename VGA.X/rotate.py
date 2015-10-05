#!/usr/bin/env python
# open a font file and rotate around (Lsb first) the first 128 characters 

def rotate( y):
    r = 0
    x = y 
    for _ in xrange( 7): # shifting the first column out 
        r <<= 1
        r += (x & 1)
        x >>= 1
    print '0x%02X,' % r,

with open( 'rfont.c') as fi:
    for line in fi.readlines():
        if line[0] != '0':
            print line,
        else:            
            for count, byte in enumerate( line.split( ',')):
                byte = byte.strip()
                if len(byte) > 1:
                    if byte[1] == 'x':  rotate (int ( byte[2:], 16))
                    elif byte[1] == 'b':  rotate( int( byte[2:], 2))
                    else: rotate( byte)
                else: print byte
                if (count+1) % 16 == 0: print
