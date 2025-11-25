#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () { 

    struct fila_t *nova_fila = malloc(sizeof(struct fila_t)) ;

    if(!nova_fila)
        return NULL ;

    memset(nova_fila, 0, sizeof(struct fila_t)) ;

    return nova_fila ;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f) {

    if(!f)
        return NULL ;

    struct fila_nodo_t *atual = f->prim ;
    struct fila_nodo_t *prox = NULL ;
    
    while(atual != NULL) {
        prox = atual->prox ;
        free(atual) ;
        atual = prox ;
    }
    free(f) ;

    return NULL;
}

// Insere um item no final da fila (politica FIFO).
// Retorno: 1 se tiver sucesso ou 0 se falhar.
int fila_insere (struct fila_t *f, int item){
    
    if(!f) //verifica se f é um ponteiro válido
        return 0;   
    
    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t)) ;

    if(!novo) //se a alocação do ponteiro falhar
        return 0 ;

    memset(novo, 0, sizeof(struct fila_nodo_t)) ;
    novo->item = item ;

    if(!f->num) {
        f->prim = novo ;
        f->ult = novo ;
    }
    else {
        f->ult->prox = novo ;
        f->ult = novo ;
    }

    f->num++ ; //Aumenta o tamanho 
    return 1 ;
}


// Retira o primeiro item da fila e o devolve
// Retorno 1 se a operação foi bem sucedida e 0 caso contrário
int fila_retira (struct fila_t *f, int *item) {

    if(!f || !item) //verifica se f e item sao ponteiros válidos
        return 0 ;

    struct fila_nodo_t *atual = f->prim ; //cria a estrutura atual para apontar para o inicio da fila
   
    *item = atual->item ; //armazena o valor do inicio da fila em *item 
    f->prim = atual->prox ; //atualiza a fila para o proximo elemento 

    f->num-- ; //atualiza o tamanho da fila 
    
    if(!f->num) //atualiza o final da fila
        f->ult = NULL;

    free(atual) ; //libera o ponteiro 
    
    return 1 ;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {

    if(!f)
        return -1 ;
    
    return f->num ;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {

    if(!f) 
        return ;

    struct fila_nodo_t *atual = f->prim ;

    while(atual != NULL) {

        printf("%d ", atual->item) ;
        atual = atual->prox ;
    }
}