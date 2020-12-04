#include <stdio.h>
#include <string.h>

#include "other/Bitmap/bitmap.h"
#include "ia_recognition/xor_ia.h"
#include "pre-processing/Removing_Colors/rmcolors.h"
#include "pre-processing/Rotate/rotate.h"
#include "characters_detection/characters_detection.h"



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
char pathimg_rot[] = "Images/origin_rot.bmp";
char pathimg_denoise[] = "Images/origin_denoise.bmp";
char pathimg_gray[] = "Images/origin_gray.bmp";
char pathimg_bin[] = "Images/origin_bin.bmp";

int main(int argc, char ** argv)
{


	if (argc <= 1)
	{
		printf("Erreur de syntaxe.\nSyntaxe: %s {cmd}\n", argv[0]);
		return 0;
	}

	if (isEqual(argv[1], "XOR") || isEqual(argv[1], "xor"))
	{
		if (argc != 4)
		{
			printf("Erreur de syntaxe.\nSyntaxe: %s %s {nb_iter} {learning_rate}\n", argv[0], argv[1]);
			return 0;
		}
		int nb_iter;
		double learning_rate;
		sscanf(argv[2], "%d", &nb_iter);
		sscanf(argv[3], "%lf", &learning_rate);
		xor(nb_iter, learning_rate);
		return 1;
	}else if (!isEqual(argv[1], "bitmaps"))
	{
		printf("La commande %s n'existe pas.\n", argv[1]);
		return 0;
	}




	printf ("--- Présentation soutenance 1 ---\n\n");
	
	printf("Appuyez sur <ENTREE> pour commencer.\n");
	getchar();	


	/*Loading image*/	
	printf ("1) Chargement d'une image\n");
	printf("	Chemin de l'image d'origine: %s\n	...\n", pathimg);
	BMPIMAGE *img = LoadBitmap(pathimg);
	printf("	Image chargée dans la mémoire.\n");
	SaveBitmap(img, pathimg_cpy);
	printf("	Une copie a été sauvegardée dans : %s\n", pathimg_cpy);


	printf ("\n\n");

	printf ("\n2) Rotation de l'image\n");
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	printf("	Degré de rotation : -90°\n	...\n");
	BMPIMAGE *img_rot =  Rotate(-90, img);
	printf("	Image a été tournée de -90°.\n");
	SaveBitmap(img_rot, pathimg_rot);
	printf("	Une copie a été sauvegardée dans : %s\n", pathimg_rot);


	printf ("\n\n");


	printf ("\n3) Déparasitage de l'image\n");
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	printf("	Déparasitage de l'image tournée\n	...\n");
	BMPIMAGE *img_denoise = Denoising(img_rot);
	SaveBitmap(img_denoise, pathimg_denoise);
	printf("	Une copie a été sauvegardée dans : %s\n", pathimg_denoise);
	printf("	Appuyez sur <ENTREE> pour continuer.");


	printf ("\n\n\n");


	printf ("\n4) Binarisation de l'image\n");
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	printf("	Convertion de l'image en niveau de gris.\n	...\n");
	BMPIMAGE *img_gray = ToGrayBitmap(img_denoise);
	SaveBitmap(img_gray, pathimg_gray);
	printf("	Une copie a été sauvegardée dans : %s\n", pathimg_gray);
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	printf("	Binarisation de l'image a partir de celle en niveau de gris.\n	...\n");
	BMPIMAGE *img_bin = ToBlackWhite(img_gray);
	SaveBitmap(img_bin, pathimg_bin);
	printf("	Une copie a été sauvegardée dans : %s\n", pathimg_bin);



	printf ("\n\n");



	/* Segmentation en lignes */
	printf ("\n5) Segmentation de l'image en lignes\n");
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	size_t nb_chars = 0;
	printf("	Séparation des lignes.\n	...\n");
	BMPIMAGE **img_chars = DetectChars(img_bin, &nb_chars, 1);
	printf("	Des copies ont été sauvegardées dans le dossier: Images/Lines/\n");
	printf("	Appuyez sur <ENTREE> pour continuer.");
	getchar();
	printf("	Séparation des caractères.\n	...\n");

	
	for (size_t i = 0; i < nb_chars; i++)
	{
    		char number[10 + 5];
    		char str[100] = "./Images/Characters/";
    		strcat(str, IntToNameFile(i, number));
		    SaveBitmap(img_chars[i], str);
	}

	printf("	Des copies ont été sauvegardées dans le dossier: Images/Characters/\n");

	for (size_t i = 0; i < nb_chars; i++)
	{

		FreeBitmap(img_chars[i]);
	}	
	FreeBitmap(img);
	FreeBitmap(img_rot);
	FreeBitmap(img_denoise);
	FreeBitmap(img_gray);
	FreeBitmap(img_bin);
	return 0;
}













