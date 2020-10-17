#include "Bitmap/bitmap.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	PrintBitmap_RGB(image);
	
	CalculBitmapGray(image);

	PrintBitmap_G(image);

	SaveBitmap_G(image, out);

	FreeBitmap(image);
	return 0;
}
