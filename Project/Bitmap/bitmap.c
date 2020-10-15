#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>


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


	image->data = malloc(sizeof(RGB*) * image->header.heigth);
	if (image->data == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (int i = 0; i < image->header.heigth; i++)
	{
		image->data[i] = malloc(sizeof(RGB) * image->header.width);
		if (image->data[i] == NULL){
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
			image->data[line][colone].B = hex; 
		else if (color == 1)
			image->data[line][colone].G = hex; 
		else 
			image->data[line][colone].R = hex; 
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
		free(image->data[i]);
	}
	free(image->data);
	free(image);
}

bool	check_bmp_header(BMPHEADER* bmp_header){
	/*
		a Header is valid if:
	       		- type is "BM"
			- Headersize is equal of the offset of dataimage	
	*/
	return	bmp_header->type == 0x4D42
		&& bmp_header->bits_per_pixel == 24
		&& bmp_header->heigth > 0
		&& bmp_header->width > 0;
}

void	printBitmap(BMPIMAGE image)
{
	printf("Bitmap size: %dx%d.\n", image.header.width, image.header.heigth);
	for(int y = 0; y < image.header.heigth; y++)
	{
		for(int x = 0; x < image.header.width; x++){
			printf("[");
			printf("%02x ", image.data[y][x].R);
			printf("%02x ", image.data[y][x].G);
			printf("%02x", image.data[y][x].B);
			printf("]");
		}
		printf("\n");
	}
}

BMPIMAGE_G	*GetBitmap_G(BMPIMAGE image)
{
	BMPIMAGE_G *image_g = malloc(sizeof(*image_g));
	if (image_g == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	image_g->width = image.header.width;
	image_g->heigth = image.header.heigth;
	image_g->data = malloc(sizeof(uint8_t) * image_g->heigth);
	if (image_g->data == NULL){
		perror("Allocation error.\n");
		exit(EXIT_FAILURE);	
	}
	for (int y = 0; y < image_g->heigth; y++)
	{
		image_g->data[y] = malloc(sizeof(uint8_t) * image_g->width);
		if (image_g->data[y] == NULL){
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);	
		}
		for (int x = 0; x < image_g->width; x++)
		{
			image_g->data[y][x] = (uint8_t)(image.data[y][x].R * 0.299 +
					image.data[y][x].G * 0.857 + image.data[y][x].B * 0.114);
		}
	}
	return image_g;
}

void	printBitmap_G(BMPIMAGE_G image){
	printf("Bitmap gray size: %dx%d.\n", image.width, image.heigth);
	for(int y = 0; y < image.heigth; y++)
	{
		for(int x = 0; x < image.width; x++){
			printf("[");
			printf("%02x", image.data[y][x]);
			printf("]");
		}
		printf("\n");
	}
}


