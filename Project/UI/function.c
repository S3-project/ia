#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ia_recognition/ia.h" 

#define TRAIN_REFRESH_PRINT 1000


void TrainIA_NN2(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName, int iteration, double lr, int nb_hiddens, int nb_hiddens2, int *stop, double *progression)
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
		nn = InitializeNN2(size*size,nb_hiddens, nb_hiddens2,nbOutputs);
	}
	else
	{
		nn = LoadNN2(neuralNetworkFileName);
	}
	nn.lr = lr;
	for (int a = 0; a < iteration && !*stop; a++){
		int i = 0;
		while (i < tdb.nb_images)
		{
			trainNN2(&nn, tdb.images[i], tdb.labels[i]);
			if(i%1000 == 0){
				*progression = (double)(a * tdb.nb_images + i) * 100.0 / (double) (tdb.nb_images * iteration);
			}

			i++;
		}
		SaveNN2(&nn, neuralNetworkFileName);
	}
	FreeTDB(&tdb);
	FreeNN2(&nn);
}





void *TrainNN(void *argv)
{
	void **arg = argv;
	char *TDBIm = (char*)arg[0];
	char *TDBLab = (char*)arg[1];
	char *nn = (char*)arg[2];
	int *iter = (int*)arg[3];
	double *lr = (double*)arg[4];
	int *hid = (int*)arg[5];
	int *hid2 = (int*)arg[6];
	int *stop = (int*)arg[7];
	double *progression = (double*)arg[8];
	TrainIA_NN2(TDBIm, TDBLab, nn, *iter, *lr, *hid, *hid2, stop, progression);
	
	pthread_exit(EXIT_SUCCESS);
}






void test(void *arg)
{
	pthread_t test1;
	printf("avant\n");
	pthread_create(&test1, NULL, TrainNN, NULL);
	printf("apres\n");
}



