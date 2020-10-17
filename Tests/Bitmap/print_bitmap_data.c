


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



	/*************************
	     Bitmap Structures
	*************************/
typedef struct{
	uint8_t R; /* Red componnent */
	uint8_t G; /* Green componnent */
	uint8_t B; /* Blue componnent */
} RGB;

struct BMP_HEADER_STCT
{
	uint16_t type; /* The type of the format. need to be 0x424d */
	uint32_t bfSize; /* The size of the file (header include) in bytes */
	uint16_t unused1; 
	uint16_t unused2;
	uint32_t imageDataOffset; /* Offset to the start of image data */
	uint32_t headerSize; /* The size of the second part of the header in bytes*/
	int32_t width; /* The width of the image in pixels*/
	int32_t heigth; /* The heigth of the image in pixels */
	uint16_t num_planes;
	uint16_t bits_per_pixel; /* The size of an unique pixel. need to be 24 bits*/
	uint32_t compression;
} __attribute__((packed));
typedef struct  BMP_HEADER_STCT BMPHEADER;

typedef struct
{
	BMPHEADER header; /* The information of the bitmap image*/
	RGB **data; /* Matrix that contains the RGBs value of each pixels*/
} BMPIMAGE;



	/*************************
		Prototipes
	*************************/

bool	check_bmp_header(BMPHEADER* bmp_header); /* Check the header of a bitmap header */


	/*************************
		CONSTANTS
	*************************/

const char *filename = "../../Ressources/images/test.bmp";


	/*************************
		Fonction principale
		recupère et affiche les données d'un fichier bitmap
		Header + data
	*************************/

int	main(){
	
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
	/*if (!check_bmp_header(&image->header))
	{
		perror("Invalid BMP format.\n");
		exit(EXIT_FAILURE);	
	}*/



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
	printf("\n");

	for(int y = 0; y < image->header.heigth; y++)
	{
		for(int x = 0; x < image->header.width; x++){
			printf("[");
			printf("%d ", image->data[y][x].R);
			printf("%d ", image->data[y][x].G);
			printf("%d ", image->data[y][x].B);
			printf("]");
		}
		printf("\n");
	}

  	printf("filetype = %d\n", image->header.type);
     	printf("bfSize = %d\n", image->header.bfSize);   
     	printf("unused1 = %d\n", image->header.unused1);   
     	printf("unused2 = %d\n", image->header.unused2);   
     	printf("imageDataOffset = %d\n", image->header.imageDataOffset);   
     	printf("headerSize = %d\n", image->header.headerSize);   
     	printf("width = %d\n", image->header.width);   
     	printf("heigth = %d\n", image->header.heigth);   
     	printf("num_planes = %d\n", image->header.num_planes);   
     	printf("bits_per_pixel = %d\n", image->header.bits_per_pixel);   
     	printf("compression = %d\n", image->header.compression);   

	fclose(fp);	
	free(image);
	return 0;
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


