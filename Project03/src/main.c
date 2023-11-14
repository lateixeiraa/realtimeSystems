#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <semphr.h>
#include <stdbool.h>

#define MAX_MISSEIS 10

#define ATTACK_TASK_PRIORITY 1
#define DEFENSE_TASK_PRIORITY 1

// Estrutura para representar um míssil
typedef struct {
    int id;
    int trajetoria;
    int tempoImpacto;
    bool interceptado;
} Missil;

// Estrutura para armazenar informações sobre mísseis ativos
typedef struct {
    Missil misseis[MAX_MISSEIS];
    SemaphoreHandle_t semaforoMisseis;
    int count;
} MissilInfo;

MissilInfo misseisAtivos;

// Função para gerar um número aleatório de mísseis
int gerarNumeroAleatorioDeMisseis() {
    int numeroMaximoDeMisseis = 5;
    return (rand() % numeroMaximoDeMisseis) + 1;
}

// Função para gerar uma trajetória aleatória
int gerarTrajetoriaAleatoria() {
    int trajetoriaMaxima = 100; // Por exemplo, valores de 0 a 100
    return rand() % trajetoriaMaxima;
}

// Função para gerar um tempoImpacto aleatório
int gerarTempoImpactoAleatorio() {
    int tempoImpactoMaximo = 5; // Por exemplo, valores de 0 a 10
    return rand() % tempoImpactoMaximo + 1;
}

// Função para criar um atraso aleatório entre os ataques.
TickType_t gerarTempoAleatorioEmTicks() {
    int tempoSegundos = (rand() % 5) + 1; // Gera um número entre 1 e 5 segundos
    return pdMS_TO_TICKS(tempoSegundos * 1000); // Converte milissegundos em ticks
}

void ProcessoAtaqueTask(void *pvParameters) {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    while (1) {
        // Gera um número aleatório de mísseis para o ataque
        int numeroDeMisseis = gerarNumeroAleatorioDeMisseis();
        printf("Lancando ataque com %d misseis.\n", numeroDeMisseis);
        
        for (int i = 0; i < numeroDeMisseis; i++) {
            // Aloca memória para um novo míssil
            Missil *missil = malloc(sizeof(Missil)); // Aloca memória para o míssil
            // Define as propriedades do míssil (ID, trajetória, tempo de impacto)
            missil->id = i;
            missil->trajetoria = gerarTrajetoriaAleatoria();
            missil->tempoImpacto = gerarTempoImpactoAleatorio();

            // Adicionar o míssil à lista de mísseis ativos
            if (xSemaphoreTake(misseisAtivos.semaforoMisseis, portMAX_DELAY)) {
                if (misseisAtivos.count < MAX_MISSEIS) {
                    misseisAtivos.misseis[misseisAtivos.count] = *missil;
                    misseisAtivos.count++;
                }
                // Libera o semáforo após adicionar o míssil à lista
                xSemaphoreGive(misseisAtivos.semaforoMisseis);
            }
            printf("Missil ID: %d lancado. Trajetoria: %d, Tempo de Impacto: %d segundos.\n", missil->id, missil->trajetoria, missil->tempoImpacto);

        }
        vTaskDelay(gerarTempoAleatorioEmTicks());
    }
}

void ordenarMisseisPorTempoETrajetoria() {
    // n recebe o número total de mísseis ativos
    int n = misseisAtivos.count;

    // Loop externo que percorre todos os mísseis, exceto o último
    for (int i = 0; i < n-1; i++) {     
        for (int j = 0; j < n-i-1; j++) {
            // Condição para troca: se o tempo de impacto do míssil atual (j) é maior que o do próximo (j+1)
            // Ou se os tempos de impacto são iguais, mas a trajetória do atual é maior que a do próximo
            if (misseisAtivos.misseis[j].tempoImpacto > misseisAtivos.misseis[j+1].tempoImpacto ||
               (misseisAtivos.misseis[j].tempoImpacto == misseisAtivos.misseis[j+1].tempoImpacto && 
                misseisAtivos.misseis[j].trajetoria > misseisAtivos.misseis[j+1].trajetoria)) {
                // Trocar misseis[j] e misseis[j+1]
                Missil temp = misseisAtivos.misseis[j];
                misseisAtivos.misseis[j] = misseisAtivos.misseis[j+1];
                misseisAtivos.misseis[j+1] = temp;
            }
        }
    }
}

void ProcessoDefesaTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(misseisAtivos.semaforoMisseis, portMAX_DELAY)) {
            // Ordena os mísseis por tempo de impacto e trajetória
            // Mísseis com menor tempo de impacto e trajetória serão priorizados
            ordenarMisseisPorTempoETrajetoria();

            // Processa cada míssil na lista de mísseis ativos
            for (int i = 0; i < misseisAtivos.count; i++) {
                // Verifica se o míssil atual está direcionado para uma área habitada
                if (misseisAtivos.misseis[i].trajetoria > 50) {
                    // Míssil é considerado uma ameaça e é interceptado
                    printf("Missil ID: %d com trajetoria %d foi interceptado. Tempo de Impacto: %d segundos.\n", 
                           misseisAtivos.misseis[i].id, 
                           misseisAtivos.misseis[i].trajetoria,
                           misseisAtivos.misseis[i].tempoImpacto);
                } else {
                    // Se o míssil não representa ameaça, imprime uma mensagem correspondente
                    printf("Missil ID: %d com trajetoria %d nao e uma ameaca a area habitada. Tempo de Impacto: %d segundos.\n", 
                           misseisAtivos.misseis[i].id, 
                           misseisAtivos.misseis[i].trajetoria,
                           misseisAtivos.misseis[i].tempoImpacto);
                }
            }

            // Após processar todos os mísseis, reseta a contagem para evitar reprocessamento
            misseisAtivos.count = 0;
           
            // Libera o semáforo após a operação para permitir que outras tasks acessem a lista
            xSemaphoreGive(misseisAtivos.semaforoMisseis);
        }
        vTaskDelay(gerarTempoAleatorioEmTicks());
    }
}

int main(void) {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa a estrutura dos mísseis ativos e o semáforo
    misseisAtivos.count = 0;
    misseisAtivos.semaforoMisseis = xSemaphoreCreateMutex();

    if (misseisAtivos.semaforoMisseis == NULL) {
        printf("Falha ao criar semaforo.\n");
        return 1; // Falha na criação do semáforo
    }

    // Criação das tasks de ataque e defesa
    xTaskCreate(ProcessoAtaqueTask, "Ataque", 1000, NULL, ATTACK_TASK_PRIORITY, NULL);
    xTaskCreate(ProcessoDefesaTask, "Defesa", 1000, NULL, DEFENSE_TASK_PRIORITY, NULL);
    
    // Inicia o escalonador de tarefas
    vTaskStartScheduler();

    // Em condições normais, o escalonador só terminará se houver falta de memória
    for (;;) {}
    return 0;
}


/*#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <queue.h>

#define MAX_MISSEIS 10  // Capacidade máxima de armazenamento temporário

// Estrutura para representar um míssil
typedef struct {
    int id;
    int trajetoria;
    int tempoImpacto;
} Missil;

// Declaração da fila
QueueHandle_t filaMissil;

Missil misseisAmeacadores[MAX_MISSEIS];
int contadorMisseis = 0;

// Função para gerar um número aleatório de mísseis
int gerarNumeroAleatorioDeMisseis() {
    int numeroMaximoDeMisseis = 10;
    return (rand() % numeroMaximoDeMisseis) + 1;
}

// Função para gerar uma trajetória aleatória
int gerarTrajetoriaAleatoria() {
    int trajetoriaMaxima = 100; // Por exemplo, valores de 0 a 100
    return rand() % trajetoriaMaxima;
}

// Função para gerar um tempoImpacto aleatório
int gerarTempoImpactoAleatorio() {
    int tempoImpactoMaximo = 10; // Por exemplo, valores de 0 a 10
    return rand() % tempoImpactoMaximo;
}

// Função para criar um atraso aleatório entre os ataques.
TickType_t gerarTempoAleatorioEmTicks() {
    int tempoSegundos = (rand() % 5) + 1; // Gera um número entre 1 e 5 segundos
    return pdMS_TO_TICKS(tempoSegundos * 1000); // Converte milissegundos em ticks
}

// Função para simular o processo de ataque
void ProcessoAtaqueTask(void *pvParameters) {
    Missil missil;
    while (1) {
        int numeroDeMisseis = gerarNumeroAleatorioDeMisseis(); // Gera número aleatório de mísseis

         printf("Ataque com %d misseis.\n", numeroDeMisseis);

        for(int i = 0; i < numeroDeMisseis; i++) {
            missil.id = i;
            missil.trajetoria = gerarTrajetoriaAleatoria();
            missil.tempoImpacto = gerarTempoImpactoAleatorio();
            xQueueSend(filaMissil, &missil, portMAX_DELAY); // Envia para a fila
        }

        vTaskDelay(gerarTempoAleatorioEmTicks());
    }
}

// Função para simular o processo de defesa
void ProcessoDefesaTask(void *pvParameters) {
    Missil missilRecebido, missilPrioritario;
    bool missilPrioritarioDefinido = false;

    while (1) {
        if (xQueueReceive(filaMissil, &missilRecebido, portMAX_DELAY)) {
            // Análise da trajetória
            if (missilRecebido.trajetoria > 50 ) { // Supondo que >50 é ameaça
                
                // Simula a interceptação do míssil
                printf("Interceptando missil ID: %d, Trajetoria: %d, Tempo de Impacto: %d\n", missilRecebido.id, missilRecebido.trajetoria, missilRecebido.tempoImpacto);
            }
            else {
                printf("Missil #%d nao representa ameaca.\n", missilRecebido.id);
            }
        }
    }
}

int main(void) {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Criação da fila
    filaMissil = xQueueCreate(10, sizeof(Missil));

    // Inicializa os processos de ataque e defesa
    xTaskCreate(ProcessoAtaqueTask, "Ataque", 1000, NULL, 1, NULL);
    xTaskCreate(ProcessoDefesaTask, "Defesa", 1000, NULL, 1, NULL);

    vTaskStartScheduler(); // Inicia o escalonador de tarefas

    // Configura a comunicação entre processos
    // Inicia o monitoramento de áreas habitadas

    for( ;; );
}*/
