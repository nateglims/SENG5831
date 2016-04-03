# These have to be installed.
from PIL import Image
import numpy as np
import math

# open image and print stats
im = Image.open("pencils.jpg")
print im.format, im.size, im.mode

# convert to 4 2-dimensional matrices
rgbmat = np.array(im) # rowxcolx4
#print(rgbmat[0,0,0])

colors = ['red','blue','green']
height = 40
width = 40

print("#define __Height "+str(height))
print("#define __Width "+str(width))
print(' ')

# print each band to be cut and paste into a c-file #define
for band in range (0,3):
	print("static const uint8_t "+colors[band]+"["+str(height)+"]["+str(width)+"] PROGMEM = {")
	for row in range(0,height):
		c_row =  "{ "+str(rgbmat[row,0,band])
		for col in range(1,width):
			c_row = c_row + "," + str(rgbmat[row,col,band])
		c_row = c_row+"} ,"
		print(c_row)
	print("};")
	print(" ")
	# the last line of each printed band needs to be edited - remove comma


