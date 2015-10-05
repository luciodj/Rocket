#!/usr/bin/env python
# open a font file and pivot around the first 128 characters

import sys

offset = 5		# first line containing a font definition
size = 96

print """
/*
 * pivoted 8x8 FONT
 */

#define F_SIZE 		128

const char Font8x8[] = {"""

fp = open( 'font.h')
lines = fp.readlines()

for x in xrange( 8):
	print '// row %d' % x
	for i in xrange( 32):
		print '0x00,',

	for i in xrange( 96):
		print lines[ offset + x + i * 9].strip(' \r\n'),
	print

fp.close()
print '};'

