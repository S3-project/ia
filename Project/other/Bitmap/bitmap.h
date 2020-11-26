#ifndef BITMAP_H
#define BITMAP_H

#include <stdbool.h>
#include <stdint.h>
/*************************
     Bitmap Structures
*************************/

struct RGB_STCT{
	uint8_t R; /* Red componnent */
	uint8_t G; /* Green componnent */
	uint8_t B; /* Blue componnent */
}  __attribute__((packed));
typedef struct  RGB_STCT RGB;

struct BMP_HEADER_STCT
{
	uint16_t type; /* The type of the format. need to be 0x424d */
	uint32_t bfSize; /* The size of the file (header include) in bytes */
	uint16_t unused1; 
	uint16_t unused2;
	uint32_t imageDataOffset; /* Offset to the start of image data_rgb */
	uint32_t headerSize; /* The size of the second part of the header in bytes*/
	uint32_t width; /* The width of the image in pixels*/
	uint32_t heigth; /* The heigth of the image in pixels */
	uint16_t num_planes;
	uint16_t bits_per_pixel; /* The size of an unique pixel. need to be 24 bits*/
	uint32_t compression;
	uint32_t unused3;
	uint32_t unused4;
	uint32_t unused5;
	uint32_t unused6;
	uint32_t unused7;
} __attribute__((packed));
typedef struct  BMP_HEADER_STCT BMPHEADER;

typedef struct
{
	BMPHEADER header; /* The information of the bitmap image*/
	RGB **data; /* Matrix that contains the RGBs value of each pixels*/
} BMPIMAGE;


/* Fonctions Bitmap */

BMPIMAGE *LoadBitmap(char *filename);
bool Check_bmp_header(BMPHEADER* bmp_header);

void FreeBitmap(BMPIMAGE *image);

RGB GetPixel(BMPIMAGE *image, uint32_t x, uint32_t y);

void GetRGB(RGB rgb, uint8_t *r, uint8_t *g, uint8_t *b);

BMPIMAGE *ToGrayBitmap(BMPIMAGE *image);

void PrintBitmap(BMPIMAGE *image);

BMPIMAGE *SubBitmap(BMPIMAGE *image, uint32_t x, uint32_t y, uint32_t lx, uint32_t ly);
void SaveBitmap(BMPIMAGE *image, char *filename);
BMPIMAGE *CreateImage(uint32_t row, uint32_t col);
#endif
