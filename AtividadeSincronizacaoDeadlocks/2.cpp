/* g++ 2.cpp -pthread */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

int iteracoes = 10000;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int n = atoi(argv[1]);
    
    // Create a shared memory region
    int *acc = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pthread_mutex_t *mutex = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (acc == MAP_FAILED || mutex == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    *acc = 0;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutex_attr);

    pid_t pid;
    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) { // Child process
            int acc_local = 0;
            for (int j = 0; j < iteracoes; j++) {
                acc_local++;
            }

            pthread_mutex_lock(mutex);
            *acc += acc_local;
            pthread_mutex_unlock(mutex);

            munmap(acc, sizeof(int));
            munmap(mutex, sizeof(pthread_mutex_t));
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    /*NÃO ALTERE ESSE PRINTF*/
    printf("Acc Esperado [%d] Acc obtido [%d]\n", n * iteracoes, *acc);

    // Cleanup
    munmap(acc, sizeof(int));
    munmap(mutex, sizeof(pthread_mutex_t));
    pthread_mutexattr_destroy(&mutex_attr);
    
    return 0;
}