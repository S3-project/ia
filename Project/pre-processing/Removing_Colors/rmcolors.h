#include "../../other/Bitmap/bitmap.h"
#ifndef RMCOLORS_H
#define RMCOLORS_H

#include <stdbool.h>
#include <stdint.h>


BMPIMAGE *ToBlackWhite(BMPIMAGE *image);

BMPIMAGE *Denoising(BMPIMAGE *image);

#endif
