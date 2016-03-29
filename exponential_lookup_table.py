#!/usr/bin/python3

import math

max = 65535

x = 255 / math.log2(max + 1)
format_string='#0%dx' % math.ceil(math.log2(max) / 4 + 2)
print('{ ', end='');
for i in range(0,256):
	print(format(round(math.pow(2, i / x) - 1), format_string), end='')
	if i == 255:
		print(' };')
	elif (i + 1) % 8 == 0:
		print(',\n  ', end='')
	else:
		print(', ', end='')
