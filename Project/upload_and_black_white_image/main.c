#include "Bitmap/bitmap.h"
#include <stdio.h>


int	main(){
	char *po = "../Ressources/images/shut.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	printBitmap_RGB(image);
	
	CalculBitmapGray(image);

	printBitmap_G(image);

	freeBitmap(image);
	return 0;
}
