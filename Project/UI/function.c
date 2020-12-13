#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread1(void *arg)
{
	printf("dedans\n");
	pthread_exit(EXIT_SUCCESS);
}




void test(void *arg)
{
	pthread_t test1;
	printf("avant\n");
	pthread_create(&test1, NULL, thread1, NULL);
	printf("apres\n");
}



