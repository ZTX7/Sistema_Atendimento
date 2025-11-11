#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//---------------------------------------------------------------
//                       LISTA ENCADEADA
//---------------------------------------------------------------

typedef struct Paciente{
    char nome[51];
    int idade;
    char CPF[12];
    char prioridade[4];

    bool naFila;

    struct Paciente *prox;

} Paciente;

int listaVazia(Paciente *lista) {
    return lista == NULL;
}

Paciente* criarNo(char nome[], int idade, char cpf[]){
    Paciente* novoNo = (Paciente*)malloc(sizeof(Paciente));

    strcpy(novoNo->nome, nome);
    novoNo->idade = idade;
    strcpy(novoNo->CPF, cpf);

    if (idade >= 50) {
        strcpy(novoNo->prioridade, "Sim");
    } else {
        strcpy(novoNo->prioridade, "Nao");
    }

    novoNo->naFila = false;
    
    novoNo->prox = NULL;
    return novoNo;
}

// Paciente* inserirElementoComeco(Paciente* lista, char nome[], int idade, char cpf[]){
//     Paciente* novoNo = criarNo(nome, idade, cpf);
//     novoNo->prox = lista;
//     return novoNo;
// }

Paciente* inserirElementoFim(Paciente* lista, char nome[], int idade, char cpf[]){
    Paciente* novoNo = criarNo(nome, idade, cpf);

    if (listaVazia(lista)) {
        return novoNo;
    }
    
    Paciente* temp = lista;
    while (temp->prox != NULL) {
        temp = temp->prox;
    }
    
    temp->prox = novoNo;
    
    return lista;
}

Paciente* removerElementoPorCPF(Paciente* lista, const char cpf[]) {
    if (listaVazia(lista)) {
        return NULL;
    }

    Paciente* atual = lista;
    Paciente* anterior = NULL;

    if (strcmp(atual->CPF, cpf) == 0) {
        lista = atual->prox;
        free(atual);
        return lista;
    }

    anterior = atual;
    atual = atual->prox;

    while (atual != NULL) {
        if (strcmp(atual->CPF, cpf) == 0) {
            anterior->prox = atual->prox;
            free(atual);
            return lista;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return lista;
}

//---------------------------------------------------------------
//                           FILA
//---------------------------------------------------------------

typedef struct {
    Paciente *inicio; 
    Paciente *fim;
    int tamanho;
} Fila;

typedef struct {
    Fila *filaPrioridade;
    Fila *filaNormal;
} FilaDuplaPrioridade;

Fila* criarFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));

    fila->inicio=NULL;
    fila->fim=NULL;
    fila->tamanho=0;   
    
    return fila;
}

FilaDuplaPrioridade* criarFilaDupla() {
    FilaDuplaPrioridade* fd = (FilaDuplaPrioridade*)malloc(sizeof(FilaDuplaPrioridade));
    
    if (fd == NULL) {
        return NULL;
    }
    
    fd->filaPrioridade = criarFila();
    fd->filaNormal = criarFila();
    
    if (fd->filaPrioridade == NULL || fd->filaNormal == NULL) {
        if (fd->filaPrioridade) free(fd->filaPrioridade);
        if (fd->filaNormal) free(fd->filaNormal);
        free(fd);
        return NULL;
    }
    
    return fd;
}

void enfileirarSimples(Fila* f, Paciente* novoNo) {
    if (f->inicio == NULL) {
        f->inicio = novoNo;
        f->fim = novoNo;
    } else {
        f->fim->prox = novoNo;
        f->fim = novoNo;
    }
    f->tamanho++;
}

void enfileirarDuplo(FilaDuplaPrioridade* fd, char nome[], int idade, char cpf[]){
    Paciente* novoNo = criarNo(nome, idade, cpf);

    if (novoNo == NULL) return;

    if (strcmp(novoNo->prioridade, "Sim") == 0) {
        enfileirarSimples(fd->filaPrioridade, novoNo);
    } else {
        enfileirarSimples(fd->filaNormal, novoNo);
    }
}

Paciente* desenfileirarSimples(Fila* f) {
    Paciente* removido = f->inicio;
    f->inicio = f->inicio->prox;

    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    f->tamanho--;
    
    printf("\n<< Paciente %s removido da Fila. >>\n", removido->nome);
    return removido;
}

//---------------------------------------------------------------
//                           PILHA
//---------------------------------------------------------------

typedef struct{
	int topo; 
	int capacidade;
	Paciente **registro;
} Pilha;

Pilha* criarPilha(int capacidade){
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
    if (p == NULL) return NULL;

    p->registro = (Paciente**)malloc(capacidade*sizeof(Paciente*));
    if(p->registro == NULL){
        free(p);
        return NULL;
    }
    p->topo = -1;
    p->capacidade = capacidade;

    return p;
}

int sePilhaVazia (Pilha *p){
    if( p == NULL || p->topo == -1 ) {
        return 1; 
    }
    return 0;   
}

int sePilhaCheia (Pilha *p){
    if( p != NULL && p->topo == p->capacidade - 1 ) {
            return 1; 
        }
    return 0; 
}

Pilha* empilhar (Pilha *p, Paciente *atendido){
    if (sePilhaCheia(p)){
        printf("\n>>ATENCAO: Historico cheio (Capacidade %d). O paciente mais antigo sera removido para inserir o novo.\n", p->capacidade);
        
        Paciente* maisAntigo = p->registro[0];
        printf("\n<< Paciente %s (Mais Antigo) removido permanentemente do Historico. >>\n", maisAntigo->nome);
        free(maisAntigo); 
        

        for (int i = 0; i < p->capacidade - 1; i++) {
            p->registro[i] = p->registro[i + 1];
        }

    } else {
        p->topo++;
    }

	p->registro [p->topo] = atendido;
}

//---------------------------------------------------------------
//                       PARTE LÓGICA
//---------------------------------------------------------------

void menu() {
    printf("\n\n_________________ MENU _________________\n");
    printf("(1). Cadastrar Paciente.\n");
    printf("(2). Pacientes Cadastrados.\n");
    printf("(3). Buscar por CPF.\n");
    printf("(4). Fila de atendimento.\n");
    printf("(5). Finalizar atendimento.\n");
    printf("(6). Historico de atendimento.\n");    
    printf("(7). Sair.\n");
    printf("\nSelecione a opcao para continuar: ");
}

Paciente* buscarPaciente(Paciente* lista, const char cpf[]) {
    Paciente *temp = lista;
    
    while (temp != NULL) {
        if (strcmp(temp->CPF, cpf) == 0) {
            return temp;
        }
        temp = temp->prox;
    }
    
    return NULL;
}

Paciente*cadastrar(Paciente *lista) {
    printf("\n\n~~~~~~~~~~ CADASTRANDO PACIENTE ~~~~~~~~~~\n\n");
    
    char nome[51];
    int idade;
    char cpf[12];

    printf("Digite o nome: ");
    fgets(nome, 51, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a idade: ");
    scanf("%d", &idade);
    while (getchar() != '\n');

    printf("Digite o CPF (11 digitos): ");
    fgets(cpf, 12, stdin);
    cpf[strcspn(cpf, "\n")] = 0;
    
    if (strlen(cpf) != 11){
        printf("\n> ERRO: O CPF deve ter 11 digitos. Cadastro cancelado.\n");
        return lista;
    }

    // ---------------------------------------------
    Paciente* pacienteExistente = buscarPaciente(lista, cpf);
    
    if (pacienteExistente != NULL) {
        printf("\n> ERRO: Paciente com CPF %s ja esta cadastrado (Nome: %s).\n", cpf, pacienteExistente->nome);
        printf("> Cadastro cancelado.\n");
        return lista;
    }
    // ---------------------------------------------

    lista = inserirElementoFim(lista, nome, idade, cpf);

    printf("\n>>> Paciente cadastrado com sucesso!\n");
    return lista;
}

void visualizarPacientes(Paciente *lista)   {
    printf("\n\n~~~~~~~~~ PACIENTES CADASTRADOS ~~~~~~~~~\n\n");
    if (listaVazia(lista)){
        printf("> Nao ha pacientes cadastrados\n");
        return;
    }
    else{
        Paciente *temp = lista;
        int i = 1;
        while (temp != NULL){
            printf("\n------- [Paciente %d] -------", i++);
            printf("\nNome: %s\n", temp->nome);
            printf("Idade: %d\n", temp->idade);
            printf("CPF: %s\n", temp->CPF);
            temp = temp->prox;
        }
    }
}

Paciente* buscarCpf(Paciente*lista) {
    char buscacpf[12];
    int condition;

    printf("\n\n~~~~~~~~~~~~ BUSCAR POR CPF ~~~~~~~~~~~~\n");
    if (listaVazia(lista)){
        printf("\n> Nao ha pacientes cadastrados.\n");
        return NULL;
    }

    do{
            
        printf("Digite o CPF (11 digitos): ");
        fgets(buscacpf, 12, stdin);
        buscacpf[strcspn(buscacpf, "\n")] = 0;

        Paciente *temp = lista;
        int posicao = 0;
        while (temp != NULL) {
            if (strcmp(temp->CPF, buscacpf) == 0) {
                
                printf("\n--- [%d]Paciente ---\n", posicao);
                printf("Nome: %s\n", temp->nome);
                printf("Idade: %d\n", temp->idade);
                printf("Prioridade: %s\n", temp->prioridade);
                
                return temp;
            }
            temp = temp->prox;
            posicao++;
        }

        if (temp == NULL){
            printf("\n> CPF nao encontrado ou inexistente\n", buscacpf);
            return NULL;
        } 

        printf("\nDeseja buscar outro paciente?\n");
        printf("(1). Sim -- (2). Nao: ");
        scanf("%d", &condition);
        while (getchar() != '\n');
        printf("%d", condition);


        if (condition == 1){
            continue;
        } else if (condition != 2){
            printf("\n>>> Retornando ao menu principal...\n");
            break;
        } 
    } while (condition == 1); 

}

void visualizarFilaSimples(Fila* f, const char* titulo) { 
   printf("\n\n--- %s (Tamanho: %d) ---\n", titulo, f->tamanho);
    
    Paciente *atual = f->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
       printf("[%d] Nome: %s, Idade: %d, CPF: %s, prioridade %s\n", 
               posicao++, atual->nome, atual->idade, atual->CPF, atual->prioridade); // Adicionado: atual->prioridade
       atual = atual->prox;
   }
}

void visualizarFilaDupla(FilaDuplaPrioridade* fd) {
    visualizarFilaSimples(fd->filaPrioridade, "FILA DE PRIORIDADE");
    visualizarFilaSimples(fd->filaNormal, "FILA NORMAL");
}

Paciente* chamarParaFila(FilaDuplaPrioridade* fd, Paciente* lista) { 
    char input_condition[10];
    int condition;
    Paciente* pacienteChamado = NULL;
    Paciente* temp = lista;

    printf("\n\n~~~~~~~~~~~~ FILA DE ATENDIMENTO ~~~~~~~~~~~~\n");

    if (listaVazia(lista)) {
        printf("\n> Nao ha pacientes cadastrados para chamar.\n");
        return lista;
    }
    
    visualizarFilaDupla(fd);

    temp = lista;
    while(temp != NULL) {
        if (temp->naFila == false) {
            pacienteChamado = temp;
            break;
        }
        temp = temp->prox;
    }

    if (pacienteChamado == NULL) {
        printf("\n> Todos os pacientes cadastrados ja foram chamados para a fila.\n");
        return lista;
    }
    // --------------------------------------------------------------------------

    do {
        printf("\n\nProximo a ser chamado: %s (CPF: %s).\n", pacienteChamado->nome, pacienteChamado->CPF);
        printf("Deseja chamar este paciente para a fila?\n");
        printf("(1). Sim / (2). Nao: ");
        
        if (fgets(input_condition, 10, stdin) == NULL || input_condition[0] == '\n') {
            printf("\nEntrada invalida. Digite 1 ou 2.\n");
            continue;
        }
        
        input_condition[strcspn(input_condition, "\n")] = 0;
        condition = atoi(input_condition);

        if (condition == 1) {
            enfileirarDuplo(fd, 
                pacienteChamado->nome, 
                pacienteChamado->idade, 
                pacienteChamado->CPF
            ); 
            
            pacienteChamado->naFila = true; 
            
            printf("\n>>> Paciente %s adicionado a fila de atendimento.\n", pacienteChamado->nome);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");  
            visualizarFilaDupla(fd); 
            
            pacienteChamado = NULL;
            temp = lista;
            while(temp != NULL) {
                if (temp->naFila == false) {
                    pacienteChamado = temp;
                    break;
                }
                temp = temp->prox;
            }
            
            if (pacienteChamado == NULL) {
                printf("\n\n>>> Todos os pacientes cadastrados foram chamados para a fila.\n");
                condition = 2;
            }

        } else if (condition == 2) {
            printf("\n>>> Retornando ao menu principal...\n");
        } else {
            printf("\n> Erro na Solicitacao. Digite 1 ou 2.\n");
        }
    } while (condition == 1 && pacienteChamado != NULL);
    
    return lista;
}

void finalizarAtendimento(Pilha* p, FilaDuplaPrioridade* fd, Paciente** listaDePacientes){  
    printf("\n\n~~~~~~~~~~ FINALIZAR ATENDIMENTO ~~~~~~~~~~\n");

    Paciente* atendido = NULL;

    if (fd->filaPrioridade->inicio != NULL) {
        atendido = desenfileirarSimples(fd->filaPrioridade);
    } 
    else if (fd->filaNormal->inicio != NULL) {
        atendido = desenfileirarSimples(fd->filaNormal);
    }
    else {
        printf("\n> Nao ha pacientes na fila de atendimento.\n");
        return;
    }
    if (atendido != NULL) {
        char cpfParaRemover[12];
        strcpy(cpfParaRemover, atendido->CPF);

        empilhar(p, atendido);
        printf("\n>>> Atendimento de %s finalizado e adicionado ao Historico.\n\n", atendido->nome);
        
        *listaDePacientes = removerElementoPorCPF(*listaDePacientes, cpfParaRemover);
        printf(">>> Paciente %s removido da lista de cadastros.\n", cpfParaRemover);

        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        visualizarFilaDupla(fd);
    }
}

void visualizarHistorico(Pilha *p){
    printf("\n\n~~~~~~~~~~~ HISTORICO DE ATENDIMENTO ~~~~~~~~~~~\n");
    if (p == NULL || sePilhaVazia(p)) {
        printf("\n> Nenhum paciente atentido.\n");
        return;
    }
    
    printf("Capacidade: %d | Total de Atendidos: %d\n\n", p->capacidade, p->topo + 1);
    
    for (int i = p->topo; i >= 0; i--) {
        Paciente *atendido = p->registro[i];
        printf("[%d] NOME: %s, CPF: %s, Prioridade: %s\n", 
                i, atendido->nome, atendido->CPF, atendido->prioridade);
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}


//---------------------------------------------------------------
//                          CHAMADAS
//---------------------------------------------------------------

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
    printf("\n----- Bem Vindo a SyClin -----");
    iniciarSistema();
    return 0;
}