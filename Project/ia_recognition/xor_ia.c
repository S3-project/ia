#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//function sigmoid
double Sigmoid(double x){
	return (1.0/(1.0 + exp(-x)));
}

//derivate of sigmoid
double Sigmoid_d(double x){
	return (x * (1.0 - x));
}

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
	for(int i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(int j = 0; j < x; j++){
			mat[i][j] = rand_0_1();
		}
	}
	return mat;
}

//init a list of randoms
double *init_bias(size_t size){
	double *lst = malloc(sizeof(lst) * size);
	for(int i = 0; i < size; i++){
		lst[i] = rand_0_1();
	}
	return lst;
}

//free weigth
void free_weights(double **mat,size_t y){
	for(int i = 0; i<y; i++){
		free(mat[i]);
	}
	free(mat);
}

//free bias
void free_bias(double *lst){
	free(lst);
}

//print a matrix
void print_weights(double **m, size_t x, size_t y)
{
        for(size_t i = 0; i < y; i++){
                for(size_t j = 0; j < x; j++){
                        printf("%lf ",m[i][j]);
                }
                printf("\n");
        }
}

//print a list
void print_bias(double *lst, size_t size){
	for(size_t i = 0; i < size; i++){
		printf("%lf ",lst[i]);
	}
	printf("\n");
}

//add two matrix
void add(double m1[], double m2[], size_t rows, size_t cols, double r[])
{
        for(size_t i = 0; i < rows * cols; i++){
                r[i] = m1[i] + m2[i];
        }
}


//multiply to matrix
/*void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[])
{
        for(size_t i = 0; i < r1; i++){
                for(size_t j = 0; j < c2; j++){
                        for(size_t n = 0; n < c1; n++){
                                r[i * c2 + j] += m1[i * c1 + n] * m2[n * c2 + j];
                        }
                }
        }
}*/

//multiply a matrix
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

int main(){
	//input and output of XOR
	double input[][2] = 
	{
		{0, 0},	
		{1, 0},
		{0, 1},
		{1, 1}
	};
	size_t input_rows = 4;
	size_t input_columns = 2;

	int exp_output[] = {0, 1, 1,0};
	size_t ouput_rows = 4;
	size_t output_columns = 1;
	
	//number of units
	size_t num_input_unit = 2;
	size_t num_output_unit = 1;
	size_t num_hidden_unit = 2;

	//other parameters
	double learning_rate= 0.01;
	int nb_iter = 10000;
	
	//init random weights and bias
	doseed();

	double **hidden_weights = init_weights(num_input_unit , num_hidden_unit);

	double *hidden_bias = init_bias(num_hidden_unit);

	double **output_weights = init_weights(num_hidden_unit , num_output_unit);

	double *output_bias = init_bias(num_output_unit);

	//print initial weights and bias
	printf("init hidden weights\n");
	print_weights(hidden_weights, num_input_unit, num_hidden_unit);

	printf("init hidden bias\n");
	print_bias(hidden_bias, num_hidden_unit);
	
	printf("init output wieghts\n");
	print_weights(output_weights, num_hidden_unit, num_output_unit);

	printf("init output bias\n");
	print_bias(output_bias, num_output_unit);

	//training algorithm
	for(int i = 0; i < nb_iter; i++){
		//forward propagation
		//hidden_layer_activation = input * hidden_weights
		double **hidden_layer_activation = mul(&input, hidden_weights, input_rows, input_columns, num_input_unit);

		//hidden_layer_activation += hidden_bias

		//hidden_layer_output = sigmoid(elt) of every elt in hidden_layer_activation

		//backward propagation

		//updating weights
		
	}

	//free the memory
	free_weights(hidden_weights, num_hidden_unit);
	free_weights(output_weights, num_output_unit);
	free_bias(hidden_bias);
	free_bias(output_bias);

	return 0;
}




