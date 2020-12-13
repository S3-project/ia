#include "negative.h"


void negative(BMPIMAGE *image, uint32_t x, uint32_t y)
{
    RGB pixel;
    for(uint32_t i = 0; i < x; i++)
    {
        for(uint32_t j = 0; j < y; j++)
        {
            pixel = image->data[i][j];
            pixel.G = 255 - pixel.G;
            pixel.R = 255 - pixel.R;
            pixel.B = 255 - pixel.B;
            image->data[i][j] = pixel;
        }
    }
}
