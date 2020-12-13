#include "ia.h"
#include "database.h"
#include <stdio.h>
#include <string.h>

#define PERIOD_REFRESH 100
#define NB_IMAGES_GETERROR 1000
#define PATH_SAVE_GRAPH "train3_lr0.1_q0.85.csv"
#define TRAIN_NAME "train3_lr0.1_q0.85"



void train1(NN *nn, TDB *tdb, double lr, int count);
void train2(NN *nn, TDB *tdb, double lr, double q,  int count);
void train3(NN *nn, TDB *tdb, double lr, double q,  int count);
void train3_opti(NN *nn, TDB *tdb, double lr, double q,  int count); // do not save graph
void SaveAsCSV(char *path, double *data, size_t count);
void replaceWithComa(char *str, double value);

int main(){
	rand_set_seed();


	//NN nn = InitializeNN(28*28,20,26);
	NN nn = LoadNN("train_tdb2_20.nn");
	TDB tdb = getTrainData("../../Ressources/Lettres/image2_tdb", "../../Ressources/Lettres/label2_tdb"); 

	train3_opti(&nn, &tdb, 0.3, 1, 100);
	
	SaveNN(&nn, "train_tdb2_20.nn");



	double error = getErrorNN(&nn, &tdb, tdb.nb_images);
	printf("\n%lf\n", error);


	FreeTDB(&tdb);
	FreeNN(&nn);
	return 0;
}


/*
 *	lr -> learning rate
 *	count -> the number of loops that will do the training
 *
 * */
void train1(NN *nn, TDB *tdb, double lr, int count)
{
	int nb_saves = count * (tdb->nb_images / PERIOD_REFRESH) + 1;
	double *data = malloc(sizeof(double) * nb_saves);
	int index = 0;
	double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
	data[index++] = error;

	nn->lr = lr;
	for (int a  =0; a < count; a++){
		int i = 0;
		while (i + PERIOD_REFRESH < tdb->nb_images){
			for (int b = i; b < i + PERIOD_REFRESH; b++){
				trainNN(nn, tdb->images[b], tdb->labels[b]);
			}
			double percent = (double)(i+1) * 100.0 / (double) tdb->nb_images;
			printf("--> %.2f%c loop=%d\n", percent, '%', a);
			i += PERIOD_REFRESH;
			double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
			data[index++] = error;
		}
		for (int b = i; b < tdb->nb_images; b++){
			trainNN(nn, tdb->images[b], tdb->labels[b]);
		}
	}
	SaveAsCSV(PATH_SAVE_GRAPH, data, nb_saves);
	free(data);
	
}

/*
 *	lr -> learning rate
 *	q -> the value that change the lr every loops
 *	count -> the number of loops that will do the training
 *
 * */
void train2(NN *nn, TDB *tdb, double lr, double q, int count)
{
	int nb_saves = count * (tdb->nb_images / PERIOD_REFRESH) + 1;
	double *data = malloc(sizeof(double) * nb_saves);
	int index = 0;
	double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
	data[index++] = error;

	nn->lr = lr;
	for (int a  =0; a < count; a++){
		int i = 0;
		while (i + PERIOD_REFRESH < tdb->nb_images){
			for (int b = i; b < i + PERIOD_REFRESH; b++){
				trainNN(nn, tdb->images[b], tdb->labels[b]);
			}
			double percent = (double)(i+1) * 100.0 / (double) tdb->nb_images;
			printf("\r--> %.2f%c loop=%d\n", percent, '%', a);
			fflush(stdout);
			i += PERIOD_REFRESH;
			double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
			data[index++] = error;
		}
		for (int b = i; b < tdb->nb_images; b++){
			trainNN(nn, tdb->images[b], tdb->labels[b]);
		}
		nn->lr *= q;
	}
	SaveAsCSV(PATH_SAVE_GRAPH, data, nb_saves);
	free(data);
	
}

/*
 *	lr -> learning rate
 *	q -> the value that change the lr every loops
 *	count -> the number of loops that will do the training
 *
 * */
void train3(NN *nn, TDB *tdb, double lr, double q, int count)
{
	int nb_saves = count * (tdb->nb_images / PERIOD_REFRESH) + 1;
	double *data = malloc(sizeof(double) * nb_saves);
	int index = 0;
	double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
	data[index++] = error;

	nn->lr = lr;
	for (int a  =0; a < count; a++){
		int i = 0;
		while (i + PERIOD_REFRESH < tdb->nb_images){
			for (int b = i; b < i + PERIOD_REFRESH; b++){
				trainNN(nn, tdb->images[b], tdb->labels[b]);
			}
			double percent = (double)(i+1) * 100.0 / (double) tdb->nb_images;
			printf("\r--> %.2f%c loop=%d\n", percent, '%', a);
			fflush(stdout);
			i += PERIOD_REFRESH;
			double error = getErrorNN(nn, tdb, NB_IMAGES_GETERROR);
			data[index++] = error;
		}
		for (int b = i; b < tdb->nb_images; b++){
			trainNN(nn, tdb->images[b], tdb->labels[b]);
		}
		if(a % 4 == 0){
			nn->lr *= q;
		}
	}
	SaveAsCSV(PATH_SAVE_GRAPH, data, nb_saves);
	free(data);
	
}


/*
 *	lr -> learning rate
 *	q -> the value that change the lr every loops
 *	count -> the number of loops that will do the training
 *
 * */
void train3_opti(NN *nn, TDB *tdb, double lr, double q, int count)
{
	nn->lr = lr;
	for (int a  =0; a < count; a++){
		int i = 0;
		while (i + PERIOD_REFRESH < tdb->nb_images){
			for (int b = i; b < i + PERIOD_REFRESH; b++){
				trainNN(nn, tdb->images[b], tdb->labels[b]);
			}
			double percent = (double)(i+1) * 100.0 / (double) tdb->nb_images;
			printf("\r--> %.2f%c loop=%d", percent, '%', a);
			fflush(stdout);
			i += PERIOD_REFRESH;
		}
		for (int b = i; b < tdb->nb_images; b++){
			trainNN(nn, tdb->images[b], tdb->labels[b]);
		}
		if(a % 4 == 0){
			nn->lr *= q;
		}
	}
	
}


void SaveAsCSV(char *path, double *data, size_t count){
	FILE *fs = fopen(path, "w");
	if (fs == NULL){
		printf("Unable to create file.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(fs, "Step;Values %s\n", TRAIN_NAME);
	for (size_t i = 0; i < count; i++){
		char value[8];
		replaceWithComa(value, data[i]);
		fprintf(fs, "%ld;%s\n", i, value);
	}
	fclose(fs);
}

void replaceWithComa(char *str, double value){
	char str_value[8];
	snprintf(str_value, 7, "%.3f", value);
	int i = 0;
	while (str_value[i]){
		if (str_value[i] == '.')
			str[i] = ',';
		else
			str[i] = str_value[i];
		i++;
	}
	str[i] = '\0';
}


