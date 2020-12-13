#ifndef IA_GRAPH_TRAIN_H
#define IA_GRAPH_TRAIN_H




#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ia_recognition/ia.h"

#define TRAIN_REFRESH_PRINT 1000

typedef struct {
	char *TDB_IM;
	char *TDB_LB;
	char *NN_PATH;
	int ITERATION;
	double LR;
	int HL1;
	int HL2;
	int *STOP;
	double *PROGRESSION;
	int *RUNNING;
} GRAPH_TRAIN_INFO;

void *Train_graph_NN(void *train_info);

void TrainIA_graph_NN2(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName, int iteration, double lr, int nb_hiddens, int nb_hiddens2, int *stop, double *progression)
{
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
		while (i < tdb.nb_images && !*stop)
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



void *Train_graph_NN(void *train_info_p)
{
	printf("ok2\n");
	GRAPH_TRAIN_INFO *train_info=(GRAPH_TRAIN_INFO*) train_info_p;
	*train_info->RUNNING = 1;
	TrainIA_graph_NN2(train_info->TDB_IM, train_info->TDB_LB, train_info->NN_PATH, train_info->ITERATION, train_info->LR, train_info->HL1, train_info->HL2, train_info->STOP, train_info->PROGRESSION);
	*train_info->RUNNING  = 0;
	
	pthread_exit(EXIT_SUCCESS);
}





#endif //FUNCTION_H
