#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definição da struct tarefa
typedef struct {
    char nome[10];
    int tempo_execucao;
    int periodo;
    int deadline;
} Tarefa;

// Função que realiza o cálculo do mmc
int mmc(int a, int b) {
    int maior, menor, resto, resultado;
    maior = (a > b) ? a : b; // Atribui o maior valor entre 'a' e 'b' à variável 'maior'
    menor = (a < b) ? a : b; // Atribui o menor valor entre 'a' e 'b' à variável 'menor'
    while (menor != 0) {
        resto = maior % menor;
        maior = menor;
        menor = resto;
    }
    resultado = (a * b) / maior;
    return resultado;
}

// Operações realizadas com as cargas
void processar_carga(Tarefa tarefas[], int n_tarefas, int carga) {
    int i;
    
    //Verifica escalonabilidade pelo Executivo cíclico
    // Calcula o ciclo menor e maior
    int ciclo_menor = tarefas[0].periodo;
    int ciclo_maior = tarefas[0].periodo;
    for (i = 1; i < n_tarefas; i++) {
        ciclo_menor = (ciclo_menor < tarefas[i].periodo) ? ciclo_menor : tarefas[i].periodo;
        ciclo_maior = mmc(ciclo_maior, tarefas[i].periodo);
    }

    // Verifica escalonabilidade com o teste de utilização 
    double utilizacao = 0; // Inicializa a variável utilizacao que armazenará a soma das utilizações das tarefas.
    for (i = 0; i < n_tarefas; i++) {
        utilizacao += ((double) tarefas[i].tempo_execucao) / tarefas[i].periodo; // Acumula a utilização de cada tarefa.
    }
    char *resultado;
    if (utilizacao <= 1) {
        resultado = "SIM";
    } else {
        resultado = "NÃO";
        // Define os valores de ciclo_menor e ciclo_maior como zero se não for escalonável
        ciclo_menor = 0;
        ciclo_maior = 0;
    }
    
    // Verifica escalonabilidade pelo Rate Monotonic (RM)
    double limite_rm = n_tarefas * (pow(2, (1.0 / n_tarefas)) - 1);
    char *resultado_rm;
    if (utilizacao <= limite_rm) {
        resultado_rm = "SIM";
    } else if (utilizacao > limite_rm && utilizacao <= 1) {
        resultado_rm = "INCONCLUSIVO";
    } else {
        resultado_rm = "NÃO";
    }
    
    // Verifica escalonabilidade pelo Earliest Deadline First (EDF)
    char *resultado_edf;
    if (utilizacao <= 1) {
        resultado_edf = "SIM";
    } else if (utilizacao > 1 && utilizacao < n_tarefas) {
        resultado_edf = "INCONCLUSIVO";
    } else {
        resultado_edf = "NÃO";
    }
    
    // Exibe os resultados
    printf("Carga %d\n", carga);
    printf("Executivo: %s\n", resultado);
    printf("Ciclo maior: %d\n", ciclo_maior);
    printf("Ciclo menor: %d\n", ciclo_menor);
    printf("RM: %s\n", resultado_rm);  
    printf("EDF: %s\n", resultado_edf);  
}

int main() {

    // Leitura das cargas presentes no arquivo .txt
    FILE *fp;
    char linha[256];
    int carga, n_tarefas = 0, i;
    Tarefa tarefas[10];  // assumindo no máximo 10 tarefas por carga

    fp = fopen("carga.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo carga.txt\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, sizeof(linha), fp) != NULL) {
        if (sscanf(linha, "%d", &carga) == 1) {
            if (n_tarefas > 0) {
                processar_carga(tarefas, n_tarefas, carga - 1);  // Processa a carga anterior
                n_tarefas = 0;  // Reseta o contador de tarefas para a próxima carga
            }
        } else {
            sscanf(linha, "%s\t%d\t%d\t%d", tarefas[n_tarefas].nome, &tarefas[n_tarefas].tempo_execucao,
                   &tarefas[n_tarefas].periodo, &tarefas[n_tarefas].deadline);
            n_tarefas++;
        }
    }

    // Processa a última carga
    processar_carga(tarefas, n_tarefas, carga);

    // Fecha o arquivo
    fclose(fp);

    return 0;
}
