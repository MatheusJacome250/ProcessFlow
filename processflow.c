#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CHAR 1000
#define MAX_ARGS 50
#define MAX_TAREFAS 100
#define MAX_TEXTO 256

typedef struct {
    char nome[MAX_TEXTO];
    char programa[MAX_TEXTO];
    char argumentos[MAX_ARGS][MAX_TEXTO];
    int qtd_argumentos;
} Tarefa;

Tarefa tarefas[MAX_TAREFAS];
int qtd_tarefas = 0;

int main() {

    char linha[MAX_CHAR];

    while (true) {

        printf("processflow> ");

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            break;
        }

        linha[strcspn(linha, "\n")] = '\0';

        char *lista_comando[MAX_ARGS];
        int contador_comando = 0;

        char *comando = strtok(linha, " \t");

        while (comando != NULL && contador_comando < MAX_ARGS) {
            lista_comando[contador_comando] = comando;
            contador_comando++;
            comando = strtok(NULL, " \t");
        }

        if (contador_comando == 0) {
            continue;
        }

        if (strcmp(lista_comando[0], "exit") == 0) {
            break;
        }
    }

    return 0;
}