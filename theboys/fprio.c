#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"


struct fprio_t *fprio_cria () {

    //Aloca memoria para f
    struct fprio_t *f = malloc(sizeof(struct fprio_t)) ; 

     //Se a alocação falhar returna NULL
    if(!f)
      return NULL;
    
    //inicializa todos os campos com zero
    memset(f,0,sizeof(struct fprio_t)) ; 

    //Retorna a fila 
    return f ; 
}

struct fprio_t *fprio_destroi (struct fprio_t *f) { 

    //Verifica o ponteiro
    if(!f) 
     return NULL ;

    //Estrutura auxiliar que vai apontar para cada elemento da fila, começando pelo primeiro
    struct fpnodo_t *atual = f->prim ; 

    //Enquanto nao chega no final 
    while (atual != NULL) { 
      //Cria um ponteiro apontando para o proximo
      struct fpnodo_t *temp = atual->prox ;
      //Libera o conteudo
      free(atual->item) ;
      //Libera o nó
      free(atual) ; 
      //Atual é atualizado com o proximo 
      atual = temp ; 
    }

    //Libera a estrutura da fila
    free(f) ; 
    return NULL ;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {

  //Verifica o ponteiro
  if (!f || !item) 
    return -1 ;
  
  //Cria um ponteiro para o inicio da fila
  struct fpnodo_t *atual = f->prim ;

  //Percorre até não chegar no fim da fila 
  while (atual != NULL) { 
    //Se o item inserido por igual, dá erro
    if(atual->item == item) 
      return -1 ;
    //Atualiza o atual pelo proximo
    atual = atual->prox ; 
  }

  //Aloca memoria para o novo
  struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t)) ; 

  //Se a alocação não der certo, dá erro
  if (!novo) 
    return -1 ;
  
  novo->item = item ;
  novo->tipo = tipo ;
  novo->prio = prio ;
  novo->prox = NULL ;

  struct fpnodo_t *ant = NULL ;
    atual = f->prim ;

  //Anda até achar uma prioridade maior ou igual
  while ((atual && atual->prio < prio) || (atual && atual->prio == prio)) { 
    ant = atual ;
    atual = atual->prox;
  }
    //Insere no inicio
    if (!ant) { 
        novo->prox= f->prim;
        f->prim = novo ;
    } 
    else { //Insere no meio ou fim
        novo->prox = ant->prox ;
        ant->prox = novo ;
    }

    //Aumenta o tamanho 
    f->num++ ; 
    //Retorna o tamanho
    return f->num ; 
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {

    //Verifica se a fila, tipo e prioridade é válida e se é vazia
    if(!f || !f->num || !tipo || !prio)  
      return NULL;

    //Cria um ponteiro para o primeiro da fila
    struct fpnodo_t *remove_item = f->prim ; 

    //Se os ponteiros forem válidos, guarda os dados
    *tipo = remove_item->tipo ;
    *prio = remove_item->prio ;

    //Ponteiro, pois pode retornar qualquer dado
    void *item = remove_item->item ; 

    //Atualiza para o proximo
    f->prim = remove_item->prox ; 
    //Decrementa o tamanho 
    f->num-- ; 

    //Libera o ponteiro
    free(remove_item) ;
    return item ;
}

int fprio_tamanho (struct fprio_t *f) {

  //Verifica o ponteiro
  if (!f)
    return -1 ;
  //Informa o numero de itens na fila
  return f->num ;
}

void fprio_imprime (struct fprio_t *f) {

  //Verifica o ponteiro e se a fila esta vazia
  if(!f || !f->num) //
    return ;

  //Cria o ponteiro apontando para o primeiro da filas
  struct fpnodo_t *imprime = f->prim ;
  
  //Inicia a posição com zero
  int pos = 0; 

  //Percorre enquanto nao chega no final
  while(imprime != NULL) {  
    //Se a posição não estiver no começo, dá o espaço
    if(pos>0) 
      printf(" ") ;
    printf("(%d %d)", imprime->tipo,  imprime->prio) ;
    imprime = imprime->prox ;
    pos++ ;
  }
}
