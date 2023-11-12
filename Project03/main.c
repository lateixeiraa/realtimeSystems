#include <FreeRTOS.h>
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
}