#include "Bitmap/bitmap.h"
#include "Removing_Colors/rmcolors.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	char *out2 = "../../Ressources/images/test2_rmc.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	
	//	BMPIMAGE *image_g = ToGrayBitmap(image);


	SaveBitmap(image, out);
	BMPIMAGE *image_g = ToGrayBitmap(image);
	//BMPIMAGE *image_rmc = ToBlackWhite(image_g);

	//RGB color = GetPixel(image_g, 0, 0);

	//printf("%d\n", color.R);

	SaveBitmap(image_g, out);
	//SaveBitmap(image_rmc, out2);

//	FreeBitmap(image);
	FreeBitmap(image);
	FreeBitmap(image_g);
	//FreeBitmap(image_rmc);
	return 0;
}
