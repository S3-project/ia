#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//=======================*Function sigmoid*============================
//=====================================================================
//function sigmoid
double Sigmoid(double x){
	return (1.0/(1.0 + exp(-x)));
}

//derivate of sigmoid
double Sigmoid_d(double x){
	return (x * (1.0 - x));
}

//sigmoid of every value in a matrix
double **sig_m(double **m, size_t x, size_t y){	
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = Sigmoid(m[i][j]);
		}
	}
	return mat;
}

//sigmoid derivative of every value in a matrix
double **sig_d_m(double **m, size_t x, size_t y){	
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = Sigmoid_d(m[i][j]);
		}
	}
	return mat;
}
//=====================================================================

//=======================*Function to init with random numbers*========
//=====================================================================
//init seed for random
void doseed(void){
	time_t t;
	time(&t);
	srand((unsigned) t);
}

//random between 0 and 1
double rand_0_1(void){
	return (rand() % RAND_MAX) / (double)RAND_MAX;
}

//random between min and max
double randbetween(double min, double max){
	return rand_0_1() * (max - min) + min;
}

//init a matrix of randoms
double **init_weights(size_t x, size_t y){
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = rand_0_1();
		}
	}
	return mat;
}
//=====================================================================

//=======================*free and print a matrix*=====================
//=====================================================================
//free a matrix
void free_mat(double **mat,size_t y){
	for(size_t i = 0; i<y; i++){
		free(mat[i]);
	}
	free(mat);
}

//print a matrix
void print_mat(double **m, size_t x, size_t y)
{
	for(size_t i = 0; i < y; i++){
		for(size_t j = 0; j < x; j++){
			printf("%lf ",m[i][j]);
		}
		printf("\n");
	}
}
//=====================================================================

//=======================*operations between matrix*===================
//=====================================================================
//multiply two matrix using scalar multiplication
double **mul(double **m1, double **m2, size_t y1, size_t x1_y2, size_t x2){	
	double **mat = malloc(sizeof(*mat) * y1);
	for(size_t i = 0; i < y1; i++){
		mat[i] = malloc(sizeof(mat) * x2);
		for(size_t j = 0; j < x2; j++){
			mat[i][j] = 0;
			for(size_t n = 0; n < x1_y2; n++){
				mat[i][j] += m1[i][n] * m2[n][j];
			} 	
		}
	}
	return mat;
}

//mutiply every elt in a matrice by n
double ** mul_x(double **m,double n, size_t x, size_t y){
	for(size_t i = 0; i < y; i++){
		for(size_t j = 0; j < x; j++){
			m[i][j] *= n;
		}
	}
	return m;
}

//do the sum of each column and return it into a list
double **sum(double **matrix1, size_t x, size_t y)
{
	double **matrix = malloc(sizeof(*matrix));
	for(size_t i = 0; i < x; i++){
		matrix[i] = malloc(sizeof(double) * x);
		matrix[i][0] = 0;
		for(size_t j = 0; j < y; j++){
			matrix[0][i] += matrix1[j][i];
		}
	}
	return matrix;
}

//help function for calcul
void copy_line(double **matrix, double *lst, size_t y, size_t size_lst)
{
	matrix[y] = malloc(sizeof(double) * size_lst);
	for (size_t x = 0; x < size_lst; x++)
	{
		matrix[y][x] = lst[x];
	}
}

//help function for calcul
double * calcul2(double *list1, double *list2, size_t i, size_t j, int operation)
{
	size_t max = i;
	if(j > max)
		max = j;
	double *list = malloc(sizeof(list) * max);

	if(i < j)
		for(size_t k = 0; k < j; k++)
			list[k] = list1[0];
	else if (j < i)
		for(size_t k = 0; k < i; k++)
			list[k] = list2[0];
	else
		for (size_t x = 0; x < i; x++)
			list[x] = list1[x];


	if(operation == 0)
		if(j < i)
			for(size_t k = 0; k < max; k++)
				list[k] += list1[k];
		else
			for(size_t k = 0; k < max; k++)
				list[k] += list2[k];
	else if (operation == 1)
		if(j < i)
			for(size_t k = 0; k < max; k++)
				list[k] = list1[k] - list[k];
		else
			for(size_t k = 0; k < max; k++)
				list[k] -= list2[k];
	else if(operation == 2)
		if(j < i)
			for(size_t k = 0; k < max; k++)
				list[k] *= list1[k];
		else
			for(size_t k = 0; k < max; k++)
				list[k] *= list2[k];
	else
		if(j < i)
			for(size_t k = 0; k < max; k++)
				list[k] = list1[k] / list[k];
		else
			for(size_t k = 0; k < max; k++)
				list[k] /= list2[k];

	return list;
}

//do the operation on two matrix : 0 = +, 1 = -, 2 = *, 3 = /;
double  **calcul(double **matrix1,double  **matrix2,size_t x1, size_t y1, size_t x2, size_t y2, int operation)
{
	size_t max = y1;
	if(y2 > max)
		max = y2;
	double **matrix = malloc(sizeof(*matrix) * max);

	if(y1 < y2)
		for(size_t k = 0; k < y2; k++)
			copy_line(matrix, matrix1[0], k, x1);
	else if (y2 < y1)
		for(size_t k = 0; k < y1; k++)
			copy_line(matrix, matrix2[0], k, x2);
	else
	{
		for (size_t y = 0; y < y1; y++)
			copy_line(matrix, matrix1[y], y, x1);
	}

	if(y2 < y1)
		for (size_t k = 0; k < max; k++)
			matrix[k] = calcul2(matrix1[k], matrix[k], x1, x2, operation);
	else
		for (size_t k = 0; k < max; k++)
			matrix[k] = calcul2(matrix[k], matrix2[k], x1, x2, operation);

	return matrix;
}

//transpose a matrix
double **transpose(double **m, size_t x, size_t y){
	double **mat = malloc(sizeof(*mat) * x);
	for(size_t i = 0; i < x; i++){
		mat[i] = malloc(sizeof(mat) * y);
		for(size_t j = 0; j < y; j++){
			mat[i][j] = m[j][i];
		}
	}
	return mat;	
}
//=====================================================================

//=======================*training algorithm*==========================
//=====================================================================
int xor(int nb_iter, double learning_rate){

	//checking if nb_iter > 0
	if (nb_iter <= 0)
	{
		printf("nb_iter must be positive");
		return 0;
	}

	//=======================*initialazing the value*s=====================
	//=====================================================================	

	//=======================*expected input and output of XOR*============
	//=====================================================================
	//size of the input matrix
	size_t input_rows = 4;
	size_t input_columns = 2;

	//init of the input matrix
	double in_tmp[] = {0,0,0,1,1,0,1,1};
	double **input = malloc(sizeof(*input) * input_rows);
	size_t n = 0;
	for(size_t i = 0; i < input_rows; i++){
		input[i] = malloc(sizeof(input) * input_columns);
		for(size_t j = 0; j < input_columns; j++){
			input[i][j] = in_tmp[n++];
		}
	}

	//size of the ouput matrix
	size_t output_rows = 4;
	size_t output_columns = 1;

	//init of the output matrix
	double out_tmp[] = {0, 1, 1, 0};
	double **output = malloc(sizeof(*output) * output_rows);
	n = 0;
	for(size_t i = 0; i < output_rows; i++){
		output[i] = malloc(sizeof(output) * output_columns);
		for(size_t j = 0; j < output_columns; j++){
			output[i][j] = out_tmp[n++];
		}
	}


	//=======================*number of units*=============================
	//=====================================================================
	size_t num_input_unit = 2;
	size_t num_output_unit = 1;
	size_t num_hidden_unit = 2;


	//=======================*init random weights and bias*================
	//=====================================================================
	doseed();	//generating a new seed

	double **hidden_weights = init_weights(num_hidden_unit , num_input_unit);

	double **hidden_bias = init_weights(num_hidden_unit, 1);

	double **output_weights = init_weights(num_output_unit, num_hidden_unit);

	double **output_bias = init_weights(num_output_unit, 1);


	printf("Starting training of the XOR :\n");

	//=======================*begining of the training iteration*==========
	//=====================================================================
	for(int i = 0; i < nb_iter; i++){
		
		//display information during processing
		double percent = (double)(i+1) * 100.0 / (double)nb_iter; 
		printf("\rStatue : %.2f%c", percent, '%');
		fflush(stdout);
		
		//=======================*forward propagation*=========================
		//=====================================================================
		//hidden_layer_activation = input * hidden_weights
		double **hidden_layer_activation = mul(input, hidden_weights, input_rows, input_columns, num_input_unit);
		double ** hidden_layer_activation2 = calcul(hidden_layer_activation, hidden_bias, num_hidden_unit,
				input_rows,num_hidden_unit, 1, 0);	//hidden_layer_activation += hidden_bias

		//hidden_layer_output = sigmoid(elt) of every elt in hidden_layer_activation
		double **hidden_layer_output = sig_m(hidden_layer_activation2, num_hidden_unit, input_rows);


		//output_layer_activation = hidden_layer_activation * output_weights
		double **output_layer_activation = mul(hidden_layer_output, output_weights, input_rows, num_hidden_unit,
				num_output_unit);

		//output_layer_activation += output_bias
		double ** output_layer_activation2 = calcul(output_layer_activation, output_bias, num_output_unit, input_rows,
				num_output_unit,1, 0);

		//output_layer_output = sigmoid(elt) of every elt in output_layer_activation
		double **predicted_output = sig_m(output_layer_activation2, num_output_unit, input_rows);

		
		//=======================*backward propagation*========================
		//=====================================================================
		//error output layer
		//basic error e = a - b
		double **error = calcul(output, predicted_output, output_columns, output_rows, num_output_unit,
				input_rows, 1);

		//error using derivate of sigmoid
		double ** predicted_output_sig = sig_d_m(predicted_output, num_output_unit, input_rows);
		double **d_predicted_output = calcul(error, predicted_output_sig,num_output_unit, output_rows, num_output_unit,
				input_rows, 2);

		//basic error of the hidden layer
		double ** output_weights_T = transpose(output_weights, num_output_unit, num_hidden_unit);
		double ** error_hidden_layer = mul(d_predicted_output, output_weights_T, output_rows, num_output_unit,
				num_hidden_unit);
		
		//error of the hidden layer using derivative of sigmoid
		double ** hidden_layer_output_sig = sig_d_m(hidden_layer_output, num_hidden_unit, input_rows);
		double ** d_hidden_layer = calcul(error_hidden_layer, hidden_layer_output_sig, num_hidden_unit, output_rows,
				num_hidden_unit, input_rows, 2);

		//=======================*Updating weights and biases*=================
		//=====================================================================
		//updating output weights and biases
		double ** hidden_layer_output_T = transpose(hidden_layer_output, num_hidden_unit, input_rows);
		double ** hidden_layer_output_T2 = mul(hidden_layer_output_T, d_predicted_output,num_hidden_unit,input_rows,
				num_output_unit);
		double ** output_weights2 = calcul(output_weights,
				mul_x(hidden_layer_output_T2, learning_rate, num_output_unit,num_hidden_unit),
				num_output_unit, num_hidden_unit,num_output_unit, num_hidden_unit, 0);

		free_mat(output_weights, num_hidden_unit);
		output_weights = output_weights2;

		double ** outputbias2 = sum(d_predicted_output, num_output_unit, output_rows);
		//ATTENTION unsure variables
		double ** outputbias3 = calcul(output_bias, mul_x(outputbias2, learning_rate, 2, 1), num_output_unit, 1, 2, 1, 0);
		free_mat(output_bias, 1);
		output_bias = outputbias3;

		//updating hidden weights and biases
		double ** input_T = transpose(input, input_columns, input_rows);
		double ** hidden_weights2 = mul(input_T, d_hidden_layer, num_hidden_unit, input_rows, num_hidden_unit);
		double ** hidden_weights3 = calcul(hidden_weights, mul_x(hidden_weights2, learning_rate, num_hidden_unit, num_hidden_unit),
				num_hidden_unit, num_input_unit, num_hidden_unit,num_hidden_unit, 0);

		free_mat(hidden_weights, num_input_unit);
		hidden_weights = hidden_weights3;

		double ** hidden_bias2 = sum(d_hidden_layer, num_hidden_unit, output_rows);
		//ATTENTION unsure variables
		mul_x(hidden_bias2, 0.1, 2, 1);
		//ATTENTION unsure variables
		double ** hidden_bias3 = calcul(hidden_bias, hidden_bias2, num_hidden_unit, 1, 2, 1, 0);
		free_mat(hidden_bias, 1);
		hidden_bias = hidden_bias3;
		
		//=======================*Display*=====================================
		//=====================================================================
		//display of the last result of the forward propagation
		if (i == nb_iter - 1)
		{
			printf("\n\nResults :\n");
			printf("\nPredicted output\n");
			print_mat(predicted_output, num_output_unit, output_rows);
		}


		
		//=======================*free the memory in the for loop*=============	
		//=====================================================================
		free_mat(hidden_layer_activation, input_rows);
		free_mat(hidden_layer_activation2, input_rows);
		free_mat(output_layer_activation, input_rows);
		free_mat(output_layer_activation2, input_rows);
		free_mat(hidden_layer_output, input_rows);
		free_mat(predicted_output, input_rows);
		free_mat(error, output_rows);
		free_mat(predicted_output_sig, input_rows);
		free_mat(d_predicted_output,output_rows);
		free_mat(output_weights_T,num_output_unit);
		free_mat(error_hidden_layer, output_rows);
		free_mat(hidden_layer_output_sig, output_rows);
		free_mat(d_hidden_layer, output_rows);
		free_mat(hidden_layer_output_T, num_hidden_unit);
		free_mat(hidden_layer_output_T2, num_hidden_unit);
		free_mat(outputbias2, 1);
		free_mat(input_T, num_hidden_unit);
		free_mat(hidden_weights2, num_hidden_unit);
		free_mat(hidden_bias2, 1);
	}

	//=======================*Display*=====================================
	//=====================================================================
	//display the final bias and weights
	printf("\nFinal hidden weights: \n");
	print_mat(hidden_weights, num_hidden_unit, num_input_unit);
	printf("\nFinal hidden bias\n");
	print_mat(hidden_bias, num_hidden_unit, 1);
	printf("\nFinal output weights: \n");
	print_mat(output_weights, num_output_unit, num_hidden_unit);
	printf("\nFinal output bias\n");
	print_mat(output_bias, num_output_unit, 1);


	//=======================*free the memory*=============================
	//=====================================================================
	free_mat(hidden_weights, num_input_unit);
	free_mat(output_weights, num_hidden_unit);
	free_mat(hidden_bias, 1);
	free_mat(output_bias, 1);
	free_mat(input, input_rows);
	free_mat(output, output_rows);


	return 0;
}




