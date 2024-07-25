/*gcc -pthread 3.cpp*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <limits>

#define BUFFER_SIZE 15
pthread_mutex_t mutex;

/*NÃO ALTERE ESSA VARIAVEL*/
int N = 0;

/*NAO ALTERE ESSE FUNCAO*/
void print_buffer(int i){
 for (int j = 0; j<i;j++) printf("#");
}

void* produtor(void *thread_id){
	while(1){
		pthread_mutex_lock(&mutex);
		if(N < BUFFER_SIZE) {
			print_buffer(N++); 
			printf("\n");
		} else {
			// printf("Buffer lotado");
		}
		pthread_mutex_unlock(&mutex); 
	}
}

void* consumidor(void *thread_id){
	while (1) {
		pthread_mutex_lock(&mutex);
		if(N > 0) {
			print_buffer(N--); 
			printf("\n");
		} else {
			// printf("Buffer vazio");
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char * argv[]){
	int consumidores = atol(argv[1]);
	int produtores = atol(argv[2]);
	pthread_t threads[consumidores+produtores];
	for(int t = 0; t < produtores; t++)
		pthread_create(&threads[t], NULL, produtor, (void *)t);

	for (int t = consumidores; t<consumidores+produtores;t++)
		pthread_create(&threads[t], NULL, consumidor, (void *)t);
	
	while(1);
	return 0;
}