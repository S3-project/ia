#include "../../other/Bitmap/bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>

#define M_PI 3.14159265358979323846







double **init_matrix(size_t c, size_t r){
	double **mat = malloc(sizeof(*mat) * r);
	for(size_t i = 0; i < r; i++){
		mat[i] = malloc(sizeof(mat) * c);
	}
	return mat;
}

double **mult(double **m1, double **m2, size_t r1, size_t c1_r2, size_t c2){
	double **mat = malloc(sizeof(*mat) * r1);
	for(size_t i = 0; i < r1; i++){
		mat[i] = malloc(sizeof(mat) * c2);
		for(size_t j = 0; j < c2; j++){
			mat[i][j] = 0;
			for(size_t n = 0; n < c1_r2; n++){
				mat[i][j] += m1[i][n] * m2[n][j];
			} 	
		}
	}
	return mat;
}

//free a matrix
void free_mat(double **mat,size_t y){
	for(size_t i = 0; i<y; i++){
		free(mat[i]);
	}
	free(mat);
}











/*
	Rotate the image for 90 or 180 or -90 or -180 degree
*/
BMPIMAGE *Rotate(double degree, BMPIMAGE *image)
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
	
		BMPIMAGE *new_image = malloc(sizeof(*new_image));
		if (new_image == NULL)
		{
			perror("Allocation error.\n");
			exit(EXIT_FAILURE);
		}
		new_image->header = image->header;
		
		if (new_image->header.heigth > new_image->header.width)
		{
			new_image->header.width=new_image->header.heigth;
		}
		else
		{
			new_image->header.heigth=new_image->header.width;
		}
		
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
				new_image->data[i][j].R = 255;
				new_image->data[i][j].G = 255;
				new_image->data[i][j].B = 255;	
			}
		}
		
		double **mat = init_matrix(2,2);
		double **mat_index = init_matrix(1,2);
		double angle=M_PI * degree/180;
		int mid_heigth = new_image->header.heigth / 2;
		int mid_width = new_image->header.width / 2;
		uint32_t _new_i=0;
		uint32_t _new_j=0;
		int i=0;
		int j=0;
		
		mat[0][0]=cos(angle);
		mat[0][1]=sin(angle);
		mat[1][0]=-sin(angle);
		mat[1][1]=cos(angle);
		for(uint32_t _i =0; _i < image->header.heigth;_i++)
		{
			for(uint32_t _j=0; _j < image->header.width;_j++)
			{
				i=_i;
				j=_j;
				i-=mid_heigth;
				j-=mid_width;
				
				
				mat_index[0][0]=i;
				mat_index[1][0]=j;
				
				
				double **mat_mul = mult(mat,mat_index,2,2,1);
				int new_i = mat_mul[0][0];
				int new_j = mat_mul[1][0];
				
				i+=mid_heigth;
				j+=mid_width;
				new_i+=mid_heigth;
				new_j+=mid_width;
				
				_new_i=new_i;
				_new_j=new_j;
				
				if(new_i<0)
					new_i=0;
				if(_new_i>=new_image->header.heigth)
					new_i=new_image->header.heigth-1;
				if(new_j<0)
					new_j=0;
				if(_new_j>=new_image->header.width)
					new_j=new_image->header.width-1;
				
				
				
				
				new_image->data[new_i][new_j]=image->data[i][j];
				free_mat(mat_mul,2);
			}
		}
		free_mat(mat,2);
		free_mat(mat_index,2);
		return new_image;
	}
}




