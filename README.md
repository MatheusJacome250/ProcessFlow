# ProcessFlow

ProcessFlow é um orquestrador de processos desenvolvido em linguagem C para a disciplina de Infraestrutura de Software.

O programa permite cadastrar e executar tarefas utilizando processos filhos, com suporte a execução sequencial, paralela, pipes, redirecionamento de entrada e saída, execução em background e workflows.

## Arquivos

- processflow.c arquivo geral do programa, possui todas as funções e a main
- Makefile arquivo necessário para rodar o programa
- README.md detalhes sobre o projeto
- evidencias.log testes feitos ao final do projeto

## Compilação

Para compilar o programa:

make clean
make

## Execução

Modo interativo:

./processflow

Modo workflow:

./processflow arquivo.pf

## Testes

Os testes de execução estão registrados no arquivo evidencias.log.

## Sistema Operacional

O programa foi desenvolvido e testado em Linux Ubuntu utilizando WSL no Windows.