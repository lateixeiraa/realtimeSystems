#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex; // Declaração do mutex
int shared_variable = 0; // Variável compartilhada

void *increment_thread(void *arg) {
    for (int i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&mutex); // Bloqueio do mutex
        shared_variable++; // Acesso à variável compartilhada
        pthread_mutex_unlock(&mutex); // Desbloqueio do mutex
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL); // Inicialização do mutex

    pthread_create(&thread1, NULL, increment_thread, NULL);
    pthread_create(&thread2, NULL, increment_thread, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Valor final da variável compartilhada: %d\n", shared_variable);

    pthread_mutex_destroy(&mutex); // Destruição do mutex

    return 0;
}


// compilar: gcc -o nome_arquivo nome_arquivo.c -lpthread
// executar: ./nome_arquivo
