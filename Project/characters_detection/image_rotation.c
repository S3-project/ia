//
// Created by Thimot on 22/10/2020.
//

#include "image_rotation.h"
#include <stdio.h>
#include <stdlib.h>
#include "../upload_and_black_white_image/Bitmap/bitmap.h"

int CreateImageNew(BMPIMAGE *source, uint32_t start, uint32_t end, char *str)
{
    BMPIMAGE *image = malloc(sizeof(*image));
    if (image == NULL){
        perror("Allocation error.\n");
        exit(EXIT_FAILURE);
    }
    image->header = source->header;
    image->header.heigth = end - start;
    image->data = malloc(sizeof(RGB*) * image->header.heigth);
    if (image->data == NULL){
        perror("Allocation error.\n");
        exit(EXIT_FAILURE);
    }

    image->header.bfSize = 122 + (3* image->header.width +(4 - (image->header.width * 3) % 4) % 4)*image->header.heigth;

    printf("%d\n", image->header.heigth);

    for(uint32_t y = start; y < end; y++)
    {
        image->data[y - start] = malloc(sizeof(RGB) * image->header.width);
        if (image->data[y - start] == NULL){
            perror("Allocation error.\n");
            exit(EXIT_FAILURE);
        }
        for(uint32_t x = 0; x < source->header.width; x++)
        {
            image->data[y - start][x] = source->data[y][x];
        }
    }
    printf("%s\n", str);
    SaveBitmap(image, str);

    FreeBitmap(image);
    return 0;
}


int GetLines (/*BMPIMAGE *image*/)
{
    BMPIMAGE *image = LoadBitmap("../../Ressources/images/text.bmp");
    uint32_t lines[image->header.heigth];
    uint32_t i = 0;
    uint32_t followingWhite = 0;
    RGB white;
    white.G = 255;
    white.B = 255;
    white.R = 255;
    uint32_t x = 0;


    for(uint32_t y = 0; y < image->header.heigth; y++)
    {
        x = 0;

        while(x < image->header.width && GetPixel(image, x, y).R == white.R && GetPixel(image, x, y).G == white.G && GetPixel(image, x, y).B == white.B)
        {
            x++;
        }
        if(x == image->header.width && followingWhite == 0)
        {
            lines[i] = y;
            i++;
            followingWhite++;
        }
        else if(x == image->header.width)
        {
            followingWhite++;
        }
        else
        {
            followingWhite = 0;
        }
    }

    char tab[6] = "0.bmp\0";
    uint32_t k = '0';
    for(uint32_t j = 0; j < i; j ++)
    {
        CreateImageNew(image, lines[j], lines[j+1], tab);
        k++;
        tab[0] = k;
    }
    FreeBitmap(image);
    return 0;
}


int main(){
    GetLines();
    return 0;
}

