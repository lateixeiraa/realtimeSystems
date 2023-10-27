#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Definição da struct tarefa
typedef struct {
    char nome[10];
    int tempo_execucao;
    int periodo;
    int deadline;
} Tarefa;

// Função que calcula o mdc
int mdc(int a, int b) {
    int resto;
    while (b != 0) {
        resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}

// Função que calcula o mmc
int mmc(int a, int b) {
    // Chama a função mdc e utiliza a fórmula do MMC
    return (a * b) / mdc(a, b);
}

// Função auxiliar para ordenar as tarefas por período
int comparar_tarefas(const void *a, const void *b) {
    Tarefa *tarefaA = (Tarefa *)a;
    Tarefa *tarefaB = (Tarefa *)b;
    return tarefaA->periodo - tarefaB->periodo;
}

// Função para verificar escalonabilidade pelo Executivo Cíclico
void verificar_executivo_ciclico(Tarefa tarefas[], int n_tarefas) {
    int i;
        
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

    // Exibe os resultados
    //printf("Carga %d\n", carga);
    printf("Executivo: %s\n", resultado);
    printf("Ciclo maior: %d\n", ciclo_maior);
    printf("Ciclo menor: %d\n", ciclo_menor);
}

// Função para verificar escalonabilidade pelo Rate Monotonic
void verificar_rate_monotonic(Tarefa tarefas[], int n_tarefas) {
    int i;
    // Verifica escalonabilidade pelo Rate Monotonic (RM)
    qsort(tarefas, n_tarefas, sizeof(Tarefa), comparar_tarefas);
    double utilizacao = 0;
    for (i = 0; i < n_tarefas; i++) {
        utilizacao += ((double) tarefas[i].tempo_execucao) / tarefas[i].periodo;
    }
    
    double limite_rm = n_tarefas * (pow(2, (1.0 / n_tarefas)) - 1);
    char *resultado_rm;
    if (utilizacao <= limite_rm) {
        resultado_rm = "SIM";
    } else if (utilizacao > limite_rm && utilizacao <= 1) {
        resultado_rm = "INCONCLUSIVO";
    } else {
        resultado_rm = "NÃO";
    }

    printf("RM: %s\n", resultado_rm); 
}

// Função para verificar escalonabilidade pelo Earliest Deadline First
void verificar_earliest_deadline_first(Tarefa tarefas[], int n_tarefas) {
    char *resultado_edf;
float utilizacao = 0;  // Nome da variável alterado de load para utilizacao

    bool is_D_equal_P = true;

    // Primeiro, verificamos se D = P para todas as tarefas
    for(int j = 0; j < n_tarefas; j++) {
        if (tarefas[j].deadline != tarefas[j].periodo) {
            is_D_equal_P = false;
            break;
        }
    }

    if (is_D_equal_P) {
        for(int j = 0; j < n_tarefas; j++) {
            utilizacao += tarefas[j].tempo_execucao / tarefas[j].periodo;
        }

        if(utilizacao <= 1) {
            resultado_edf = "SIM";
        } else {
            resultado_edf = "NÃO";
        }
    } else {
        for(int j = 0; j < n_tarefas; j++) {
            utilizacao += tarefas[j].tempo_execucao / tarefas[j].deadline;
        }

        if(utilizacao <= 1) {
            resultado_edf = "INCONCLUSIVO";
        } else {
            resultado_edf = "NÃO";
        }
    }

    // Agora, você pode imprimir ou usar o resultado_edf conforme necessário
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
        int nova_carga = 0;
        if (sscanf(linha, "%d", &nova_carga) == 1) {
            if (n_tarefas > 0) {
                printf("Carga %d\n", carga);
                verificar_executivo_ciclico(tarefas, n_tarefas);
                verificar_rate_monotonic(tarefas, n_tarefas);
                verificar_earliest_deadline_first(tarefas, n_tarefas);
                n_tarefas = 0;  // Reseta o contador de tarefas para a próxima carga
            }
            carga = nova_carga;  // Atualiza a carga para a nova carga
        } else {
            sscanf(linha, "%s\t%d\t%d\t%d", tarefas[n_tarefas].nome, &tarefas[n_tarefas].tempo_execucao,
                   &tarefas[n_tarefas].periodo, &tarefas[n_tarefas].deadline);
            n_tarefas++;
        }
    }

    // Processa a última carga se houver tarefas pendentes
    if (n_tarefas > 0) {
        printf("Carga %d\n", carga);
        verificar_executivo_ciclico(tarefas, n_tarefas);
        verificar_rate_monotonic(tarefas, n_tarefas);
        verificar_earliest_deadline_first(tarefas, n_tarefas);
    }

    // Fecha o arquivo
    fclose(fp);

    return 0;
}
