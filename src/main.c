#include "../include/chamadas.h"

void iniciarSistema(){
    int option;
    Paciente *listaDePacientes = NULL;
    FilaDuplaPrioridade *gerenciadorDeFilaDupla = criarFilaDupla();
    Pilha *gerenciadorPilha= criarPilha(15);
    
    do{
        menu();
        scanf("%d", &option);
        while (getchar() != '\n');

        switch (option){
        case 1:
            listaDePacientes = cadastrar(listaDePacientes);
            break;
        case 2:
            visualizarPacientes(listaDePacientes); 
            break;
        case 3:
            buscarCpf(listaDePacientes);
            break;
        case 4:
            chamarParaFila(gerenciadorDeFilaDupla, listaDePacientes);
            break;
        case 5:
            finalizarAtendimento(gerenciadorPilha, gerenciadorDeFilaDupla, &listaDePacientes);
            break; 
        case 6:
            visualizarHistorico(gerenciadorPilha);   
            break;
        case 7:
            printf("\nSaindo do programa...\n");
            break;
        default:
            printf("\n> Opcao invalida. Por favor, selecione uma opcao de 1 a 7.\n");
            break;
        }
    } while (option != 7);
}

int main(){   
    printf("\n\n----- Bem Vindo a SyClin -----");
    iniciarSistema();

    return 0;
}