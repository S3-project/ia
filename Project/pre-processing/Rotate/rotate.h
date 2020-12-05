#include "../../other/Bitmap/bitmap.h"
#include <stddef.h>
#ifndef ROTATE_H
#define ROTATE_H


BMPIMAGE *Rotate(double degree, BMPIMAGE *image);
double **init_matrix(size_t c, size_t r);
double **mul(double **m1, double **m2, size_t r1, size_t c1_r2, size_t c2);
void free_mat(double **mat,size_t y);

#endif
