#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

void rand_set_seed(void);
double rand_range(double min, double max);
double **dot(double **m1, double **m2, size_t y1, size_t x1_y2, size_t x2);	
double **sigmoid(double **m1, size_t lx, size_t ly);
double **sigmoid_derivative(double **m1, size_t lx, size_t ly);
double **add(double **m1, double **m2, size_t lx, size_t ly);
double **sub(double **m1, double **m2, size_t lx, size_t ly);
double **mul(double **m1, double **m2, size_t lx, size_t ly);
double **mul_by_number(double **m1, double value, size_t lx, size_t ly);
double **transpose(double **m1, size_t lx, size_t ly);
double **sum(double **m1, size_t lx, size_t ly);

// init seed for random
void rand_set_seed(void){
	time_t t;
	time(&t);
	srand((unsigned) t);
}

// random between min and max
double rand_range(double min, double max){
	return ((double)rand() * (max - min)) / (double)RAND_MAX + min;
}


// multiply two matrix
double **dot(double **m1, double **m2, size_t y1, size_t x1_y2, size_t x2){	
	double **mat = malloc(sizeof(double*) * y1);
	for(size_t i = 0; i < y1; i++){
		mat[i] = malloc(sizeof(double) * x2);
		for(size_t j = 0; j < x2; j++){
			mat[i][j] = 0;
			for(size_t n = 0; n < x1_y2; n++){
				mat[i][j] += m1[i][n] * m2[n][j];
			} 	
		}
	}
	return mat;
}




/*
 * Apply the sigmoid function of each elements of the matrix
 * And store the result in the matrix
 */
double **sigmoid(double **m1, size_t lx, size_t ly){
	for (size_t y = 0; y < ly; y++){
		for (size_t x = 0; x < lx; x++){
			m1[y][x] = 1.0 / (1.0 + exp(-m1[y][x]));
		}
	}
	return m1;
}

/*
 * Apply the sigmoid derivative function of each elements of the matrix
 * And return a new matrix
 */
double **sigmoid_derivative(double **m1, size_t lx, size_t ly){
	double **mat = malloc(sizeof(*mat) * ly);
	for (size_t y = 0; y < ly; y++){
		mat[y] = malloc(sizeof(mat) * lx);
		for (size_t x = 0; x < lx; x++){
			mat[y][x] = m1[y][x] * (1.0 - m1[y][x]);
		}
	}
	return mat;
}



/*
 * Take to matrix with the same size,
 * and do a sum of each element of the two matrix
 * And store the result in the matrix 1
 */
double **add(double **m1, double **m2, size_t lx, size_t ly){
	for (size_t y = 0; y < ly; y++){
		for (size_t x = 0; x < lx; x++){
			m1[y][x] = m1[y][x] + m2[y][x];
		}
	}
	return m1;
}

/*
 * Take to matrix with the same size,
 * and do a sub of each element of the two matrix
 *Create a new matrix 
 */
double **sub(double **m1, double **m2, size_t lx, size_t ly){
	double **mat = malloc(sizeof(*mat) * ly);
	for (size_t y = 0; y < ly; y++){
		mat[y] = malloc(sizeof(double) * lx);
		for (size_t x = 0; x < lx; x++){
			mat[y][x] = m1[y][x] - m2[y][x];
		}
	}
	return mat;
}

/*
 * Take to matrix with the same size,
 * and do a mul of each element of the two matrix
 * And store the result in the matrix 1
 */
double **mul(double **m1, double **m2, size_t lx, size_t ly){
	for (size_t y = 0; y < ly; y++){
		for (size_t x = 0; x < lx; x++){
			m1[y][x] = m1[y][x] * m2[y][x];
		}
	}
	return m1;
}

double **mul_by_number(double **m1, double value, size_t lx, size_t ly){
	for (size_t y = 0; y < ly; y++){
		for (size_t x = 0; x < lx; x++){
			m1[y][x] = m1[y][x] * value;
		}
	}
	return m1;
}

double **transpose(double **m1, size_t lx, size_t ly){
	double **mat = malloc(sizeof(*mat) * lx);
	for(size_t x = 0; x < lx; x++){
		mat[x] = malloc(sizeof(mat) * ly);
		for(size_t y = 0; y < ly; y++){
			mat[x][y] = m1[y][x];
		}
	}
	return mat;	
}

double **sum(double **m1, size_t lx, size_t ly)
{
	double **mat = malloc(sizeof(*mat));
	mat[0] = malloc(sizeof(double) * lx);
	for(size_t x = 0; x < lx; x++){
		mat[0][x] = 0;
		for(size_t y = 0; y < ly; y++){
			mat[0][x] += m1[y][x];
		}
	}
	return mat;
}


#endif
