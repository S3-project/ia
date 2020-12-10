#include "contrast.h"
#include "../other/Bitmap/bitmap.h"


void contrast(BMPIMAGE *image, uint32_t x, uint32_t y)
{
    int delta = 150;
    RGB pixel;
    int red;
    int blue;
    int green;
    double factor = 259 * (delta + 255.0) / (255.0 * (259 - delta));

    for(uint32_t i = 0; i < x; i++)
    {
        for(uint32_t j = 0; j < y; j++)
        {
            pixel = image->data[i][j];
            red = factor * (pixel.R - 128) + 128;
            blue = factor * (pixel.B - 128) + 128;
            green = factor * (pixel.G - 128) + 128;

            if(red > 255)
                red = 255;
            else if(red < 0)
                red = 0;
            if(blue > 255)
                blue = 255;
            else if(blue < 0)
                blue = 0;
            if(green > 255)
                green = 255;
            else if(green < 0)
                green = 0;

            pixel.G = green;
            pixel.R = red;
            pixel.B = blue;
            image->data[i][j] = pixel;
        }
    }
}
