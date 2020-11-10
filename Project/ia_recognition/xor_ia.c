#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//function sigmoid
double Sigmoid(double x){
	return (1.0/(1.0 + exp(-x)));
}

//derivate of sigmoid
double Sigmoid_d(double x){
	return (x * (1.0 - x));
}

//random between 0 and 1
double rand_0_1(void){
	return (rand() % RAND_MAX) / (double)RAND_MAX;
}

//random between min and max
double randbetween(double min, double max){
	return randfrac() * (max - min) + min;
}

//init seed for random
void doseed(void){
	
}

void xor_ia(){
	//input and output of XOR
	int input[][] = {{0, 0}, {1, 0}, {0, 1}, {1 , 1}};
	int exp_output[] = {0, 1 , 1 ,0};
	
	//number of units
	int num_input_unit = 2;
	int num_output_unit = 1;
	int num_hidden_unit = 2;

	//other parameters
	float learning_rate= 0.01;
	int nb_iter = 10000;
	
	//init random weights and bias
	


	//training algorithm
	for(int i = 0; i < nb_iter; i++){
		//forward propagation
		

		//backward propagation


		//updating weights
	}
}


