#include "../other/Bitmap/bitmap.h"
#include "contrast.h"
#include "negative.h"

#include "Removing_Colors/rmcolors.h"
#include "Rotate/rotate.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/Hello_World_PowerPoint.bmp";
	char *out = "./test.bmp";
	char *out2 = "../../Ressources/images/Hello_World_Denoised.bmp";
	char *out3 = "../../Ressources/images/Rotated_Hello_World.bmp";
	
	BMPIMAGE *image = LoadBitmap(po);

    	//contrast(image, image->header.heigth, image->header.width);

	BMPIMAGE *rotateimage = Rotate(-45,image);
	BMPIMAGE *image_denoised = Denoising(rotateimage);
	//BMPIMAGE *image_g = ToGrayBitmap(image_denoised);

	//BMPIMAGE *image_BandW = ToBlackWhite(image_g);
	


	SaveBitmap(image, out);
	//SaveBitmap(image_BandW, out);
	SaveBitmap(image_denoised, out2);
	SaveBitmap(rotateimage, out3);

	FreeBitmap(image);
	//FreeBitmap(image_BandW);
	FreeBitmap(image_denoised);
	FreeBitmap(rotateimage);
	return 0;
}
