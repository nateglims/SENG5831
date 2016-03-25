This collection of files can be used to implement the Hough transform for the Lab 1 assignment. This code is taken from http://rosettacode.org/wiki/Example:Hough_transform/C.  Files are ...

main.c: example main that includes the RGB image segment and calls houghTransform()

imageRTES/imageConstruct.py: Python script to create RGB matrices and configure for a #include file in c. You can configure the size of the image segment.

imageRTES/pencils.jpg: the example file on which the transform is applied

houghGray.c: contains houghTransform()

image40.c: 40x40 segment of pencils.jpg configured to store in flash memory

image60.c: 60x60 segment of pencils.jpg configured to store in flash memory

testload.c: a test file with printable characters to test the storing and retrieving of data stored in flash memory
