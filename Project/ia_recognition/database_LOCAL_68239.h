#ifndef DATABASE_H
#define DATABASE_H


typedef struct{
	double **images;
	double **labels;
	int32_t nb_images;
	int32_t size_images;
} TDB;

int32_t ReverseInt(int32_t i){
	unsigned char c1, c2, c3, c4;
	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;
	return ((int32_t)c1<<24)+((int32_t)c2<<16)+((int32_t)c3<<8)+c4;
}

TDB getTrainData(char *pathImages, char *pathLabels){
	
	FILE *fp1 = fopen(pathImages, "r");
	if (fp1 == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);	
	}
	FILE *fp2 = fopen(pathLabels, "r");
	if (fp2 == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);	
	}
	
	fseek(fp1, 4, SEEK_SET);
	fseek(fp2, 8, SEEK_SET);

	int32_t nb_images;
	int32_t size_y;
	int32_t size_x;
	fread(&nb_images, 1, 4, fp1);
	fread(&size_y, 1, 4, fp1);
	fread(&size_x, 1, 4, fp1);
	nb_images = ReverseInt(nb_images);
	size_y = ReverseInt(size_y);
	size_x = ReverseInt(size_x);
	int32_t size_image = size_y * size_x;

	TDB tdb = {
		NULL,
		NULL,
		nb_images,
		size_x
	};
	
	tdb.images = malloc(sizeof(double*) * nb_images); 
	tdb.labels = malloc(sizeof(double[26]) * nb_images); 
	for (int32_t i = 0; i < nb_images; i++){
		tdb.images[i] = malloc(sizeof(double) * size_image);
		tdb.labels[i] = malloc(sizeof(double) * 26);
		for (int32_t a = 0; a < size_image; a++){
			uint8_t value;
			fread(&value, 1, 1, fp1);
			tdb.images[i][a] = (double)value / 255.0;
		}
		char c;
		fread(&c, 1, 1, fp2);
		c--;
		for (int a = 0; a < 26; a++){
			tdb.labels[i][a] = a == c ? 1 : 0;
		}
	}
	return tdb;
}

void createTDB(double **images, char *labels, size_t nb, char *pathImages, char *pathLabels){
	int32_t mn1 = ReverseInt(2051);
	int32_t mn2 = ReverseInt(2049);
	int32_t nb_images = ReverseInt((int32_t)nb);
	int32_t size = ReverseInt(28);

	FILE *fp1 = fopen(pathImages, "wb");
	if (fp1 == NULL){
		printf("Unable to open the file.\n");
		exit(EXIT_FAILURE);
	}
	fwrite(&mn1, 1, sizeof(mn1), fp1);
	fwrite(&nb_images, 1, sizeof(nb_images), fp1);
	fwrite(&size, 1, sizeof(size), fp1);
	fwrite(&size, 1, sizeof(size), fp1);
	for (size_t y = 0; y < nb; y++){
		fwrite(images[y], 1, sizeof(double)*28*28, fp1);
	}
	fclose(fp1);
	FILE *fp2 = fopen(pathLabels, "wb");
	if (fp2 == NULL){
		printf("Unable to open the file.\n");
		exit(EXIT_FAILURE);
	}
	fwrite(&mn2, 1, sizeof(mn2), fp2);
	fwrite(&nb_images, 1, sizeof(nb_images), fp2);
	for (size_t y = 0; y < nb; y++){
		char c = labels[y] - 'A' + 1;
		fwrite(&c, 1, 1, fp2);
	}
	fclose(fp2);

}

void FreeTDB(TDB *tdb){
	for (uint32_t i = 0; i < 1; i++){
		free(tdb->images[i]);
		free(tdb->labels[i]);
	}
	free(tdb->images);
	free(tdb->labels);
}

#endif
