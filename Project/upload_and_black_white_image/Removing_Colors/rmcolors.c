#include "../Bitmap/bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
Return the gray scale of the bitmap with this formula : (R+B+G) / 3, using the RGB Matrix "data"
*/
/*BMPIMAGE *ToGrayBitmap(BMPIMAGE *image)
{
	BMPIMAGE *new_image = malloc(sizeof(*new_image));
	if (new_image == NULL){
	perror("Allocation error.\n");
	exit(EXIT_FAILURE);
	}
	new_image->header = image->header;
	new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
	if (new_image->data == NULL){
	perror("Allocation error.\n");
	exit(EXIT_FAILURE);
	}
	for (int y = 0; y < new_image->header.heigth; y++)
	{
		new_image->data[y] = malloc(sizeof(RGB) * new_image->header.width);
		if (new_image->data[y] == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);
		}
		for (int x = 0; x < new_image->header.width; x++)
		{
			int color = (image->data[y][x].R + image->data[y][x].G
			+ image->data[y][x].B) / 3;
			new_image->data[y][x].R = (uint8_t) color;
			new_image->data[y][x].G = (uint8_t) color;
			new_image->data[y][x].B = (uint8_t) color;
		}
	}
	return new_image;
}*/

/*
	Return an image in binary Black and White
*/
BMPIMAGE *ToBlackWhite(BMPIMAGE *image)
{
	BMPIMAGE *new_image = malloc(sizeof(*new_image));
	if (new_image == NULL)
	{
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);
	}
	new_image->header = image->header;
	new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
	if (new_image->data == NULL)
	{
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);
	}
	uint8_t r;
	uint8_t g;
	uint8_t b;
	for(uint32_t i =0; i<new_image->header.heigth;i++)
	{
		new_image->data[i] = malloc(sizeof(RGB) * new_image->header.width);
		if (new_image->data[i] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);
		}
		for(uint32_t j=0; j<new_image->header.width;j++)
		{
			RGB rgb =GetPixel(image,j,i);
			GetRGB(rgb,&r,&g,&b);
			if(r>=127 && g>=127 && b>=127)
			{
				r=255;
				g=255;
				b=255;
			}
			else{
				r=0;
				g=0;
				b=0;
			}
			new_image->data[i][j].R = r;
			new_image->data[i][j].G = g;
			new_image->data[i][j].B = b;
		}
	}
	return new_image;
}
