#ifndef IA_H
#define IA_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "matrix_math.h"
#include "database.h"

// NeuralNetwork => NN

typedef struct {
	unsigned int number_inputs;
	unsigned int number_hiddens;
	unsigned int number_outputs;
	double lr;
	double **hidden_weights;
	double **hidden_biais;
	double **output_weights;
	double **output_biais;
} NN;




NN InitializeNN(unsigned int nb_inputs, unsigned int nb_hiddens, unsigned int outputs);
double **RandomMatrix(unsigned int x, unsigned int y);
void trainNN(NN *nn, double *input_f, double *output_f);
char predictNN(NN *nn, double *input_f);
size_t indexOfMax(double *lst, size_t size);
void SaveNN(NN *nn, char *path);
NN LoadNN(char *path, double lr);
void FreeNN(NN *nn);
void FreeMatrix(double **matrix, unsigned int y);






NN InitializeNN(unsigned int nb_inputs, unsigned int nb_hiddens, unsigned int nb_outputs){
	NN nn = {
		nb_inputs,
		nb_hiddens,
		nb_outputs,
		0.1,
		NULL,
		NULL,
		NULL,
		NULL
	};
	nn.hidden_weights = RandomMatrix(nn.number_hiddens, nn.number_inputs); 
	nn.hidden_biais = RandomMatrix(nn.number_hiddens, 1); 
	nn.output_weights = RandomMatrix(nn.number_outputs, nn.number_hiddens); 
	nn.output_biais = RandomMatrix(nn.number_outputs, 1);
	return nn;	
}


//initialize a matrix of randoms
double **RandomMatrix(unsigned int x, unsigned int y){
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = rand_range(-1,1);
		}
	}
	return mat;
}

void print_matrix(double **m, size_t lx, size_t ly)
{
	for (size_t y = 0; y < ly; y++){
		for (size_t x = 0; x < lx; x++){
			printf("%.2f ", m[y][x]);
		}
		printf("\n");
	}
	printf("\n");
}

void trainNN(NN *nn, double *input_f, double *output_f)
{
	// make matrix with parameters
	double	*input[1] = {input_f};
	double	*output[1] = {output_f};

	// Find The outputs of the hidden neurones
	double **hidden_layer_activation = dot(input, nn->hidden_weights, 1, nn->number_inputs, nn->number_hiddens);
	hidden_layer_activation = add(hidden_layer_activation, nn->hidden_biais, nn->number_hiddens, 1); // update hidden_layer_activation
	double **hidden_layer_output = sigmoid(hidden_layer_activation, nn->number_hiddens, 1); // update hidden_layer_activation

	// Find The predicted output
	double **output_layer_activation = dot(hidden_layer_output, nn->output_weights, 1, nn->number_hiddens, nn->number_outputs);
	output_layer_activation = add(output_layer_activation, nn->output_biais, nn->number_outputs, 1); // update output_layer_activation
	double **predicted_output = sigmoid(output_layer_activation, nn->number_outputs, 1); // update hidden_layer_activation

	double **error = sub(output, predicted_output, nn->number_outputs, 1);
	double **d_predicted_output = sigmoid_derivative(predicted_output, nn->number_outputs, 1);
	d_predicted_output = mul(d_predicted_output, error, nn->number_outputs, 1); // update d_predicted_output

	double **output_weights_T = transpose(nn->output_weights, nn->number_outputs, nn->number_hiddens);
	double **error_hidden_layer = dot(d_predicted_output, output_weights_T, 1, nn->number_outputs, nn->number_hiddens);
	double **d_hidden_layer = sigmoid_derivative(hidden_layer_output, nn->number_hiddens, 1);
	d_hidden_layer = mul(d_hidden_layer, error_hidden_layer, nn->number_hiddens, 1); // update d_hidden_layer


	double **hidden_layer_output_T = transpose(hidden_layer_output, nn->number_hiddens, 1);
	double **offset_output_weights = dot(hidden_layer_output_T, d_predicted_output, nn->number_hiddens, 1, nn->number_outputs);
	offset_output_weights = mul_by_number(offset_output_weights, nn->lr, nn->number_outputs, nn->number_hiddens); // update offset_output_weights
	nn->output_weights = add(nn->output_weights, offset_output_weights, nn->number_outputs, nn->number_hiddens); // update nn

	double **offset_output_biais = sum(d_predicted_output, nn->number_outputs, 1);
	offset_output_biais = mul_by_number(offset_output_biais, nn->lr, nn->number_outputs, 1); // update offset_output_biais
	nn->output_biais = add(nn->output_biais, offset_output_biais, nn->number_outputs, 1); // update nn

	double **input_T = transpose(input, nn->number_inputs, 1);
	double **offset_hidden_weights = dot(input_T, d_hidden_layer, nn->number_inputs, 1, nn->number_hiddens);
	offset_hidden_weights = mul_by_number(offset_hidden_weights, nn->lr, nn->number_hiddens, nn->number_inputs); // update offset_output_weights
	nn->hidden_weights = add(nn->hidden_weights, offset_hidden_weights, nn->number_hiddens, nn->number_inputs); // update nn

	double **offset_hidden_biais = sum(d_hidden_layer, nn->number_hiddens, 1);
	offset_hidden_biais = mul_by_number(offset_hidden_biais, nn->lr, nn->number_hiddens, 1); // update offset_output_biais
	nn->hidden_biais = add(nn->hidden_biais, offset_hidden_biais, nn->number_hiddens, 1); // update nn

	FreeMatrix(hidden_layer_output, 1);

	FreeMatrix(predicted_output, 1);

	FreeMatrix(error, 1);
	FreeMatrix(d_predicted_output, 1);

	FreeMatrix(output_weights_T, nn->number_outputs);
	FreeMatrix(error_hidden_layer, 1);
	FreeMatrix(d_hidden_layer, 1);


	FreeMatrix(hidden_layer_output_T, nn->number_hiddens);
	FreeMatrix(offset_output_weights, nn->number_hiddens);

	FreeMatrix(offset_output_biais, 1);

	FreeMatrix(input_T, nn->number_inputs);
	FreeMatrix(offset_hidden_weights, nn->number_inputs);

	FreeMatrix(offset_hidden_biais, 1);

}


char predictNN(NN *nn, double *input_f)
{
	// make matrix with parameters
	double	*input[1] = {input_f};

	// Find The outputs of the hidden neurones
	double **hidden_layer_activation = dot(input, nn->hidden_weights, 1, nn->number_inputs, nn->number_hiddens);
	hidden_layer_activation = add(hidden_layer_activation, nn->hidden_biais, nn->number_hiddens, 1); // update hidden_layer_activation
	double **hidden_layer_output = sigmoid(hidden_layer_activation, nn->number_hiddens, 1); // update hidden_layer_activation

	// Find The predicted output
	double **output_layer_activation = dot(hidden_layer_output, nn->output_weights, 1, nn->number_hiddens, nn->number_outputs);
	output_layer_activation = add(output_layer_activation, nn->output_biais, nn->number_outputs, 1); // update output_layer_activation
	double **predicted_output = sigmoid(output_layer_activation, nn->number_outputs, 1); // update hidden_layer_activation

	FreeMatrix(hidden_layer_output, 1);

	size_t index = indexOfMax(predicted_output[0], nn->number_outputs) + 'A';
	FreeMatrix(predicted_output, 1);
	return index;
}

double getErrorNN(NN *nn, TDB *tdb, int number){
	number = number > tdb->nb_images ? tdb->nb_images : number;
	int count = 0;
	for (int i = 0; i < number; i++){
		char predict = predictNN(nn, tdb->images[i]);
		char result = (char)indexOfMax(tdb->labels[i], 26) + 'A';		
		if (predict == result){
			count++;
		}
	}
	return (double)count * 100.0 / number;
}

size_t indexOfMax(double *lst, size_t size){
	double max_value = lst[0];
	size_t max_index = 0;
	for (size_t i = 1; i < size; i++){
		if (lst[i] > max_value){
			max_value = lst[i];
			max_index = i;
		}
	}
	return max_index;
}


void SaveNN(NN *nn, char *path){
	FILE *fp = fopen(path, "wb");
	if (fp == NULL){
		printf("Unable to open the file.\n");
		exit(EXIT_FAILURE);
	}
	fwrite(&nn->number_inputs, 1, sizeof(nn->number_inputs), fp);
	fwrite(&nn->number_hiddens, 1, sizeof(nn->number_hiddens), fp);
	fwrite(&nn->number_outputs, 1, sizeof(nn->number_outputs), fp);
	
	for (uint32_t y = 0; y < nn->number_inputs; y++){
		for (uint32_t x = 0; x < nn->number_hiddens; x++){
			fwrite(&nn->hidden_weights[y][x], 1, sizeof(double), fp);
		}	
	}
	for (uint32_t x = 0; x < nn->number_hiddens; x++){
		fwrite(&nn->hidden_biais[0][x], 1, sizeof(double), fp);
	}
	for (uint32_t y = 0; y < nn->number_hiddens; y++){
		for (uint32_t x = 0; x < nn->number_outputs; x++){
			fwrite(&nn->output_weights[y][x], 1, sizeof(double), fp);
		}	
	}
	for (uint32_t x = 0; x < nn->number_outputs; x++){
		fwrite(&nn->output_biais[0][x], 1, sizeof(double), fp);
	}
	fclose(fp);
}

NN LoadNN(char *path, double lr){
	NN nn = {0,0,0,lr,NULL,NULL,NULL,NULL};
	FILE *fp = fopen(path, "r");
	if (fp == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);	
	}
	fread(&nn.number_inputs, sizeof(nn.number_inputs), 1, fp);
	fread(&nn.number_hiddens, sizeof(nn.number_hiddens), 1, fp);
	fread(&nn.number_outputs, sizeof(nn.number_outputs), 1, fp);

	nn.hidden_weights = malloc(sizeof(nn.hidden_weights) * nn.number_inputs);
	for (uint32_t y = 0; y < nn.number_inputs; y++){
		nn.hidden_weights[y] = malloc(sizeof(nn.hidden_weights[y]) * nn.number_hiddens);
		for (uint32_t x = 0; x < nn.number_hiddens; x++){
			fread(&nn.hidden_weights[y][x], sizeof(double), 1, fp);
		}
	}
	nn.hidden_biais = malloc(sizeof(nn.hidden_biais) * 1);
	nn.hidden_biais[0] = malloc(sizeof(nn.hidden_biais[0]) * nn.number_hiddens);
	fread(nn.hidden_biais[0], 1, sizeof(double) * nn.number_hiddens, fp);

	nn.output_weights = malloc(sizeof(nn.output_weights) * nn.number_hiddens);
	for (uint32_t y = 0; y < nn.number_hiddens; y++){
		nn.output_weights[y] = malloc(sizeof(nn.output_weights[y]) * nn.number_outputs);
		fread(nn.output_weights[y], 1, sizeof(double) * nn.number_outputs, fp);
	}
	nn.output_biais = malloc(sizeof(nn.output_biais) * 1);
	nn.output_biais[0] = malloc(sizeof(nn.output_biais[0]) * nn.number_outputs);
	fread(nn.output_biais[0], 1, sizeof(double) * nn.number_outputs, fp);
	
	fclose(fp);
	return nn;
}


// Free the memory of a NN
void FreeNN(NN *nn){
	FreeMatrix(nn->hidden_weights, nn->number_inputs);
	FreeMatrix(nn->hidden_biais, 1);
	FreeMatrix(nn->output_weights, nn->number_hiddens);
	FreeMatrix(nn->output_biais, 1);
}

// Free the memory of a matrix
void FreeMatrix(double **matrix, unsigned int y){
	for (unsigned int i = 0; i < y; i++){
		free(matrix[i]);
	}
	free(matrix);
}


#endif
