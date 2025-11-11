#ifndef CHAMADAS_H
#define CHAMADAS_H

#include "../include/fila_fifo.h"
#include "../include/pilha.h"

void menu();
Paciente*cadastrar(Paciente *lista);
void visualizarPacientes(Paciente *inicio);
Paciente* buscarCpf(Paciente* inicio);
Paciente* buscarPaciente(Paciente* lista, const char cpf[]);
void visualizarFilaSimples(Fila* f, const char* titulo);
void visualizarFilaDupla(FilaDuplaPrioridade* fd);
Paciente* chamarParaFila(FilaDuplaPrioridade* fd, Paciente* lista);
void finalizarAtendimento(Pilha* p, FilaDuplaPrioridade* fd, Paciente** listaDePacientes);
void visualizarHistorico(Pilha *p);

#endif