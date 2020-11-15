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




char pathimg[] = "Images/origin.bmp";
char pathimg_cpy[] = "Images/origin_copy.bmp";

int main()
{
	printf ("--- Presentation soutenance 1 ---\n\n");
	
	printf("Appuyez sur <ENTER> pour commencer.\n");
	getchar();	


	/*Loading image*/	
	printf ("1) Chargement d'une image\n");
	printf("	Chemin de l'image d'origine: %s\n", pathimg);
	BMPIMAGE *img = LoadBitmap(pathimg);
	printf("	Image chargee dans la mémoire.\n");
	SaveBitmap(img, pathimg_cpy);
	printf("	Une copie a ete sauvegardé dans : %s\n", pathimg_cpy);



	printf ("2) Rotation de l'image\n");
	printf("Appuyez sur <ENTER> pour continuer.\n");
	getchar();


	return 0;
}













