#include "../upload_and_black_white_image/Bitmap/bitmap.h"

#include "Removing_Colors/rmcolors.h"
#include "Rotate/rotate.h"
#include <stdio.h>


int	main(){
	char *po = "../../Ressources/images/receipt.bmp";
	char *out = "../../Ressources/images/BandW.bmp";
	char *out2 = "../../Ressources/images/New_receipt.bmp";
	
	
	BMPIMAGE *image = LoadBitmap(po);
	
	
	BMPIMAGE *rotateimage = Rotate(-180,image);

	//BMPIMAGE *image_g = ToGrayBitmap(image);
	
	//BMPIMAGE *image_denoised = Denoising(image);
	
	//BMPIMAGE *image_g = ToGrayBitmap(image_denoised);

	//BMPIMAGE *image_BandW = ToBlackWhite(image_g);
	
	
	
	//BMPIMAGE *image_BandW = ToBlackWhite(image_g);

	//BMPIMAGE *image_sub = SubBitmap(image_g, 0, 0, image_g->header.width, 500);


	//SaveBitmap(image_g, out);
	//SaveBitmap(image_BandW, out);
	SaveBitmap(rotateimage, out2);

	FreeBitmap(image);
	//FreeBitmap(image_sub);
	//FreeBitmap(image_g);
	//FreeBitmap(image_BandW);
	//FreeBitmap(image_denoised);
	FreeBitmap(rotateimage);
	return 0;
}
