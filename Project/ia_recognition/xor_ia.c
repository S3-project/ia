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
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = rand_0_1();
		}
	}
	return mat;
}


//init a list of randoms
double *init_bias(size_t size){
	double *lst = malloc(sizeof(lst) * size);
	for(size_t i = 0; i < size; i++){
		lst[i] = rand_0_1();
	}
	return lst;
}

//free weigth
void free_weights(double **mat,size_t y){
	for(size_t i = 0; i<y; i++){
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
/*void add(double m1[], double m2[], size_t rows, size_t cols, double r[])
{
        for(size_t i = 0; i < rows * cols; i++){
                r[i] = m1[i] + m2[i];
        }
}*/


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

//add two matrix
double **add(double **m1, double **m2, size_t x, size_t y){	
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = m1[i][j] + m2[i][j];
		}
	}
	return mat;
}

//add two matrix
double **sub(double **m1, double **m2, size_t x, size_t y){	
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = m1[i][j] - m2[i][j];
		}
	}
	return mat;
}

//multiply two matrix
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

//multiply two matrix
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
double **mul_x(double **m,double n, size_t x, size_t y){	
	double **mat = malloc(sizeof(*mat) * y);
	for(size_t i = 0; i < y; i++){
		mat[i] = malloc(sizeof(mat) * x);
		for(size_t j = 0; j < x; j++){
			mat[i][j] = n * m[i][j];
		}
	}
	return mat;
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

int main(){
	//		*input and output of XOR*
	/*double input[][2] = 
	{
		{0, 0},	
		{1, 0},
		{0, 1},
		{1, 1}
	};*/
	size_t input_rows = 4;
	size_t input_columns = 2;

	double in_tmp[] = {0,0,1,0,0,1,1,1};
	double **input = malloc(sizeof(*input) * input_rows);
	size_t n = 0;
	for(size_t i = 0; i < input_rows; i++){
		input[i] = malloc(sizeof(input) * input_columns);
		for(size_t j = 0; j < input_columns; j++){
			input[i][j] = in_tmp[n++];
			//printf("%lf\n",input[i][j]);
		}
	}
	//print_weights(input, 2, 4);

	//int exp_output[] = {0, 1, 1,0};
	
	size_t output_rows = 4;
	size_t output_columns = 1;

	double out_tmp[] = {0, 1, 1, 0};
	double **output = malloc(sizeof(*output) * output_rows);
	n = 0;
	for(size_t i = 0; i < output_rows; i++){
		output[i] = malloc(sizeof(output) * output_columns);
		for(size_t j = 0; j < output_columns; j++){
			output[i][j] = out_tmp[n++];
			//printf("%lf\n",output[i][j]);
		}
	}
	//print_weights(output, 1, 4);
	

	//		*number of units*
	size_t num_input_unit = 2;
	size_t num_output_unit = 1;
	size_t num_hidden_unit = 2;


	//		*other parameters*
	//double learning_rate= 0.01;
	int nb_iter = 10000;
	

	//		*init random weights and bias*
	doseed();

	double **hidden_weights = init_weights(num_input_unit , num_hidden_unit);

	double **hidden_bias = init_weights(1, num_hidden_unit);

	double **output_weights = init_weights(num_hidden_unit , num_output_unit);

	double **output_bias = init_weights(1, num_output_unit);


	//		*print initial weights and bias*
	printf("init hidden weights\n");
	print_weights(hidden_weights, num_input_unit, num_hidden_unit);

	printf("init hidden bias\n");
	print_weights(hidden_bias, 1, num_hidden_unit);
	
	printf("init output wieghts\n");
	print_weights(output_weights, num_hidden_unit, num_output_unit);

	printf("init output bias\n");
	print_weights(output_bias, 1, num_output_unit);


	//		*training algorithm*
	for(int i = 0; i < nb_iter; i++){
		//		*forward propagation*
		//hidden_layer_activation = input * hidden_weights
		double **hidden_layer_activation = mul(input, hidden_weights, input_rows, input_columns, num_input_unit);

		//hidden_layer_activation += hidden_bias
		hidden_layer_activation = add(hidden_layer_activation, hidden_bias, 1, num_hidden_unit); 

		//hidden_layer_output = sigmoid(elt) of every elt in hidden_layer_activation
		double **hidden_layer_output = sig_m(hidden_layer_activation, 1, num_hidden_unit);

		
		//output_layer_activation = hidden_layer_activation * output_weights
		double **output_layer_activation = mul(hidden_layer_activation, output_weights, num_hidden_unit,1 ,num_hidden_unit);

		//output_layer_activation += output_bias
		output_layer_activation = add(output_layer_activation, output_bias, 1, num_output_unit); 

		//output_layer_output = sigmoid(elt) of every elt in output_layer_activation
		double **predicted_output = sig_m(output_layer_activation, 1, num_output_unit);


		//		*backward propagation*
		//error output layer
		//basic error e = a - b
		double **error = sub(output, predicted_output, 1, num_output_unit);
		double err = error[0][0];

		//error using derivate of sigmoid
		predicted_output = sig_d_m(predicted_output, 1, num_output_unit);
		predicted_output = mul_x(predicted_output, err, 1, num_output_unit);

		//error hidden layer
		//double **trans_output_weights = ;
		//double **error_hl = 

		//		*updating weights*
	
		//free the memory	
		free_weights(hidden_layer_activation, num_hidden_unit);
		free_weights(output_layer_activation, num_output_unit);
		free_weights(hidden_layer_output, num_output_unit);
		free_weights(predicted_output, num_output_unit);
		free_weights(error, num_output_unit);
	}

	//free the memory
	free_weights(hidden_weights, num_hidden_unit);
	free_weights(output_weights, num_output_unit);
	free_weights(hidden_bias, num_hidden_unit);
	free_weights(output_bias, num_output_unit);
	free_weights(input, input_rows);
	free_weights(output, output_rows);
	

	return 0;
}




