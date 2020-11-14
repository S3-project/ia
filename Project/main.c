//
// Created by Thimot on 03/11/2020.
//

#include <stdio.h>

#include "main.h"
#include "other/Bitmap/bitmap.h"
#include "pre-processing/Removing_Colors/rmcolors.h"
#include "characters_detection/characters_detection.h"

void delay(int a)
{
	int b = 0;
	for (int i = 0; i < a*10000000; i++)
		b++;
}



void test1()
{
	printf("wesh la street du 92! damsdams bangbang");
}


void TestLoadImage(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Error Syntax\nSyntax: %s %s {imagePath}\n", argv[0], argv[1]);
		return;
	} 


	BMPIMAGE *img;
	printf("---- Test load image ----\n");
	printf("image path: '%s'\nLoading image...", argv[2]);
	img = LoadBitmap(argv[2]);
	printf("OK\n");
	printf("Printing image data...\n\n");
	PrintBitmap(img);

	FreeBitmap(img);
}



void TestToBlackWhite(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Error Syntax\nSyntax: %s %s {imagePath} {savePath}\n", argv[0], argv[1]);
		return;
	}

	BMPIMAGE *img;
	printf("---- Test save Black and White image ----\n");
	printf("image path: '%s'\nLoading image...", argv[2]);
	img = LoadBitmap(argv[2]);
	printf("OK\n");
	printf("Converting to black and white..");
	BMPIMAGE *gray = ToGrayBitmap(img);
	BMPIMAGE *black = ToBlackWhite(gray);
	printf("OK\n");


	printf("\nSaving the black and white image..\n");
	SaveBitmap(black, argv[3]);
	printf("The image have been saved at '%s'.\n", argv[3]);

	FreeBitmap(img);
	FreeBitmap(gray);
	FreeBitmap(black);
}









int	isEqual(char* s1, char *s2)
{
	int i = 0;
	int res = 1;
	while (s1[i])
	{
		char c  = s1[i];
		if (s2[i])
		{
			if (c != s2[i])
				res = 0;

		}else
		{
			res = 0;
		}
		i++;
	}
	if (s2[i])
		res = 0;
	return res;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Error Syntax\nSyntax: %s {str}\n", argv[0]);
		return 1;
	}

	if (isEqual(argv[1], "loadimage"))
		TestLoadImage(argc, argv);
	else if(isEqual(argv[1], "saveblackwhite"))
		TestToBlackWhite(argc, argv);
	else
	{
		printf("Test not Found: %s\n", argv[1]);
		return 1;
	}
	return 0;
}
