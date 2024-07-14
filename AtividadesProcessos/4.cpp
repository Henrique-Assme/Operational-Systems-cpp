/*
gcc -pthread ... 
*/

#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <pthread.h>

int number = -1;
int created = 0;
std::vector<int> vet;

void* workerA(void *thread_id) {
    std::cout << "Thread A: Digite o tamanho do vetor: ";
    std::cin >> number;
}

void* workerB(void *thread_id) {
    while(number == -1) {

    }
    if (number > 0) {
        for(int i = 0; i < number; ++i) {
            vet.push_back(i);
        }
        created = 1;
    }
}

void* workerC(void *thread_id) {
    while (created == 0) {

    }
    std::cout << "Thread C: Elementos do vetor: ";
    for(const int& val : vet) {
        std::cout << val << " ";
    }
}

int main(int argc, char * argv[])
{	
	pthread_t threads[3];
	pthread_create(&threads[0], NULL, workerC, (void *)0);
    pthread_create(&threads[1], NULL, workerB, (void *)1);
    pthread_create(&threads[2], NULL, workerA, (void *)2);

    for( int i = 0; i < 3; i++ )
		pthread_join(threads[i], NULL);
    return 0;
}
