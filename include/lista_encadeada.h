#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Paciente{
    char nome[51];
    int idade;
    char CPF[12];
    char prioridade[4];

    bool naFila;

    struct Paciente *prox;

} Paciente;

int listaVazia(Paciente *inicio);
Paciente* criarNo(char nome[], int idade, char cpf[]);
Paciente* inserirElementoFim(Paciente* lista, char nome[], int idade, char cpf[]);
Paciente* removerElementoPorCPF(Paciente* lista, const char cpf[]);

#endif