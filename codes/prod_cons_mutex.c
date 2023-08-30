#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

pthread_mutex_t mutex; // Mutex para proteger o buffer
sem_t empty, full;
int buffer[BUFFER_SIZE];
int index = 0;

void *producer(void *arg) {
    int item = 1;
    while (1) {
        sleep(1);

        sem_wait(&empty);

        pthread_mutex_lock(&mutex); // Bloqueia o mutex antes de acessar o buffer

        buffer[index] = item;
        printf("Produced: %d\n", item);
        item++;

        index = (index + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Desbloqueia o mutex após acessar o buffer

        sem_post(&full);
    }
}

void *consumer(void *arg) {
    while (1) {
        sleep(2);

        sem_wait(&full);

        pthread_mutex_lock(&mutex); // Bloqueia o mutex antes de acessar o buffer

        int consumed_item = buffer[index];
        printf("Consumed: %d\n", consumed_item);

        index = (index + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Desbloqueia o mutex após acessar o buffer

        sem_post(&empty);
    }
}

int main() {
    pthread_t prod_thread1, prod_thread2, cons_thread1, cons_thread2;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&prod_thread1, NULL, producer, NULL);
    pthread_create(&prod_thread2, NULL, producer, NULL);
    pthread_create(&cons_thread1, NULL, consumer, NULL);
    pthread_create(&cons_thread2, NULL, consumer, NULL);

    pthread_join(prod_thread1, NULL);
    pthread_join(prod_thread2, NULL);
    pthread_join(cons_thread1, NULL);
    pthread_join(cons_thread2, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

