#include "Bitmap/bitmap.h"
#include <stdio.h>


int	main(){
	char *po = "../Ressources/images/shut.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	printBitmap(*image);
	
	BMPIMAGE_G *image_g = GetBitmap_G(*image);

	printBitmap_G(*image_g);
	freeBitmap(image);
	return 0;
}
