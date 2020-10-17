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
	int offset_endLine = (4 - (image->header.width * 3) % 4) % 4;
	int colone = 0;
	int line = image->header.heigth - 1;
	int color = 0;
	int byte_size = image->header.heigth * image->header.width * (image->header.bits_per_pixel / 8); 
	for(int i = 0; i < byte_size; i++){
		uint8_t hex;
		fread(&hex, 1, 1, fp);
		if (i == byte_size - 1)
			printf("%d   ", hex);
		if (color == 0)
			image->data_rgb[line][colone].B = hex; 
		else if (color == 1)
			image->data_rgb[line][colone].G = hex; 
		else 
			image->data_rgb[line][colone].R = hex; 
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
	for (int i = 0; i < image->header.heigth; i++){
		free(image->data_rgb[i]);
		if (image->data_g != NULL)
			free(image->data_g[i]);
	}
	free(image->data_rgb);
	free(image->data_g);
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
	Calcul the gray scale of the bitmap with this formula : (R+B+G) / 3, using the RGB Matrix "data_rgb"
	And stock the result in the Matrix "data_g"
*/
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
			int color = (image->data_rgb[y][x].R + image->data_rgb[y][x].G 
				+ image->data_rgb[y][x].B) / 3;
			image->data_g[y][x] = (uint8_t) color;
		}
	}
}


/*
	Print the Matrix of data_rgb on the terminal
*/
void	PrintBitmap_RGB(BMPIMAGE *image)
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





/*
	Print the Matrix of data_g on the terminal
	data_g need to be calculed before with "CalculBitmapGray()"  
*/
void	PrintBitmap_G(BMPIMAGE *image){
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



/*
	Verify if the gray scale Matrix "data_g" has been calculed
*/
bool	IsGrayBitmapCalculed(BMPIMAGE *image){
	return image->data_g != NULL;
}






/*
	Save the RGB bitmap ("data_rgb")
*/
void	SaveBitmap_RGB(BMPIMAGE *image, char *filename)
{
	FILE *fp = fopen(filename, "wb");

	fwrite(&image->header, 1, sizeof(image->header), fp);
	int offset_endLine = (4 - (image->header.width * 3) % 4) % 4;

	uint8_t null[3] = {0,0,0};
	for (int y = image->header.heigth - 1; y >= 0; y--){
		for (int x = 0; x < image->header.width; x++){
			for (int c = 0; c < 3; c++){
				if (c == 0)
					fwrite(&image->data_rgb[y][x].B, 1, 1, fp);
				else if(c == 1)
					fwrite(&image->data_rgb[y][x].G, 1, 1, fp);
				else {
					fwrite(&image->data_rgb[y][x].R, 1, 1, fp);
				}
			}
		}
		fwrite(&null, 1, offset_endLine, fp);
	}
	fclose(fp);
}



/*
	Save the RGB bitmap ("data_g")
*/
void	SaveBitmap_G(BMPIMAGE *image, char *filename)
{
	FILE *fp = fopen(filename, "wb");

	fwrite(&image->header, 1, sizeof(image->header), fp);
	int offset_endLine = (4 - (image->header.width * 3) % 4) % 4;

	uint8_t null[3] = {0,0,0};
	for (int y = image->header.heigth - 1; y >= 0; y--){
		for (int x = 0; x < image->header.width; x++){
			fwrite(&image->data_g[y][x], 1, 1, fp);
			fwrite(&image->data_g[y][x], 1, 1, fp);
			fwrite(&image->data_g[y][x], 1, 1, fp);
		}
		fwrite(&null, 1, offset_endLine, fp);
	}
	fclose(fp);
}
