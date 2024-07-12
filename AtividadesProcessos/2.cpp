#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>

int number = -1;
bool numberSet = false;
const char* filename;

void* threadB(void* arg) {
    // Abre o arquivo
    std::ifstream file(filename);
    if (!file.is_open()) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    // Lê o valor do arquivo
    int value;
    file >> value;
    number = value;

    std::cout << "Thread B: " << number << std::endl;
    numberSet = true;

    file.close();

    pthread_exit(NULL);
}

void* threadA(void* arg) {
    while(numberSet == false);

    std::cout << "Thread A: " << number << std::endl;

    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    if (argc < 2) { // verifica se o filename foi passado
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    filename = argv[1]; // Atribui o nome do arquivo à variável global

    // Thread ID
    pthread_t thread_id[2];

    pthread_create(&thread_id[0], NULL, threadA, NULL); // Cria a thread A
    pthread_create(&thread_id[1], NULL, threadB, NULL); // Cria a thread B

    pthread_join(thread_id[0], NULL); // Espera a thread A terminar

    return 0;
}
