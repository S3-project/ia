#include "rmcolors.h"


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


/*
	Denoise the image by computing the average of the pixels around one pixel.
*/
BMPIMAGE *Denoising(BMPIMAGE *image)
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
	float sum_r=0;
	float sum_g=0;
	float sum_b=0;
	float sum_e=0;
	float average=0;
	float etype=0;
	float ecart=0;
	
	RGB rgb;
	int h=new_image->header.heigth;
	int w=new_image->header.width;
	if(h>=3 && w>=3)
	{
		for(int i =0; i<h;i++)
		{
			new_image->data[i] = malloc(sizeof(RGB) * new_image->header.width);
			if (new_image->data[i] == NULL){
				perror("Allocation error.\n");
				exit(EXIT_FAILURE);
			}
			for(int j=0; j<w;j++)
			{	
				if(i==0 || i==h-1 || j==0 || j==w-1)
				{
					rgb =GetPixel(image,j,i);
					GetRGB(rgb,&r,&g,&b);
					new_image->data[i][j].R = r;
					new_image->data[i][j].G = g;
					new_image->data[i][j].B = b;
				}
				else
				{
					for(int k=-1;k<2;k++)
					{
						rgb =GetPixel(image,j+k,i-1);  //We add the pixels near the pixel(i,j) to compute the average.
						GetRGB(rgb,&r,&g,&b);
						sum_r+=r;
						sum_g+=g;
						sum_b+=b; 
					}
					
					
					rgb =GetPixel(image,j-1,i);
					GetRGB(rgb,&r,&g,&b);
					sum_r+=r;
					sum_g+=g;
					sum_b+=b;
					
					rgb =GetPixel(image,j+1,i);
					GetRGB(rgb,&r,&g,&b);
					sum_r+=r;
					sum_g+=g;
					sum_b+=b;
					
					for(int u=-1;u<2;u++)
					{
						rgb =GetPixel(image,j+u,i+1);
						GetRGB(rgb,&r,&g,&b);
						sum_r+=r;
						sum_g+=g;
						sum_b+=b;
					}
					
					
					average= (sum_r+sum_g+sum_b)/24;
					
					rgb =GetPixel(image,j-1,i-1);  //This is to compute the etype
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j,i-1);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j+1,i-1);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j-1,i);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j+1,i);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j-1,i+1);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j,i+1);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					rgb =GetPixel(image,j+1,i+1);
					GetRGB(rgb,&r,&g,&b);
					sum_e+=((r+g+b)/3 -average)*((r+g+b)/3 -average);
					
					etype=sqrtf(sum_e/8);
					sum_e=0;
					
					rgb =GetPixel(image,j,i);
					GetRGB(rgb,&r,&g,&b);
					
					ecart=abs((int)((r+g+b)/3 -average));
					if(ecart>etype)
					{
						new_image->data[i][j].R = (int)sum_r/8;
						new_image->data[i][j].G = (int)sum_g/8;
						new_image->data[i][j].B = (int)sum_b/8;
					}
					else
					{
						new_image->data[i][j] = image->data[i][j];
					}
					sum_r=0;
					sum_g=0;
					sum_b=0;
				}
			}
		}
	}
	return new_image;
}
