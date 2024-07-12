#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char * argv[])
{
    if (argc < 2) { // verifica se o filename foi passado
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];

    // Create a shared memory region
    int* number = (int*) mmap(NULL, sizeof (int) , PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (number == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    *number = -1; // Inicializa o valor da memória compartilhada

    // a partir daqui temos 2 processos que rodam o mesmo programa
    int id;
    id = fork();

    if (id < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (id == 0) { // id 0 corresponde ao processo filho
        std::ifstream file(filename);
        if (!file.is_open()) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        int value;
        file >> value;
        *number = value;

        std::cout << "Fork Filho: " << *number << std::endl;
        if (munmap(number, sizeof(int)) == -1) {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    } else { // Processo pai
        wait(NULL); // Espera um processo filho terminar

        std::cout << "Fork Pai: " << *number << std::endl;

        // Unmap memory and exit parent process
        if (munmap(number, sizeof(int)) == -1) {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
        return 0;   
    }
}
