#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com lista encadeada simples

// A COMPLETAR

// descreve um nodo da fila de prioridades
struct fpnodo_t { 

    void *item ;
    int tipo ;
    int prioridade ;
    struct fpnodo_t *proximo ;

} ;

// descreve uma fila de prioridades
struct fprio_t { 

    struct fpnodo_t *primeiro ;
    struct fpnodo_t *ultimo ; 
    int tam ;

} ;

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () { 
    struct fprio_t *f = malloc(sizeof(struct fprio_t)) ; //Aloca um novo nó na fila 

    if (!f) //Se a alocação falhar retorna nulo 
        return NULL ;
    
    memset(f, 0, sizeof(struct fprio_t)) ; //Inicia os campos da fila com 0
    return f ; //Retorna a fila 

}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) {

    if (!f) //Verifica a validade da fila 
        return NULL ;

    struct fpnodo_t *atual = f->primeiro ; //estrutura auxiliar que vai apontar para cada elemento da fila, começa no primeiro 

    while (atual != NULL) { //Enquanto nao chega no final 
        struct fpnodo_t *temp = atual->proximo ; //Guarda o endereço do proximo nó antes de liberal o atual 
        
        free(atual->item) ; //Libera o conteudo do nó
        free(atual) ; //Libera o nó 
        atual = temp ; //Atualiza o atual
    }

    free(f) ; //Libera a estrutura da fila
    return NULL ;
    
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    if (!f || !item) //Verifica se a fila e o item é valido
        return -1 ;

    struct fpnodo_t *atual = f->primeiro ; //Estrutura que vai apontar para cada elemento da fila

    while (atual != NULL) { //Enquanto nao chegamos no final da fila, ele percorre
        if (atual->item == item) //Se forem iguais, dá erro
            return -1 ;
        atual = atual->proximo ; //Caso contrário, vai para o proximo
    }

    struct fpnodo_t *novo = malloc (sizeof(struct fpnodo_t)) ; //aloca um novo nó na fila
    if (!novo) //Se a alocação nao der certo, retorna erro 
        return -1 ; 
    
    /*Preenche os campos do novo nó com os dados da função*/
    novo->item = item ;
    novo->tipo = tipo ;
    novo->prioridade = prio ;
    novo->proximo = NULL ;
        
    /*Se a fila estiver vazia*/
    if (f->tam == 0) {
        f->primeiro = novo ;
        f->ultimo = novo ;
        f->tam++ ;
        return f->tam ;
    } 
    
    struct fpnodo_t *ant = NULL ;
    atual = f->primeiro ;

    while (atual && atual->prioridade < prio) { //Anda até achar uma prioridade maior 
        ant = atual ;
        atual = atual->proximo ;
    }

    while (atual && atual->prioridade == prio) { //Anda até achar um igual
        ant = atual ;
        atual = atual ->proximo ;

    }

    if (!ant) { //Insere no inicio
        novo->proximo = f->primeiro ;
        f->primeiro = novo ;
    } 
    else { 
        novo->proximo = ant->proximo ;
        ant->proximo = novo ;
    }

    if (!novo->proximo) //Atualiza o ultimo se o novo nó for o ultimo
        f->ultimo = novo ;
    
    f->tam++ ; //Aumenta o tamanho 
    return f->tam ; //Retorna o tamanho

}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    if (!f || !f->tam || !tipo || !prio) //Verifica se a fila, tipo e prioridade é válida ou se a fila é vazia
        return NULL ;
        
    struct fpnodo_t *remove_item = f->primeiro ; // Armaze o primeiro nó em remove item

    /*Se os ponteiros forem validos, guarda os dados*/
    *tipo = remove_item->tipo ; 
    *prio = remove_item->prioridade ;

    void *item = remove_item->item ;

    f->primeiro = remove_item->proximo ; //Atualiza o ponteiro 
    f->tam-- ; //Decrementa o tamanho

    if (!f->primeiro) //Se a fila ficou vazia, atualiza o ultimo
        f->ultimo = NULL ; 
    
    free(remove_item) ; //Libera o nó 
    return item ;  //retorna o item

}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {

    if (!f) //verifica a validade do ponteiro da fila
        return -1 ;
    return f->tam ; //retorna o tamanho da fila
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f) { 
    if (!f) //Verifica a validade do ponteiro
        return ;
    if (!f->tam) //Verifica o tamanho da fila
        return ;
    
    struct fpnodo_t *imprime = f->primeiro ; //cria estrutura para percorrer a fila 
        int pos = 0 ; //inicializa a posição com 0
    
    while (imprime != NULL) { //percorre a fila até chegar no final 
        if (pos>0) //Se a posição nao estiver no começo, dá o espaço 
            printf(" ") ;
        printf("(%d %d)", imprime->tipo, imprime->prioridade) ;
        imprime = imprime->proximo ;
        pos++ ;
    }
}
