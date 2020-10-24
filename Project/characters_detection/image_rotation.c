//
// Created by Thimot on 22/10/2020.
//

#include "image_rotation.h"
#include <stdio.h>
#include <stdlib.h>
#include "../upload_and_black_white_image/Bitmap/bitmap.h"

int IsLineWhite(BMPIMAGE *image, uint32_t y, uint32_t width)
{
    uint32_t x = 0;
    int white = 1;
    while(x < width && white == 1 && GetPixel(image, x, y).R == 255 && GetPixel(image, x, y).G == 255 && GetPixel(image, x, y).B == 255)
        x++;

    if(x != width)
        white = 0;

    return white;
}

char*  IntToNameFile(int64_t x, char *name)                    //transform the int to a string and add the extension file
{
    char extension[5] = ".bmp\0";
    ltoa(x, name, 10);

    int j,k;
    for (j = 0; name[j]!='\0'; j++);

    for (k = 0; extension[k]!='\0'; k++, j++)
    {
        name[j] = extension[k];
    }
    name[j] = '\0';

    return name;
}

int GetLines (/*char *path*/)
{
    BMPIMAGE *image = LoadBitmap("../../Ressources/images/text2.bmp");
    uint32_t lines[image->header.heigth];
    uint32_t i = 0;
    int above = 1;                                      //indicates if the current we are in inter-lines


    if (IsLineWhite(image, 0, image->header.width) == 0)
    {
        above = 0;
        lines[i] = 0;
        i++;
    }

    for(uint32_t y = 1; y < image->header.heigth; y++)
    {
        if(above == 1 && IsLineWhite(image, y, image->header.width) == 0)
        {
            above = 0;
            lines[i] = y;
            i++;
        }
        else if(above == 0 && IsLineWhite(image, y, image->header.width) == 1)
        {
            above = 1;
            lines[i] = y - 1;
            i++;
        }
    }

    if(above == 0)
    {
        lines[i] = (image->header.heigth) - 1;
        i++;
    }

    int64_t num = 0;
    char name[10 + 5] = "";

    for(uint32_t j = 0; j < i - 1; j += 2)
    {
        BMPIMAGE *subImage = SubBitmap(image,0,lines[j], image->header.width, lines[j+1] - lines[j]);
        SaveBitmap(subImage,IntToNameFile(num, name));
        FreeBitmap(subImage);
        num++;
    }
    FreeBitmap(image);
    return 0;
}


int main(){
    GetLines();
    return 0;
}

