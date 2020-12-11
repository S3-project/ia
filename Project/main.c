#include <stdio.h>
#include <string.h>

#include "main.h"
#include "characters_detection/characters_detection.h"
#include "ia_recognition/database.h"
#include "ia_recognition/ia.h"
#include "other/Bitmap/bitmap.h"
#include "pre-processing/Removing_Colors/rmcolors.h"
#include "pre-processing/Rotate/rotate.h"
#include "pre-processing/contrast.h"
#include "pre-processing/negative.h"




char * LaunchOCR(char *filename, char *neuralNetworkFileName, double rotation)
{
    size_t nb_chars = 0;
    int counter = 0;
    size_t sizeImage = 28;
    double *input = malloc(sizeof(double) * sizeImage * sizeImage);

    BMPIMAGE *img = LoadBitmap(filename);
    img =  Rotate(rotation, img);
    img = Denoising(img);
    img = ToGrayBitmap(img);
    img = ToBlackWhite(img);
    BMPIMAGE **img_chars = DetectChars(img, &nb_chars, 0);
    char *text = malloc(sizeof(char) * (nb_chars + 1));
    NN neuralNetwork = LoadNN(neuralNetworkFileName, 1);

    for(size_t i = 0; i < nb_chars; i++)
    {
        counter = 0;
        negative(img_chars[i], sizeImage, sizeImage);
        for (size_t y = 0; y < img_chars[i]->header.heigth; y++)
        {
            for (size_t x = 0; x < img_chars[i]->header.width; x++)
            {
                input[counter++] = (double) img_chars[i]->data[y][x].R / 255.0;
            }
        }

        rand_set_seed(); //   CA LA FAUT LE METTRE OU ?

        text[i] =  predictNN(&neuralNetwork, input);
    }
    text[nb_chars] = '\0';


    for (size_t i = 0; i < nb_chars; i++)
    {
        FreeBitmap(img_chars[i]);
    }
    FreeBitmap(img);

    return text;
}

void TrainIA(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName, int iteration, double *progression)
{
    int PERIOD_REFRESH = 5;

    NN neuralNetwork;
    size_t size = 28;
    int nbHiddensLayer = 100;
    int nbOutputs = 26;
    double lr = 0.1;
    double regression = 0.85;
    char *exitPathName = "neural_network.nn";
    TDB tdb = getTrainData(dataBaseImagesFilename, dataBaseLabelsFilename);

    if(neuralNetworkFileName == NULL)
        neuralNetwork = InitializeNN(size*size,nbHiddensLayer,nbOutputs);
    else
        neuralNetwork = LoadNN(neuralNetworkFileName, lr);


    neuralNetwork.lr = lr;
    for (int a = 0; a < iteration; a++){
        int i = 0;
        while (i + PERIOD_REFRESH < tdb.nb_images){
            for (int b = i; b < i + PERIOD_REFRESH; b++){
                trainNN(&neuralNetwork, tdb.images[b], tdb.labels[b]);
            }
            if(i%1000 == 0)
                *progression = (double)(i+1) * 100.0 / (double) (tdb.nb_images * iteration);
            i += PERIOD_REFRESH;
        }
        for (int b = i; b < tdb.nb_images; b++){
            trainNN(&neuralNetwork, tdb.images[b], tdb.labels[b]);
        }
        if(a % 4 == 0){
            neuralNetwork.lr *= regression;
        }
    }


    SaveNN(&neuralNetwork, exitPathName);
    FreeTDB(&tdb);
    FreeNN(&neuralNetwork);
}

int main(/*int argc, char ** argv*/)
{

    return 0;
}













