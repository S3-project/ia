#include "Bitmap/bitmap.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/test2.bmp";
	char *out = "../../Ressources/images/test2_g.bmp";
	BMPIMAGE *image = LoadBitmap(po);

	
	//	BMPIMAGE *image_g = ToGrayBitmap(image);


	SaveBitmap(image, out);

//	FreeBitmap(image);
	FreeBitmap(image);
	return 0;
}
