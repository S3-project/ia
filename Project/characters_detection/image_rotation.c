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


int IsRowWhite(BMPIMAGE *image, uint32_t x, uint32_t height)
{
    uint32_t y = 0;
    int white = 1;
    while(y < height && white == 1 && GetPixel(image, x, y).R == 255 && GetPixel(image, x, y).G == 255 && GetPixel(image, x, y).B == 255)
        y++;

    if(y != height)
        white = 0;

    return white;
}


char*  IntToNameFile(int64_t x, char *name)                    //transform the int to a string and add the extension file
{
    char extension[5] = ".bmp\0";
    sprintf(name, "%ld", x);

    int j,k;
    for (j = 0; name[j]!='\0'; j++);

    for (k = 0; extension[k]!='\0'; k++, j++)
    {
        name[j] = extension[k];
    }
    name[j] = '\0';

    return name;
}


BMPIMAGE* GetLines (/*char *path*/)
{
    BMPIMAGE *image = LoadBitmap("../../Ressources/images/text.bmp");
    uint32_t lines[image->header.heigth];
    uint32_t numberOfLines = 0;
    int above = 1;                                      //indicates if the current we are in inter-lines


    if (IsLineWhite(image, 0, image->header.width) == 0)
    {
        above = 0;
        lines[numberOfLines] = 0;
        numberOfLines++;
    }

    for(uint32_t y = 1; y < (uint32_t)image->header.heigth; y++)
    {
        if(above == 1 && IsLineWhite(image, y, image->header.width) == 0)
        {
            above = 0;
            lines[numberOfLines] = y;
            numberOfLines++;
        }
        else if(above == 0 && IsLineWhite(image, y, image->header.width) == 1)
        {
            above = 1;
            lines[numberOfLines] = y - 1;
            numberOfLines++;
        }
    }

    if(above == 0)
    {
        lines[numberOfLines] = (image->header.heigth) - 1;
        numberOfLines++;
    }

    int64_t num = 0;
    //char name[10 + 5] = "";

    BMPIMAGE *imgLines = malloc(numberOfLines);

    for(uint32_t j = 0; j < numberOfLines - 1; j += 2)
    {
        BMPIMAGE *subImage = SubBitmap(image,0,lines[j], image->header.width, lines[j+1] - lines[j]);
        //SaveBitmap(subImage,IntToNameFile(num, name));
        imgLines[j] = *subImage;
        FreeBitmap(subImage);
        num++;
    }
    FreeBitmap(image);
    return imgLines;
}


void GetChars(BMPIMAGE *lines)
{
    for(uint32_t i = 0; i < sizeof(lines); i++)
    {
        BMPIMAGE *line = &lines[i];
        uint32_t row[line->header.width];
        uint32_t numberOfChar = 0;
        int before = 1;

        if (IsRowWhite(line, 0, line->header.heigth) == 0)
        {
            before = 0;
            row[numberOfChar] = 0;
            numberOfChar++;
        }

        for(uint32_t x = 1; x < (uint32_t)line->header.width; x++)
        {
            if(before == 1 && IsRowWhite(line, x, line->header.width) == 0)
            {
                before = 0;
                row[numberOfChar] = x;
                numberOfChar++;
            }
            else if(before == 0 && IsLineWhite(line, x, line->header.width) == 1)
            {
                before = 1;
                row[numberOfChar] = x - 1;
                numberOfChar++;
            }
        }

        if(before == 0)
        {
            row[numberOfChar] = (line->header.width) - 1;
            numberOfChar++;
        }
    }
}

int main(){
    BMPIMAGE  *imgLines = GetLines();
    //BMPIMAGE *test = &imgLines[0];
    //printf("%d\n", test->header.heigth);
    //printf("%d\n", test->header.heigth);
    //SaveBitmap(test, "TEST.bmp");
    printf("here");
    free(imgLines);

    return 0;
}

