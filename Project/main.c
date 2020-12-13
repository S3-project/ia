#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "characters_detection/characters_detection.h"
#include "ia_recognition/ia.h"
#include "other/Bitmap/bitmap.h"
#include "pre-processing/Removing_Colors/rmcolors.h"
#include "pre-processing/Rotate/rotate.h"
#include "pre-processing/contrast.h"
#include "pre-processing/negative.h"
#include "UI/src/UIgtk.h"
#include "UI/ia_graph_train.h"

#define FILENAME "chars_detected.txt"
#define WRONGCOMMAND "Command Error, there is the commands you can do:\n\
	-help		<command>\n\
	-graph\n\
	-ocr		<image_path> <neural_network_path> <rotation>\n\
	-train		<database_images_path> <database_labels_path> <neural_network_path>\n\
	-train_new	<database_images_path> <database_labels_path> <neural_network_output>\n\
	-train2		<database_images_path> <database_labels_path> <neural_network_path>\n\
	-train2_new	<database_images_path> <database_labels_path> <neural_network_output>\n"
#define TRAIN_REFRESH_PRINT 1000


#define GRAPH_HELP "\
COMMAND -graph help:\n\n\
	Open The graphic version of the program.\n\
	Not implemented yet.\n"
#define OCR_HELP "\
COMMAND -ocr <image_path> <neural_network_path> <rotation> help:\n\n\
	<image_path> The path of the image to which you want extract the text. (*.bmp)\n\
	<neural_network_path> The path of the neural network which you want to use. (*.nn)\n\
	<rotation> The degree of rotation of the image. can be 0, -90, 90, -180, 180.\n\n\
	This command try to extract the text from an image with a neural network.\n\
	The output will be store in a file.\n"
#define TRAIN_HELP "\
COMMAND -train <database_images_path> <database_labels_path> <neural_network_path> help:\n\n\
	<database_images_path> The path of the binary file of the database images you want to use.\n\
	<database_labels_path> The path of the binary file of the database labels you want to use.\n\
	<neural_network_path> The path of the neural network which you want to use. It must have just one hidden layer (*.nn)\n\n\
	This command will train the neural network you passed in parameters.\n\
       	Others parameters will be required: the number of iterations and the learning rate.\n\
	The neural network will be trained iteration times by all the database.\
	The new neural network will replace the older.\n"
#define TRAIN_NEW_HELP "\
COMMAND -train_new <database_images_path> <database_labels_path> <neural_network_output> help:\n\n\
	<database_images_path> The path of the binary file of the database images you want to use.\n\
	<database_labels_path> The path of the binary file of the database labels you want to use.\n\n\
	<neural_network_path> The path of the trained neural network output.\n\
	This command will create and train a neural network with one hidden layer.\n\
       	Others parameters will be required: the number of iterations, the learning rate and the number of neurons of the hidden layer.\n\
	The neural network will be trained iteration times by all the database.\n\
	The neural network will store in the <neural_network_output>.\n"
#define TRAIN2_HELP "\
COMMAND -train2 <database_images_path> <database_labels_path> <neural_network_path> help:\n\n\
	<database_images_path> The path of the binary file of the database images you want to use.\n\
	<database_labels_path> The path of the binary file of the database labels you want to use.\n\
	<neural_network_path> The path of the neural network which you want to use. It must have two hidden layer (*.nn)\n\n\
	This command will train the neural network you passed in parameters.\n\
       	Others parameters will be required: the number of iterations and the learning rate.\n\
	The neural network will be trained iteration times by all the database.\
	The new neural network will replace the older.\n"
#define TRAIN2_NEW_HELP "\
COMMAND -train2_new <database_images_path> <database_labels_path> <neural_network_output> help:\n\n\
	<database_images_path> The path of the binary file of the database images you want to use.\n\
	<database_labels_path> The path of the binary file of the database labels you want to use.\n\
	<neural_network_path> The path of the trained neural network output.\n\n\
	This command will create and train a neural network with two hidden layer.\n\
       	Others parameters will be required: the number of iterations, the learning rate, \n\
	the number of neurons of the first hidden layer and the number of the second.\n\
	The neural network will be trained iteration times by all the database.\n\
	The neural network will store in the <neural_network_output>.\n"

/*
   NN -> NeuralNetwork
   TDB -> Train DataBase
   */



void Error(){
	printf("%s", WRONGCOMMAND);
	exit(EXIT_FAILURE);
}


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

char * LaunchOCR(char *filepath, char *neuralNetworkFilepath, double rotation, int denoising)
{
	/*
	 * Launch the detection of characters after having applied the rotation, denoising and made a picture in
	 * black and white
	 */
	size_t nb_chars = 0;
	int counter = 0;
	size_t sizeImage = 28;
	double *input = malloc(sizeof(double) * sizeImage * sizeImage);

	BMPIMAGE *img = LoadBitmap(filepath);
	img =  Rotate(rotation, img);
	if(denoising)
	{
		img = Denoising(img);
	}
	
	img = ToGrayBitmap(img);
	img = ToBlackWhite(img);
	BMPIMAGE **img_chars = DetectChars(img, &nb_chars, 0);
	char *text = malloc(sizeof(char) * (nb_chars + 1));

	// get the magic number of the file
	int32_t magic_nb = getFileMagicNumber(neuralNetworkFilepath);

	if (magic_nb == MAGIC_NB_NN){

		NN nn = LoadNN(neuralNetworkFilepath);
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

			text[i] =  predictNN(&nn, input);
		}
		text[nb_chars] = '\0';
		FreeNN(&nn);
	}
	else if (magic_nb == MAGIC_NB_NN2){
		NN2 nn = LoadNN2(neuralNetworkFilepath);
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

			text[i] =  predictNN2(&nn, input);
		}
		text[nb_chars] = '\0';
		FreeNN2(&nn);
	}else{
		perror("File format error.\n");
		Error();
	}

	for (size_t i = 0; i < nb_chars; i++)
	{
		FreeBitmap(img_chars[i]);
	}
	FreeBitmap(img);

	return text;
}

void TrainIA_NN(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName,
		int iteration, double lr, int nb_hiddens)
{
	/*
	 * This part train the neural network and print regularly the progression
	 * At each loop save the neural network in a file in the case of the program ended unexpectedly
	 */
	TDB tdb = getTrainData(dataBaseImagesFilename, dataBaseLabelsFilename);
	size_t size = 28;
	int nbOutputs = 26;
	NN nn;

	// get the magic number of the file
	int32_t magic_nb = getFileMagicNumber(neuralNetworkFileName);
	if(magic_nb != MAGIC_NB_NN){
		nn = InitializeNN(size*size,nb_hiddens,nbOutputs);
	}
	else
	{
		nn = LoadNN(neuralNetworkFileName);
	}
	nn.lr = lr;
	for (int a = 0; a < iteration; a++){
		int i = 0;
		while (i < tdb.nb_images)
		{
			trainNN(&nn, tdb.images[i], tdb.labels[i]);
			if(i%TRAIN_REFRESH_PRINT == 0){
				double p = (double)(a * tdb.nb_images + i) * 100.0 / (double) (tdb.nb_images * iteration);
				printf("\r--> %.2f %c", p, '%');
				fflush(stdout);
			}

			i++;
		}
		SaveNN(&nn, neuralNetworkFileName);
	}
	FreeTDB(&tdb);
	FreeNN(&nn);
}

void TrainIA_NN2(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName,
		int iteration, double lr, int nb_hiddens, int nb_hiddens2)
{
	/*
	 * This part train the neural network and print regularly the progression
	 * At each loop save the neural network in a file in the case of the program ended unexpectedly
	 */
	TDB tdb = getTrainData(dataBaseImagesFilename, dataBaseLabelsFilename);
	size_t size = 28;
	int nbOutputs = 26;
	NN2 nn;

	// get the magic number of the file
	int32_t magic_nb = getFileMagicNumber(neuralNetworkFileName);
	if(magic_nb != MAGIC_NB_NN2){
		nn = InitializeNN2(size*size,nb_hiddens,nb_hiddens2,nbOutputs);
	}
	else
	{       
		nn = LoadNN2(neuralNetworkFileName);
	}
	nn.lr = lr;
	for (int a = 0; a < iteration; a++){
		int i = 0;
		while (i < tdb.nb_images)
		{
			trainNN2(&nn, tdb.images[i], tdb.labels[i]);
			if(i%TRAIN_REFRESH_PRINT == 0){
				double p = (double)(a * tdb.nb_images + i) * 100.0 / (double) (tdb.nb_images * iteration);
				printf("\r--> %.2f %c", p, '%');
				fflush(stdout);
			}

			i++;
		}
		SaveNN2(&nn, neuralNetworkFileName);
	}
	FreeTDB(&tdb);
	FreeNN2(&nn);
}


void Train(char **argv, int argc)
{
	if (argc != 5){
		Error();
	}
	int32_t magic_nb = getFileMagicNumber(argv[4]);
	if (magic_nb != MAGIC_NB_NN){
		perror("File format error.\n");
		Error();
	}
	FILE *fileImages = fopen(argv[2], "r");
	FILE *fileLabels = fopen(argv[3], "r");

	if (fileImages == NULL || fileLabels == NULL){
		perror("Error while loading files\n");
		Error();
	}

	int interation;
	double lr;
	printf("number of iterations : ");
	scanf("%d", &interation);
	printf("learning rate : ");
	scanf("%lf", &lr);
	
	fclose(fileImages);
	fclose(fileLabels);
	printf("The neural network training is launched\nAt each loop it will be save under the following name: %s\n", argv[4]);
	TrainIA_NN(argv[2], argv[3], argv[4], interation, lr, -1);
	printf("\n");
}

void Train2(char **argv, int argc)
{
        if (argc != 5){
                Error();
        }
        int32_t magic_nb = getFileMagicNumber(argv[4]);
        if (magic_nb != MAGIC_NB_NN2){
                perror("File format error.\n");
                Error();
        }
        FILE *fileImages = fopen(argv[2], "r");
        FILE *fileLabels = fopen(argv[3], "r");

        if (fileImages == NULL || fileLabels == NULL){
                perror("Error while loading files\n");
                Error();
        }

        int interation;
        double lr;
        printf("number of iterations : ");
        scanf("%d", &interation);
        printf("learning rate : ");
        scanf("%lf", &lr);

        fclose(fileImages);
        fclose(fileLabels);
        printf("The neural network training is launched\nAt each loop it will be save under the following name: %s\n", argv[4]);
        TrainIA_NN2(argv[2], argv[3], argv[4], interation, lr, -1, -1);
        printf("\n");
}


void Train_new(char **argv, int argc)
{
        if (argc != 5){
                Error();
        }
        FILE *fileImages = fopen(argv[2], "r");
        FILE *fileLabels = fopen(argv[3], "r");

        if (fileImages == NULL || fileLabels == NULL){
                perror("Error while loading files\n");
                Error();
        }

        int interation;
        double lr;
	unsigned int nb_hiddens; 
        printf("number of iterations : ");
        scanf("%d", &interation);
        printf("learning rate : ");
        scanf("%lf", &lr);
        printf("number of hidden neurons : ");
        scanf("%u", &nb_hiddens);

        fclose(fileImages);
        fclose(fileLabels);
        printf("The neural network training is launched\nAt each loop it will be save under the following name: %s\n", argv[4]);
        TrainIA_NN(argv[2], argv[3], argv[4], interation, lr, nb_hiddens);
	printf("\n");
}

void Train2_new(char **argv, int argc)
{
        if (argc != 5){
                Error();
        }
        FILE *fileImages = fopen(argv[2], "r");
        FILE *fileLabels = fopen(argv[3], "r");

        if (fileImages == NULL || fileLabels == NULL){
                perror("Error while loading files\n");
                Error();
        }

        int interation;
        double lr;
        unsigned int nb_hiddens;
        unsigned int nb_hiddens2;
        printf("number of iterations : ");
        scanf("%d", &interation);
        printf("learning rate : ");
        scanf("%lf", &lr);
        printf("number of hidden neurons of the first layer : ");
        scanf("%u", &nb_hiddens);
        printf("number of hidden neurons of the second layer : ");
        scanf("%u", &nb_hiddens2);

        fclose(fileImages);
        fclose(fileLabels);
        printf("The neural network training is launched\nAt each loop it will be save under the following name: %s\n", argv[4]);
        TrainIA_NN2(argv[2], argv[3], argv[4], interation, lr, nb_hiddens, nb_hiddens2);
        printf("\n");
}


void Help(char *cmd){
	if (isEqual(cmd, "graph")){
		printf(GRAPH_HELP);
	}else if (isEqual(cmd, "ocr")){
		printf(OCR_HELP);
	}else if (isEqual(cmd, "train")){
		printf(TRAIN_HELP);
	}else if (isEqual(cmd, "train_new")){
		printf(TRAIN_NEW_HELP);
	}else if (isEqual(cmd, "train2")){
		printf(TRAIN2_HELP);
	}else if (isEqual(cmd, "train2_new")){
		printf(TRAIN2_NEW_HELP);
	}
	else{
		Error();
	}
}

int main(int argc, char ** argv)
{
	/*
	 * Launch the program in graphique or console mode
	 * You can do the neural network training or the detection in both mode
	 */
	rand_set_seed();
	if(argc == 2 && isEqual(argv[1], "-graph"))
	{
		UI();
	}
	else if (argc == 5 && isEqual(argv[1], "-ocr"))
	{
		FILE *file = fopen(argv[2], "r");
		FILE *networkFile = fopen(argv[3], "r");
		double rotation = 0;
		sscanf(argv[4], "%lf", &rotation);
		char *text = "";


		if(file != NULL && networkFile != NULL)
		{
			fclose(file);
			fclose(networkFile);
			text = LaunchOCR(argv[2], argv[3], rotation, 1);
			size_t len = 0;
			printf("The OCR recognize this text :\n%s\nIt has been save in file %s\n", text, FILENAME);

			while(text[len] != '\0')
				len++;
			FILE *fileText = fopen(FILENAME, "w+");
			fwrite(text, 1, len, fileText);
			fclose(fileText);
		}
		else
		{
			printf("Error while loading files\n");
			if(networkFile != NULL)
				fclose(networkFile);
			if(file != NULL)
				fclose(file);
		}

	}
	else if (argc > 1 && isEqual(argv[1], "-train"))
	{
		Train(argv, argc);
	}
	else if (argc > 1 && isEqual(argv[1], "-train_new"))
	{
		Train_new(argv, argc);
	}
	else if (argc > 1 && isEqual(argv[1], "-train2"))
	{
		Train2(argv, argc);
	}
	else if (argc > 1 && isEqual(argv[1], "-train2_new"))
	{
		Train2_new(argv, argc);
	}
	else if (argc == 3 && isEqual(argv[1], "-help"))
	{
		Help(argv[2]);
	}
	else
	{
		Error();
	}

	return 0;
}













