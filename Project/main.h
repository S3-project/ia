#ifndef MAIN_H
#define MAIN_H

char * LaunchOCR(char *filename, char *neuralNetworkFileName, double rotation);
void TrainIA(char *dataBaseImagesFilename, char *dataBaseLabelsFilename, char *neuralNetworkFileName, int iteration, double *progression);

#endif //MAIN_H
