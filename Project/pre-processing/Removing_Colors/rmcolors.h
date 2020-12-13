#ifndef RMCOLORS_H
#define RMCOLORS_H

#include "../../other/Bitmap/bitmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


BMPIMAGE *ToBlackWhite(BMPIMAGE *image);

BMPIMAGE *Denoising(BMPIMAGE *image);

#endif
