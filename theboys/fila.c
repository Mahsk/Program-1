#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () { 

    //Aloca memoria para a fila
    struct fila_t *nova_fila = malloc(sizeof(struct fila_t)) ;

    //Se a alocaçao falhar, retorna NULL
    if(!nova_fila)
        return NULL ;

    //Inicializa todos os campos da fila com 0
    memset(nova_fila, 0, sizeof(struct fila_t)) ;

    return nova_fila ;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
struct fila_t *fila_destroi (struct fila_t *f) {

    //Verifica se é um ponteiro válido
    if(!f)
        return NULL ;

    //Cria um ponteiro para o primeiro da fila
    struct fila_nodo_t *atual = f->prim ;
    struct fila_nodo_t *prox = NULL ;
    
    //Enquanto nao chega no final, atualiza
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
    
    //Verifica se f é um ponteiro válido
    if(!f) 
        return 0;   
    
    //Aloca memória para um novo elemento
    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t)) ;

     //Se a alocação do ponteiro falhar
    if(!novo)
        return 0 ;

    //Inicializa os elementos com 0
    memset(novo, 0, sizeof(struct fila_nodo_t)) ;
    novo->item = item ;

    //Se a fila está vazia, o novo elemento será tanto o ultimo e o primeiro da fila
    if(!f->num) {
        f->prim = novo ;
        f->ult = novo ;
    }
    else { //Se já contém elementos,o ultimo da fila aponta para o novo elemento criado e o ultimo se torna o novo
        f->ult->prox = novo ;
        f->ult = novo ;
    }

    //Aumenta o tamanho 
    f->num++ ; 
    //A operaçao foi bem sucedida
    return 1 ;
}


// Retira o primeiro item da fila e o devolve
int fila_retira (struct fila_t *f, int *item) {

    //Verifica se f e item sao ponteiros válidos
    if(!f || !item) 
        return 0 ;

    //Cria a estrutura atual para apontar para o inicio da fila
    struct fila_nodo_t *atual = f->prim ; 
   
    //Armazena o valor do inicio da fila em *item 
    *item = atual->item ; 
    //Atualiza a fila para o proximo elemento 
    f->prim = atual->prox ; 

    //Atualiza o tamanho da fila 
    f->num-- ; 
    
    //Atualiza o final da fila
    if(!f->num) 
        f->ult = NULL;

    //Libera o ponteiro
    free(atual) ;  
    //Retorna 1 se sucesso
    return 1 ;
}

// Informa o número de itens na fila.
int fila_tamanho (struct fila_t *f) {

    //Verifica se é um ponteiro valido
    if(!f)
        return -1 ;
    //Retorna o tamanho da fila
    return f->num ;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {

    //Verifica se é um ponteiro válido
    if(!f) 
        return ;

    //Cria o ponteiro para o inicio da fila
    struct fila_nodo_t *atual = f->prim ;

    //Enquanto nao chegar ao final
    while(atual != NULL) {
        //Imprime
        printf("%d ", atual->item) ;
        //Atualiza para o proximo
        atual = atual->prox ;
    }
}