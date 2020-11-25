#include "../other/Bitmap/bitmap.h"

#include "Removing_Colors/rmcolors.h"
#include "Rotate/rotate.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/Hello_World_PowerPoint.bmp";
	char *out = "../../Ressources/images/BandW.bmp";
	char *out2 = "../../Ressources/images/Hello_World_Denoised.bmp";
	char *out3 = "../../Ressources/images/Rotated_Hello_World.bmp";
	
	BMPIMAGE *image = LoadBitmap(po);
	
	
	BMPIMAGE *rotateimage = Rotate(90,image);

	
	BMPIMAGE *image_denoised = Denoising(image);
	BMPIMAGE *image_g = ToGrayBitmap(image_denoised);

	BMPIMAGE *image_BandW = ToBlackWhite(image_g);
	


	SaveBitmap(image_g, out);
	SaveBitmap(image_BandW, out);
	SaveBitmap(image_denoised, out2);
	SaveBitmap(rotateimage, out3);

	FreeBitmap(image);
	FreeBitmap(image_BandW);
	FreeBitmap(image_denoised);
	FreeBitmap(rotateimage);
	return 0;
}
