#include "Bitmap/bitmap.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	
	BMPIMAGE *image_g = ToGrayBitmap(image);

	RGB color = GetPixel(image_g, 0, 0);

	printf("%d\n", color.R);

	SaveBitmap(image_g, out);

	FreeBitmap(image);
	FreeBitmap(image_g);
	return 0;
}
