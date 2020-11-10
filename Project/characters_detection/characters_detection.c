//
// Created by Thimot on 22/10/2020.
//

#include "characters_detection.h"
#include <stdio.h>
#include <stdlib.h>
#include "../upload_and_black_white_image/Bitmap/bitmap.h"

int IsLineWhite(BMPIMAGE *image, uint32_t y)
{
    uint32_t x = 0;
    int white = 1;
    size_t width = image->header.width;
    while(x < width && white == 1 && GetPixel(image, x, y).R == 255 && GetPixel(image, x, y).G == 255 && GetPixel(image, x, y).B == 255)
        x++;

    if(x != width)
        white = 0;

    return white;
}


int IsRowWhite(BMPIMAGE *image, uint32_t x)
{

    uint32_t y = 0;
    int white = 1;
    size_t height = image->header.heigth;
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
        name[j] = extension[k];
    name[j] = '\0';

    return name;
}

void SaveChar(BMPIMAGE **chars, size_t *size)
{
    uint64_t num = 0;
    char number[10 + 5];

    for(size_t i = 0; i < *size; i++, num++)
        SaveBitmap(chars[i], IntToNameFile(num, number));
}


BMPIMAGE ** GetLines (BMPIMAGE *image, size_t *size)
{
    uint32_t lines[image->header.heigth];
    uint32_t numberOfLines = 0;
    int above = 1;                                      //indicates if the current we are in inter-lines


    if (IsLineWhite(image, 0) == 0)
    {
        above = 0;
        lines[numberOfLines] = 0;
        numberOfLines++;
    }

    for(size_t y = 1; y < image->header.heigth; y++)
    {
        if(above == 1 && IsLineWhite(image, y) == 0)
        {
            above = 0;
            lines[numberOfLines] = y;
            numberOfLines++;
        }
        else if(above == 0 && IsLineWhite(image, y) == 1)
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

    *size = numberOfLines / 2;
    BMPIMAGE **imgLines = malloc(sizeof(*imgLines) * *size);


    for(uint32_t j = 0; j < numberOfLines; j += 2)
        imgLines[j / 2] = SubBitmap(image,0,lines[j], image->header.width, lines[j+1] - lines[j]);

    FreeBitmap(image);

    return imgLines;
}


BMPIMAGE ** GetChars(BMPIMAGE **lines, size_t *sizeLines, size_t *sizeChar)
{
    uint64_t maxChar = 0;
    for(size_t i = 0; i < *sizeLines; i++)
        maxChar += lines[i]->header.width;

    BMPIMAGE **temporaire = malloc(sizeof(*temporaire) * maxChar);

    for(uint32_t i = 0; i < *sizeLines; i++)
    {
        BMPIMAGE *line = lines[i];
        uint32_t row[line->header.width];
        uint32_t numberOfChar = 0;
        int before = 1;

        if (IsRowWhite(line, 0) == 0)
        {
            before = 0;
            row[numberOfChar] = 0;
            numberOfChar++;
        }
        for(size_t x = 1; x < line->header.width; x++)
        {
            if(before == 1 && IsRowWhite(line, x) == 0)
            {
                before = 0;
                row[numberOfChar] = x;
                numberOfChar++;
            }
            else if(before == 0 && IsRowWhite(line, x) == 1)
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

        for(size_t j = 0; j < numberOfChar; j += 2)
            temporaire[*sizeChar + j / 2] = SubBitmap(line,row[j],0, row[j + 1] - row[j], line->header.heigth);

        *sizeChar += numberOfChar / 2;
    }

    BMPIMAGE **imgChar = malloc(sizeof(*imgChar) * *sizeChar);
    for(size_t i = 0; i < *sizeChar; i++)
        imgChar[i] = temporaire[i];
    free(temporaire);
    return imgChar;
}


BMPIMAGE ** DetectChars(BMPIMAGE *image, int print)
{
    size_t mallocSizeLines = 0;
    size_t mallocSizeChars = 0;

    BMPIMAGE **lines = GetLines(image, &mallocSizeLines);
    BMPIMAGE **chars = GetChars(lines, &mallocSizeLines, &mallocSizeChars);

    if(print == 1)
        SaveChar(chars, &mallocSizeChars);

    for(size_t i = 0; i < mallocSizeLines; i++)
        FreeBitmap(lines[i]);
    free(lines);

    return chars;
}
