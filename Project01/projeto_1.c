/**
 * Sistema de manufatura baseado no problema do produtor cnsumidor
 * Desenvolvedores: 
 * Larissa Teixeira da Silva - 119111077
 * Marcus Vinicius Costa Pereira - 119111203
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10         // Tamanho do buffer
#define NUM_PRODUCERS 3        // Número de produtores
#define NUM_CONSUMERS 2        // Número de consumidores

typedef struct {
    int id;
} Item;

sem_t empty, full, mutex;       // Semáforos para controle
Item buffer[BUFFER_SIZE];       // Buffer para os itens
int in = 0, out = 0;            // Índices para inserção e retirada
int cont = 0;

/**
 * Função que representa as máquinas produtoras do sistema
*/
void *producer(void *arg) {
    int producer_id = *(int *)arg; //Inicialização do produtor (0, 1 ou 2)

    while (1) {

        Item item;             // Inicializa a criação dos itens
        item.id = rand();      // Simula a produção de um item de fora aleatória

        sem_wait(&empty);       // Espera por um espaço vazio no buffer
        sem_wait(&mutex);       // Entra na seção crítica

        buffer[in] = item;      // Insere o item no buffer
        in = (in + 1) % BUFFER_SIZE; // Atualiza o índice de inserção
        
        cont ++; //Contador utilizado para verificação do preenchimento do buffer
                 //Nesse caso ele incrementa sempre que produz um item
                
        printf("Producer %d: Item %d produced\n", producer_id, item.id);
        printf("Buffer: %d\n\n", cont); 

        sem_post(&mutex);       // Sai da seção crítica
        sem_post(&full);        // Sinaliza que um novo item está no buffer

        sleep(1);               // Varia a taxa de produção
    }
}

/**
 * Função que representa as máquinas consumidoras do sistema
 * 
*/
void *consumer(void *arg) {
    int consumer_id = *(int *)arg;
    while (1) {
        sem_wait(&full);        // Espera por um item no buffer
        sem_wait(&mutex);       // Entra na seção crítica

        Item item = buffer[out]; // Remove um item do buffer
        
        out = (out + 1) % BUFFER_SIZE; // Atualiza o índice de retirada

        cont --; //Contador utilizado para verificação do preenchimento do buffer
                 //Nesse caso ele decrementa sempre que consome um item
                
        printf("Consumer %d: Item %d consumed\n", consumer_id, item.id);
        printf("\nBuffer: %d\n\n", cont);

        sem_post(&mutex);       // Sai da seção crítica
        sem_post(&empty);       // Sinaliza que há um espaço vazio no buffer

        sleep(2);               // Tempo de processamento
        
    }
}

int main() {
    srand(time(NULL));         // Inicializa o gerador de números aleatórios

    sem_init(&empty, 0, BUFFER_SIZE); // Inicializa semáforo de espaços vazios
    sem_init(&full, 0, 0);      // Inicializa semáforo de itens no buffer
    sem_init(&mutex, 0, 1);     // Inicializa semáforo para exclusão mútua

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    // Cria threads para produtores
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Cria threads para consumidores
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Aguarda o término das threads de produtores
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Aguarda o término das threads de consumidores
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    // Libera os semáforos
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
