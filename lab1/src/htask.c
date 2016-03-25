#include "htask.h"
#include "hough/hough.h"
#include "hough/image10.h"

/* A quick adapter for the hough transform */
void hough_transform_task(uint16_t * counter)
{
    volatile char dummyVar;
    dummyVar = houghTransform( (uint16_t) &red, (uint16_t) &green, (uint16_t) &blue );
    (void) dummyVar; /* Mad hacks */
    *counter = *counter + 1;
}
