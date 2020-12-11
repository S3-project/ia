#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>




/*
	Create a BMPIMAGE from a bitmap image. 
	And allocate memory
*/
BMPIMAGE	*LoadBitmap(char *filename)
{	
	BMPIMAGE *image = malloc(sizeof(*image));
	if (image == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}

	FILE *fp = fopen(filename, "r");
	if (fp == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);	
	}
	
	rewind(fp);
	fread(&image->header, sizeof(image->header), 1, fp);
	if (!Check_bmp_header(&image->header))
	{
		perror("Invalid BMP format.\n");
		exit(EXIT_FAILURE);	
	}


	image->data = malloc(sizeof(RGB*) * image->header.heigth);
	if (image->data == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (uint32_t i = 0; i < image->header.heigth; i++)
	{
		image->data[i] = malloc(sizeof(RGB) * image->header.width);
		if (image->data[i] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
	}


	fseek(fp, image->header.imageDataOffset, SEEK_SET);
	int offset_endLine = (4 - (image->header.width * 3) % 4) % 4;
	int colone = 0;
	int line = image->header.heigth - 1;
	int color = 0;
	int byte_size = image->header.heigth * image->header.width * (image->header.bits_per_pixel / 8); 
	for(int i = 0; i < byte_size; i++){
		uint8_t hex;
		fread(&hex, 1, 1, fp);
		if (color == 0)
			image->data[line][colone].B = hex; 
		else if (color == 1)
			image->data[line][colone].G = hex; 
		else 
			image->data[line][colone].R = hex; 
		color = (color + 1) % 3;
		if (color == 0){
			colone = (colone + 1) % image->header.width;
			if (colone == 0){
				fseek(fp, offset_endLine, SEEK_CUR);
				line -= 1;
			}
		}

	}
	fclose(fp);	
	return image;
}




/*
	Free the BMPIMAGE from the memory
*/
void	FreeBitmap(BMPIMAGE *image){
	for (uint32_t i = 0; i < image->header.heigth; i++){
		free(image->data[i]);
	}
	free(image->data);
	free(image);
}




/*
	Check if the header of the file opened has a good format.
*/
bool	Check_bmp_header(BMPHEADER* bmp_header){
	return	bmp_header->type == 0x4D42
		&& bmp_header->bits_per_pixel == 24
		&& bmp_header->heigth > 0
		&& bmp_header->width > 0;
}





/*
	Return the gray scale of the bitmap with this formula : (R+B+G) / 3, using the RGB Matrix "data"
*/
BMPIMAGE	*ToGrayBitmap(BMPIMAGE *image)
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
	for (uint32_t y = 0; y < new_image->header.heigth; y++)
	{
		new_image->data[y] = malloc(sizeof(RGB) * new_image->header.width);
		if (new_image->data[y] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
		for (uint32_t x = 0; x < new_image->header.width; x++)
		{
			int color = (image->data[y][x].R + image->data[y][x].G 
				+ image->data[y][x].B) / 3;
			new_image->data[y][x].R = (uint8_t) color;
			new_image->data[y][x].G = (uint8_t) color;
			new_image->data[y][x].B = (uint8_t) color;
		}
	}
	return new_image;
}


/*
	Print the Matrix of data on the terminal
*/
void	PrintBitmap(BMPIMAGE *image)
{
	if (image == NULL){
		printf("The image pointor is NULL\n");
		return;
	}
	printf("Bitmap size: %dx%d.\n", image->header.width, image->header.heigth);
	for(uint32_t y = 0; y < image->header.heigth; y++)
	{
		for(uint32_t x = 0; x < image->header.width; x++){
			printf("[");
			printf("%02x ", image->data[y][x].R);
			printf("%02x ", image->data[y][x].G);
			printf("%02x", image->data[y][x].B);
			printf("]");
		}
		printf("\n");
	}
}




/*
	Save the bitmap ("data")
*/
void	SaveBitmap(BMPIMAGE *image, char *filename)
{
	int offset_endLine = (4 - (image->header.width * 3) % 4) % 4;
	image->header.bfSize = 14 + 40 + (3*image->header.width * image->header.heigth +
			offset_endLine * image->header.heigth);
	FILE *fp = fopen(filename, "wb");
	fwrite(&image->header, 1, sizeof(image->header), fp);
	uint8_t null[3] = {0,0,0};
	for (uint32_t y = image->header.heigth - 1; y+1 > 0; y--){
		for (uint32_t x = 0; x < image->header.width; x++){
			for (int c = 0; c < 3; c++){
				if (c == 0)
					fwrite(&image->data[y][x].B, 1, 1, fp);
				else if(c == 1)
					fwrite(&image->data[y][x].G, 1, 1, fp);
				else {
					fwrite(&image->data[y][x].R, 1, 1, fp);
				}
			}
		}
		fwrite(&null, 1, offset_endLine, fp);
	}
	fclose(fp);
}


/*
	Get the color by an RGB structure from the position in the matrix data of the image.
*/
RGB	GetPixel(BMPIMAGE *image, uint32_t x, uint32_t y)
{
	if (x >= image->header.width || y >= image->header.heigth){
		perror("GetPixel params out of bounds.\n");
		exit(EXIT_FAILURE);	
	}
	return image->data[y][x];
}

/*
	Get the r,g,b values of the RGB structure
*/
void GetRGB(RGB rgb, uint8_t *r, uint8_t *g, uint8_t *b)
{

	*r=rgb.R;
	*g=rgb.G;
	*b=rgb.B;
}



BMPIMAGE *SubBitmap(BMPIMAGE *image, uint32_t x, uint32_t y, uint32_t lx, uint32_t ly)
{
	if (lx == 0 || ly == 0 || x+lx > image->header.width || y+ly > image->header.heigth){
		perror("GetPixel params out of bounds.\n");
		exit(EXIT_FAILURE);	
	}
	BMPIMAGE *new_image = malloc(sizeof(*new_image));
	if (new_image == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	new_image->header = image->header;
	new_image->header.width = lx;
	new_image->header.heigth = ly;
	new_image->header.bfSize = 14 + 40 + (3*new_image->header.width * new_image->header.heigth +
                                          (4 - (new_image->header.width*3) %4)%4) * new_image->header.heigth;
    new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
	if (new_image->data == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (uint32_t yy = 0; yy < ly; yy++)
	{
		new_image->data[yy] = malloc(sizeof(RGB) * lx);
		if (new_image->data[yy] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
		for (uint32_t xx = 0; xx < lx; xx++)
		{
			new_image->data[yy][xx] = image->data[y+yy][x+xx];
		}
	}
	return new_image;
}


BMPIMAGE *CreateImage(uint32_t row, uint32_t col)
{
    BMPIMAGE *image = malloc(sizeof(*image));
    if (image == NULL){
        perror("Allocation error.\n");
        exit(EXIT_FAILURE);
    }
    image->header.type = 0x4d42;
    image->header.unused1 = 0;
    image->header.unused2 = 0;
    image->header.imageDataOffset = 54;
    image->header.headerSize = 40;
    image->header.width = col;
    image->header.heigth = row;
    image->header.num_planes = 1;
    image->header.bits_per_pixel = 24;
    image->header.compression = 0;
    image->header.unused3 = 0;
    image->header.unused4 = 0;
    image->header.unused5 = 0;
    image->header.unused6 = 0;
    image->header.unused7 = 0;


    image->header.bfSize = 14 + 40 + (3*image->header.width * image->header.heigth +
                                       (4 - (image->header.width*3) %4)%4) * image->header.heigth;

    image->data = malloc(sizeof(RGB*) * image->header.heigth);
    if (image->data == NULL){
        perror("Allocation error.\n");
        exit(EXIT_FAILURE);
    }

    RGB pixelBlanc = {255,255,255};
    for (uint32_t i = 0; i < image->header.heigth; i++)
    {
        image->data[i] = malloc(sizeof(RGB) * image->header.width);
        if (image->data[i] == NULL){
            perror("Allocation error.\n");
            exit(EXIT_FAILURE);
        }

       for(uint32_t k = 0; k < image->header.width; k++)
           image->data[i][k] = pixelBlanc;
    }

    return image;
}

