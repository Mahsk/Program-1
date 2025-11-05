#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"



// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () { 
    struct fila_t *f = malloc(sizeof(struct fila_t)) ; //Aloca um novo nó na fila 

    if (!f) //Se a alocação falhar retorna nulo 
        return NULL ;
    
    memset(f, 0, sizeof(struct fila_t)) ; //Inicia os campos da fila com 0
    return f ; //Retorna a fila 

}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f) {

    if (!f) //Verifica a validade da fila 
        return NULL ;

    struct fila_nodo_t *atual = f->prim; //estrutura auxiliar que vai apontar para cada elemento da fila, começa no primeiro 

    while (atual != NULL) { //Enquanto nao chega no final 
        struct fila_nodo_t *temp = atual->prox ; //Guarda o endereço do proximo nó antes de liberal o atual 
        
        free(atual->item) ; //Libera o conteudo do nó
        free(atual) ; //Libera o nó 
        atual = temp ; //Atualiza o atual
    }

    free(f) ; //Libera a estrutura da fila
    return NULL ;
    
}


// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
//Sefue a filosofia de FIFO
int fila_insere  (struct fila_t *f, void *item) {
    if (!f || !item) //Verifica se a fila e o item é valido
        return -1 ;

    struct fila_nodo_t *atual = f->prim ; //Estrutura que vai apontar para cada elemento da fila

    while (atual != NULL) { //Enquanto nao chegamos no final da fila, ele percorre
        if (atual->item == item) //Se forem iguais, dá erro
            return -1 ;
        atual = atual->prox ; //Caso contrário, vai para o proximo
    }

    struct fila_nodo_t *novo = malloc (sizeof(struct fila_nodo_t)) ; //aloca um novo nó na fila
    if (!novo) //Se a alocação nao der certo, retorna erro 
        return -1 ; 
    
    /*Preenche os campos do novo nó com os dados da função*/
    novo->item = item ;
    novo->prox = NULL ;
        
    /*Se a fila estiver vazia*/
    if (f->num == 0) {
        f->prim = novo ;
        f->fim = novo ;
    } 
    else { 
        f->fim->prox = novo ; //aponta o ultimo nó para novo 
        f->fim = novo ; //atualiza o ponteiro fim para o novo
    }

    f->num++ ; //Aumenta o tamanho 
    return f->num ; //Retorna o tamanho

}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f) {
    if (!f || !f->num) //Verifica se a fila é válida ou se a fila é vazia
        return NULL ;
        
    struct fila_nodo_t *remove_item = f->prim ; // Armaze o primeiro nó em remove item

    void *item = remove_item->item ;

    f->prim = remove_item->prox ; //Atualiza o ponteiro 
    f->num-- ; //Decrementa o tamanho

    if (!f->prim) //Se a fila ficou vazia, atualiza o ultimo
        f->fim = NULL ; 
    
    free(remove_item) ; //Libera o nó 
    return item ;  //retorna o item

}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {

    if (!f) //verifica a validade do ponteiro da fila
        return -1 ;
    return f->num; //retorna o tamanho da fila
}

//Imprime o conteúdo da fila
void fila_imprime (struct fila_t *f) { 
    if (!f) //Verifica a validade do ponteiro
        return ;
    if (!f->num) //Verifica o tamanho da fila
        return ;
    
    struct fila_nodo_t *imprime = f->prim; //cria estrutura para percorrer a fila 
        int pos = 0 ; //inicializa a posição com 0
    
    while (imprime != NULL) { //percorre a fila até chegar no final 
        int *valor = (int *)imprime->item ;
        printf("%d", *valor) ;
        imprime = imprime->prox;
        pos++;
    }
}
