#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHAR 1000
#define MAX_ARGS 50
#define MAX_TEXTO 256

typedef struct Tarefa {
    char nome[MAX_TEXTO];
    char programa[MAX_TEXTO];
    char argumentos[MAX_ARGS][MAX_TEXTO];
    int qtd_argumentos;
    struct Tarefa *prox;
} Tarefa;

Tarefa *inicio = NULL;
Tarefa *fim = NULL;

pid_t iniciar_tarefa(Tarefa *tarefa) {

    char *argumentos[MAX_ARGS + 2];

    argumentos[0] = tarefa->programa;

    for (int i = 0; i < tarefa->qtd_argumentos; i++) {
        argumentos[i + 1] = tarefa->argumentos[i];
    }

    argumentos[tarefa->qtd_argumentos + 1] = NULL;

    pid_t pid = fork();

    if (pid < 0) {
        printf("Erro ao criar processo.\n");
        return -1;
    }

    if (pid == 0) {
        execvp(tarefa->programa, argumentos);

        perror("Erro ao executar programa");
        _exit(1);
    }

    return pid;
}

void cadastrar_tarefa(char *lista_comando[], int contador_comando) {

    if (contador_comando < 3) {
        printf("Erro!!! \n formato correto: task <nome> <programa> [argumentos...]\n");
        return;
    }

    Tarefa *nova_tarefa = (Tarefa *) malloc(sizeof(Tarefa));

    if (nova_tarefa == NULL) {
        printf("Erro!!! Falha ao alocar memoria.\n");
        return;
    }

    strcpy(nova_tarefa->nome, lista_comando[1]);
    strcpy(nova_tarefa->programa, lista_comando[2]);

    nova_tarefa->qtd_argumentos = contador_comando - 3;

    for (int i = 3; i < contador_comando; i++) {
        strcpy(nova_tarefa->argumentos[i - 3], lista_comando[i]);
    }

    nova_tarefa->prox = NULL;

    if (inicio == NULL) {
        inicio = nova_tarefa;
        fim = nova_tarefa;
    }
    else {
        fim->prox = nova_tarefa;
        fim = nova_tarefa;
    }
}

Tarefa *buscar_tarefa(char *nome) {
    
    Tarefa *atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}

void executar_tarefa(Tarefa *tarefa) {

    pid_t pid = iniciar_tarefa(tarefa);

    if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
}

void executar_sequencial(char *lista_comando[], int contador_comando) {

    for (int i = 2; i < contador_comando; i++) {
        Tarefa *tarefa = buscar_tarefa(lista_comando[i]);

        if (tarefa == NULL) {
            printf("Erro!!! Tarefa %s nao encontrada.\n", lista_comando[i]);
            continue;
        }

        executar_tarefa(tarefa);
    }
}

void executar_paralelo(char *lista_comando[], int contador_comando) {

    pid_t pids[MAX_ARGS];
    int qtd_pids = 0;

    for (int i = 2; i < contador_comando; i++) {

        Tarefa *tarefa = buscar_tarefa(lista_comando[i]);

        if (tarefa == NULL) {
            printf("Erro!!! Tarefa %s nao encontrada.\n", lista_comando[i]);
            continue;
        }

        pid_t pid = iniciar_tarefa(tarefa);

        if (pid > 0) {
            pids[qtd_pids] = pid;
            qtd_pids++;
        }
    }

    for (int i = 0; i < qtd_pids; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

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

        else if (strcmp(lista_comando[0], "task") == 0) {
            cadastrar_tarefa(lista_comando, contador_comando);
        }

        else if (strcmp(lista_comando[0], "run") == 0) {

            if (contador_comando < 2) {
                printf("Erro!!! Informe uma tarefa.\n");
                continue;
            }

            if (strcmp(lista_comando[1], "sequential") == 0) {

                if (contador_comando < 3) {
                    printf("Erro!!! Informe pelo menos uma tarefa.\n");
                    continue;
                }

                executar_sequencial(lista_comando, contador_comando);
            }

            else {

                Tarefa *tarefa = buscar_tarefa(lista_comando[1]);

                if (tarefa == NULL) {
                    printf("Erro!!! Tarefa nao encontrada.\n");
                    continue;
                }

                executar_tarefa(tarefa);
            }
        }
    }

    return 0;
}