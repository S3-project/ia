//
// Created by Thimot on 22/10/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../other/Bitmap/bitmap.h"

#ifndef chars_detection_h
#define chars_detection_h

BMPIMAGE ** DetectChars(BMPIMAGE *image, size_t *number_chars, int print);
char*  IntToNameFile(int64_t x, char *name);

#endif
