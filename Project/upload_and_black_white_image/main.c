#include "Bitmap/bitmap.h"
#include "Removing_Colors/rmcolors.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	char *out2 = "../../Ressources/images/test2_rmc.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	BMPIMAGE *image_g = ToGrayBitmap(image);


	BMPIMAGE *image_sub = SubBitmap(image_g, 0, 0, image_g->header.width, 500);


	SaveBitmap(image_g, out);
	SaveBitmap(image_sub, out2);

	FreeBitmap(image);
	FreeBitmap(image_sub);
	FreeBitmap(image_g);
	return 0;
}
