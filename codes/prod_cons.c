#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

sem_t empty, full; // Declaração dos semáforos empty e full
int buffer[BUFFER_SIZE];
int index = 0;

void *producer(void *arg) {
    int item = 1;
    while (1) {
        sleep(1);

        sem_wait(&empty); // Bloqueia se o buffer estiver cheio (semáforo vazio)

        buffer[index] = item;
        printf("Produced: %d\n", item);
        item++;

        index = (index + 1) % BUFFER_SIZE;

        sem_post(&full); // Sinaliza que o buffer está cheio (incrementa semáforo cheio)
    }
}

void *consumer(void *arg) {
    while (1) {
        sleep(2);

        sem_wait(&full); // Bloqueia se o buffer estiver vazio (semáforo cheio)

        int consumed_item = buffer[index];
        printf("Consumed: %d\n", consumed_item);

        index = (index + 1) % BUFFER_SIZE;

        sem_post(&empty); // Sinaliza que o buffer está vazio (incrementa semáforo vazio)
    }
}

int main() {
    sem_init(&empty, 0, BUFFER_SIZE); // Inicializa o semáforo empty com o tamanho do buffer
    sem_init(&full, 0, 0); // Inicializa o semáforo full com 0 (buffer inicialmente vazio)

    pthread_t prod_thread, cons_thread;

    pthread_create(&prod_thread, NULL, producer, NULL); // Cria a thread do produtor
    pthread_create(&cons_thread, NULL, consumer, NULL); // Cria a thread do consumidor

    pthread_join(prod_thread, NULL); // Aguarda a thread do produtor terminar
    pthread_join(cons_thread, NULL); // Aguarda a thread do consumidor terminar

    sem_destroy(&empty); // Libera recursos do semáforo empty
    sem_destroy(&full); // Libera recursos do semáforo full

    return 0;
}

