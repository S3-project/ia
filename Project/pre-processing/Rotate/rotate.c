#include "../../upload_and_black_white_image/Bitmap/bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/*
	Rotate the image for 90 or 180 or -90 or -180 degree
*/
BMPIMAGE *Rotate(int degree, BMPIMAGE *image)
{
	if(degree==90 || degree==180 || degree==-90 || degree==-180)
	{
		BMPIMAGE *new_image = malloc(sizeof(*new_image));
		if (new_image == NULL)
		{
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);
		}
		
		new_image->header = image->header;
		
		if(degree==180 || degree==-180)                   //180 and -180 degree
		{
			new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
			if (new_image->data == NULL)
			{
				perror("Allocation error.\n");
				exit(EXIT_FAILURE);
			}
			for(uint32_t i =0; i<new_image->header.heigth;i++)
			{
				new_image->data[new_image->header.heigth -1 -i] = malloc(sizeof(RGB) * new_image->header.width);
				if (new_image->data[new_image->header.heigth -1 -i] == NULL){
					perror("Allocation error.\n");
					exit(EXIT_FAILURE);
				}
				for(uint32_t j=0; j<new_image->header.width;j++)
				{
					new_image->data[new_image->header.heigth -1 -i][new_image->header.width -1 -j] = image->data[i][j];
				}
			}
		}
		
		
		if(degree==90)                                   //90 degree
		{
			new_image->header.heigth=image->header.width;
			new_image->header.width=image->header.heigth;
			
			new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
			if (new_image->data == NULL)
			{
				perror("Allocation error.\n");
				exit(EXIT_FAILURE);
			}
			for(size_t i =0; i<new_image->header.heigth;i++)
			{
				new_image->data[i] = malloc(sizeof(RGB) * new_image->header.width);
				if (new_image->data[i] == NULL){
					perror("Allocation error.\n");
					exit(EXIT_FAILURE);
				}
				for(size_t j=0; j<new_image->header.width;j++)
				{
					new_image->data[i][j] = image->data[image->header.heigth-j-1][i];
				}
			}
		}
		
		if(degree==-90)                                   //-90 degree
		{
			new_image->header.heigth=image->header.width;
			new_image->header.width=image->header.heigth;
			
			new_image->data = malloc(sizeof(RGB*) * new_image->header.heigth);
			if (new_image->data == NULL)
			{
				perror("Allocation error.\n");
				exit(EXIT_FAILURE);
			}
			for(uint32_t i =0; i<new_image->header.heigth;i++)
			{
				new_image->data[i] = malloc(sizeof(RGB) * new_image->header.width);
				if (new_image->data[i] == NULL){
					perror("Allocation error.\n");
					exit(EXIT_FAILURE);
				}
				for(uint32_t j=0; j<new_image->header.width;j++)
				{
					new_image->data[i][j] = image->data[j][image->header.width-i-1];
				}
			}
		}
		return(new_image);
	}
	else
	{
		perror("Degree not valid.\n");
		exit(EXIT_FAILURE);
	}
}
