#include "Bitmap/bitmap.h"
#include "Removing_Colors/rmcolors.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	char *out2 = "../../Ressources/images/test2_rmc.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	
<<<<<<< HEAD
	//	BMPIMAGE *image_g = ToGrayBitmap(image);


	SaveBitmap(image, out);
=======
	BMPIMAGE *image_g = ToGrayBitmap(image);
	//BMPIMAGE *image_rmc = ToBlackWhite(image_g);

	//RGB color = GetPixel(image_g, 0, 0);

	//printf("%d\n", color.R);

	SaveBitmap(image_g, out);
	//SaveBitmap(image_rmc, out2);
>>>>>>> 583a91f3ee9b814187b3cf816c6dd41ecb898473

//	FreeBitmap(image);
	FreeBitmap(image);
<<<<<<< HEAD
=======
	FreeBitmap(image_g);
	//FreeBitmap(image_rmc);
>>>>>>> 583a91f3ee9b814187b3cf816c6dd41ecb898473
	return 0;
}
