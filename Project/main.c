#include <stdio.h>
#include <stdlib.h>
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
#include "UI/src/UIgtk.h"


char *FILENAME = "chars_detected.txt";
char *NEURALNETWORKNAME = "neural_network.nn";

int isEqual(char *s1,char *s2)
{
    int i = 0;
    int res = 1;
    while (s1[i])
    {
        char c  = s1[i];
        if (s2[i])
        {
            if (c != s2[i])
                res = 0;

        }else
        {
            res = 0;
        }
        i++;
    }
    if (s2[i])
        res = 0;
    return res;
}

char * LaunchOCR(char *filepath, char *neuralNetworkFilepath, double rotation)
{
    size_t nb_chars = 0;
    int counter = 0;
    size_t sizeImage = 28;
    double *input = malloc(sizeof(double) * sizeImage * sizeImage);

    BMPIMAGE *img = LoadBitmap(filepath);
    img =  Rotate(rotation, img);
    img = Denoising(img);
    img = ToGrayBitmap(img);
    img = ToBlackWhite(img);
    BMPIMAGE **img_chars = DetectChars(img, &nb_chars, 0);
    char *text = malloc(sizeof(char) * (nb_chars + 1));
    NN neuralNetwork = LoadNN(neuralNetworkFilepath, 1);

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
    NN neuralNetwork;
    size_t size = 28;
    int nbHiddensLayer = 100;
    int nbOutputs = 26;
    double lr = 0.1;
    double regression = 0.85;
    TDB tdb = getTrainData(dataBaseImagesFilename, dataBaseLabelsFilename);

    if(neuralNetworkFileName == NULL)
        neuralNetwork = InitializeNN(size*size,nbHiddensLayer,nbOutputs);
    else
        neuralNetwork = LoadNN(neuralNetworkFileName, lr);


    neuralNetwork.lr = lr;
    for (int a = 0; a < iteration; a++){
        int i = 0;
        while (i < tdb.nb_images)
        {
            trainNN(&neuralNetwork, tdb.images[i], tdb.labels[i]);
            if(i%1000 == 0)
                *progression = (double)(a * tdb.nb_images + i) * 100.0 / (double) (tdb.nb_images * iteration);
            i++;
        }

        if(a % 4 == 0){
            neuralNetwork.lr *= regression;
        }

        SaveNN(&neuralNetwork, NEURALNETWORKNAME);
    }

    FreeTDB(&tdb);
    FreeNN(&neuralNetwork);
}

int main(int argc, char ** argv)
{
    if(argc == 2 && isEqual(argv[1], "-graph"))
    {
        printf("need to implement the graphic part\n");
        UI();
    } else if (argc == 5 && isEqual(argv[1], "-ocr"))
    {
        FILE *file = fopen(argv[2], "r");
        FILE *networkFile = fopen(argv[3], "r");
        double rotation = 0;
        sscanf(argv[4], "%lf", &rotation);
        char *text = "";


        if(file != NULL && networkFile != NULL)
        {
            text = LaunchOCR(argv[2], argv[3], rotation);
            size_t len = 0;
            printf("The OCR recognize this text :\n%s\nIt has been save in file %s\n", text, FILENAME);

            while(text[len] != '\0')
                len++;
            FILE *fileText = fopen(FILENAME, "w+");
            fwrite(text, 1, len, fileText);

            fclose(fileText);
            fclose(file);
            fclose(networkFile);
        } else
        {
            printf("Error while loading files\n");
        }

    } else if ((argc == 5 || argc == 6) && isEqual(argv[1], "-train"))
    {
        FILE *fileImages = fopen(argv[2], "r");
        FILE *fileLabels = fopen(argv[3], "r");
        FILE *networkFile = NULL;
        if(argc == 6)
            fopen(argv[5], "r");
        int interation = 0;
        double progression = 0;
        sscanf(argv[4], "%d", &interation);


        if(fileImages != NULL && fileLabels != NULL && (argc == 5 || networkFile != NULL))
        {
            if(networkFile != NULL)
                TrainIA(argv[2], argv[3], argv[5], interation, &progression);
            else
                TrainIA(argv[2], argv[3], NULL, interation, &progression);
            fclose(fileImages);
            fclose(fileLabels);
        }
        else
            printf("Error while loading files\n");


        if(networkFile != NULL)
            fclose(networkFile);

    }else
    {
        printf("Command Error, there is the commande you can do:\n");
        printf("-graph\n");
        printf("-ocr filepath neural_network_path rotation\n");
        printf("-train data_base_image_path data_base_labels_path iteration neural_network\n");
    }

    return 0;
}













