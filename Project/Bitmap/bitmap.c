#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


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
	if (!check_bmp_header(&image->header))
	{
		perror("Invalid BMP format.\n");
		exit(EXIT_FAILURE);	
	}


	image->data_rgb = malloc(sizeof(RGB*) * image->header.heigth);
	if (image->data_rgb == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (int i = 0; i < image->header.heigth; i++)
	{
		image->data_rgb[i] = malloc(sizeof(RGB) * image->header.width);
		if (image->data_rgb[i] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
	}


	fseek(fp, image->header.imageDataOffset, SEEK_SET);
	int colone = 0;
	int line = image->header.heigth - 1;
	int color = 0;
	int byte_size = image->header.heigth * image->header.width * (image->header.bits_per_pixel / 8); 
	for(int i = 0; i < byte_size - 1; i++){
		uint8_t hex;
		fread(&hex, 1, 1, fp);

		if (color == 0)
			image->data_rgb[line][colone].B = hex; 
		else if (color == 1)
			image->data_rgb[line][colone].G = hex; 
		else 
			image->data_rgb[line][colone].R = hex; 
		color = (color + 1) % 3;
		if (color == 0){
			colone = (colone + 1) % image->header.width;
			if (colone == 0)
				line -= 1;
		}

	}
	fclose(fp);	
	return image;
}

void	freeBitmap(BMPIMAGE *image){
	for (int i = 0; i < image->header.heigth; i++){
		free(image->data_rgb[i]);
		if (image->data_g != NULL)
			free(image->data_g[i]);
	}
	free(image->data_rgb);
	free(image->data_g);
	free(image);
}

bool	check_bmp_header(BMPHEADER* bmp_header){
	/*
		a Header is valid if:
	       		- type is "BM"
			- Headersize is equal of the offset of data_rgbimage	
	*/
	return	bmp_header->type == 0x4D42
		&& bmp_header->bits_per_pixel == 24
		&& bmp_header->heigth > 0
		&& bmp_header->width > 0;
}


void	CalculBitmapGray(BMPIMAGE *image)
{
	image->data_g = malloc(sizeof(uint8_t*) * image->header.heigth);
	if (image->data_g == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (int y = 0; y < image->header.heigth; y++)
	{
		image->data_g[y] = malloc(sizeof(uint8_t) * image->header.width);
		if (image->data_g[y] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
		for (int x = 0; x < image->header.width; x++)
		{
			image->data_g[y][x] = (uint8_t)(image->data_rgb[y][x].R * 0.299 +
					image->data_rgb[y][x].G * 0.857 + image->data_rgb[y][x].B * 0.114);
		}
	}
}


void	printBitmap_RGB(BMPIMAGE *image)
{
	if (image == NULL){
		printf("The image pointor is NULL\n");
		return;
	}
	printf("Bitmap size: %dx%d.\n", image->header.width, image->header.heigth);
	for(int y = 0; y < image->header.heigth; y++)
	{
		for(int x = 0; x < image->header.width; x++){
			printf("[");
			printf("%02x ", image->data_rgb[y][x].R);
			printf("%02x ", image->data_rgb[y][x].G);
			printf("%02x", image->data_rgb[y][x].B);
			printf("]");
		}
		printf("\n");
	}
}

void	printBitmap_G(BMPIMAGE *image){
	if (image == NULL){
		printf("The image pointor is NULL\n");
		return;
	}
	if (!isGrayBitmapCalculed(image)){
		printf("The gray scale of the bitmap hasn't be calculed. Use CalculBitmapGray().\n");
		return;
	}
	printf("Bitmap gray size: %dx%d.\n", image->header.width, image->header.heigth);
	for(int y = 0; y < image->header.heigth; y++)
	{
		for(int x = 0; x < image->header.width; x++){
			printf("[");
			printf("%02x", image->data_g[y][x]);
			printf("]");
		}
		printf("\n");
	}
}
bool	isGrayBitmapCalculed(BMPIMAGE *image){
	return image->data_g != NULL;
}
