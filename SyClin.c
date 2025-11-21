#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//---------------------------------------------------------------
//                       LISTA ENCADEADA
//---------------------------------------------------------------

// Criando a Struct Paciente com alias (apelido) de Paciente
typedef struct Paciente{
    char nome[51];
    int idade;
    char CPF[12];
    char prioridade[4];

    bool naFila;

    struct Paciente *prox;

} Paciente;

// Verfica se o elemento head, o primeiro NÓ, existe
int listaVazia(Paciente* head) {
    return head == NULL;
}

// Criando o NÓ Paciente
Paciente* criarNo(char nome[], int idade, char cpf[]){
    // novoNo vai ser um ponteiro para Paciente que vai guardar o endereço do novo bloco de memória na heap
    Paciente* novoNo = (Paciente*)malloc(sizeof(Paciente));

    // strcpy copia uma string de um local origem para outro destino
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

// Inserir um NÓ no final da lista
Paciente* inserirElementoFim(Paciente* head, char nome[], int idade, char cpf[]){
    Paciente* novoNo = criarNo(nome, idade, cpf);

    if (listaVazia(head)) {
        return novoNo;
    }
    
    // Declara um ponteiro temporário que aponta para o 'head' da lista
    Paciente* temp = head;

    // LOOP para percorrer a lista do início ao fim até encontrar o último NÓ antes de NULL
    while (temp->prox != NULL) {
        temp = temp->prox;
    }
    
    // lembrando que após o LOOP: temp->prox = NULL
    temp->prox = novoNo;
    
    return head;
}

// Busca e Remove o NÓ da lista pelo CPF
Paciente* removerElementoPorCPF(Paciente* head, const char cpf[]) {
    if (listaVazia(head)) {
        return NULL;
    }

    Paciente* atual = head;
    Paciente* anterior = NULL;

    // "strcmp(atual->CPF, cpf) == 0" vai comparar o CPF do nó atual (atual->CPF) com o CPF de busca (cpf) 
    // e se forem iguais (... == 0) vai executar o a condição
    
    // Verifica o primeiro nó, o 'head'
    if (strcmp(atual->CPF, cpf) == 0) {
        head = atual->prox;
        free(atual);
        return head;
    }

    // Os 2 apontam para o 'head'
    anterior = atual;

    // O atual passa a ser o 2° Nó (anterior aponta para o 'head')
    atual = atual->prox;

    // Percorrendo a lista, a partir do 2° Nó:
    while (atual != NULL) {
        if (strcmp(atual->CPF, cpf) == 0) {
            // O anterior vai apontar para o Nó depois do atual
            anterior->prox = atual->prox;
            free(atual);
            return head;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return head;
}

//---------------------------------------------------------------
//                           FILA
//---------------------------------------------------------------

// Criando uma Struct anônima com alias (apelido) de Fila
// diferente da Struct Paciente eu n preciso me auto-referenciar como "struct Paciente *prox;"
typedef struct {
    Paciente *inicio; 
    Paciente *fim;
    int tamanho;
} Fila;

// Criando uma Struct anônima com alias (apelido) de FilaDuplaPrioridade: 1 Fila que vai conter 2 Filas
typedef struct {
    Fila *filaPrioridade;
    Fila *filaNormal;
} FilaDuplaPrioridade;

// Criar fila individualmente
Fila* criarFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));

    fila->inicio=NULL;
    fila->fim=NULL;
    fila->tamanho=0;   
    
    return fila;
}

// Criar a fila "maior" 
FilaDuplaPrioridade* criarFilaDupla() {
    FilaDuplaPrioridade* fd = (FilaDuplaPrioridade*)malloc(sizeof(FilaDuplaPrioridade));
    
    fd->filaPrioridade = criarFila();
    fd->filaNormal = criarFila();
    
    return fd;
}

// Adicionando Paciente na fila
void enfileirar(Fila* node, Paciente* novoNo) {
    if (node->inicio == NULL) {
        node->inicio = novoNo;
        node->fim = novoNo;
    } else {
        node->fim->prox = novoNo;
        node->fim = novoNo;
    }
    node->tamanho++;
}

// Criando Nós nas duas filas
void enfileirarDuplo(FilaDuplaPrioridade* node, char nome[], int idade, char cpf[]){
    Paciente* novoNo = criarNo(nome, idade, cpf);

    if (strcmp(novoNo->prioridade, "Sim") == 0) {
        enfileirar(node->filaPrioridade, novoNo);
    } else {
        enfileirar(node->filaNormal, novoNo);
    }
}

// Removendo Paciente da fila
Paciente* removerDaFila(Fila* node) {
    // Primeiro paciente
    Paciente* removido = node->inicio;

    // O inicio passa a ser o 2° Paciente
    node->inicio = node->inicio->prox;

    node->tamanho--;
    
    printf("\n<< Paciente %s removido da Fila. >>\n", removido->nome);
    return removido;
}

//---------------------------------------------------------------
//                           PILHA
//---------------------------------------------------------------

// Criando uma Struct anônima com alias (apelido) de Pilha
typedef struct{
	int topo; 
	int capacidade;

    // 'registro' armazena uma coleção de ponteiros para 'Paciente'
	Paciente **registro;
} Pilha;

// Criando a Pilha com limite de capacidade
Pilha* criarPilha(int capacidade){
    // Aloca memória para a Pilha em si
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));

    // Aloca memória para o array que vai armazenar os ponteiros para 'Paciente'
    p->registro = (Paciente**)malloc(capacidade*sizeof(Paciente*));

    // Se topo começar em 0 ao adicionar um 'Paciente' ele seria adicionado no indice 1 e o indice 0 estaria vazio
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

// Acrescentando Paciente na pilha
Pilha* empilhar (Pilha *p, Paciente *atendido){
    if (sePilhaCheia(p)){
        printf("\n>>ATENCAO: Historico cheio (Capacidade %d). O paciente mais antigo sera removido para inserir o novo.\n", p->capacidade);
        
        // removendo o primeiro Paciente que foi adicionado na Pilha
        Paciente* maisAntigo = p->registro[0];
        printf("\n<< Paciente %s (Mais Antigo) removido permanentemente do Historico. >>\n", maisAntigo->nome);
        free(maisAntigo); 
        
       //  movendo todos os ponteiros de Paciente uma posição para a esquerda
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

    // ---------------------------------------------
    Paciente* pacienteExistente = buscarPaciente(lista, cpf);
    
    if (pacienteExistente != NULL) {
        printf("\n> ERRO: CPF %s ja esta cadastrado.\n", cpf);
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
            printf("------- [Paciente %d] -------", i++);
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
        atendido = removerDaFila(fd->filaPrioridade);
    } 
    else if (fd->filaNormal->inicio != NULL) {
        atendido = removerDaFila(fd->filaNormal);
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
        // visualizarFilaDupla(fd);
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