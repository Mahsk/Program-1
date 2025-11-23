#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"



// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () {

    struct fprio_t *f = malloc(sizeof(struct fprio_t)) ; //aloca um novo nó na fila

    if(!f) //se a alocação falhar returna NULL
      return NULL;
    
    memset(f,0,sizeof(struct fprio_t)) ; //inicia todos os campos com zero
    return f ; //retorna a fila 
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) { 

    if(!f) //Verifica a validade da fila 
     return NULL ;

    struct fpnodo_t *atual = f->prim ; //estrutura auxiliar que vai apontar para cada elemento da fila, começando pelo primeiro
    
    while (atual != NULL) { //enquanto nao chega no final 
      
      struct fpnodo_t *temp = atual->prox ;
      free(atual->item) ; //libera o conteudo
      free(atual) ; //libera o nó
      atual = temp ; //atual é atualizado com o proximo 
    }

    free(f) ; //libera a estrutura da fila
    return NULL ;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {

  if (!f || !item) 
    return -1 ;
  
  struct fpnodo_t *atual = f->prim ;

  while (atual != NULL) { //percorre até não chegar no fim da fila 
    if(atual->item == item) //se o item inserido por igual, dá erro
      return -1 ;
    atual = atual->prox ; 
  }

  struct fpnodo_t *novo = malloc(sizeof(struct fpnodo_t)) ; //aloca um novo nó na fila 

  if (!novo) //se a alocação não der certo, dá erro
    return -1 ;
  
  novo->item = item ;
  novo->tipo = tipo ;
  novo->prio = prio ;
  novo->prox = NULL ;

  struct fpnodo_t *ant = NULL ;
    atual = f->prim ;

  while ((atual && atual->prio < prio) || (atual && atual->prio == prio)) { //Anda até achar uma prioridade maior ou igual
    ant = atual ;
    atual = atual->prox;
  }

    if (!ant) { //Insere no inicio
        novo->prox= f->prim;
        f->prim = novo ;
    } 
    else { //insere no meio ou fim
        novo->prox = ant->prox ;
        ant->prox = novo ;
    }

    
    f->num++ ; //Aumenta o tamanho 
    return f->num ; //Retorna o tamanho
}


// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {

    if(!f || !f->num || !tipo || !prio) //verifica se a fila, tipo e prioridade é válida 
      return NULL;
  
    struct fpnodo_t *remove_item = f->prim ; //armazena o primeiro nó em remove item 

    /*Se os ponteiros forem válidos, guarda os dados*/
    *tipo = remove_item->tipo ;
    *prio = remove_item->prio ;

    void *item = remove_item->item ; 
    f->prim = remove_item->prox ; //atualiza para o proximo
    f->num-- ; //decrementa o tamanho 

    free(remove_item) ;//libera o nó 
    return item ;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {

  if (!f)
    return -1 ;
  return f->num ;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f) {

  if(!f) //
    return ;

  if(!f->num) //verifica o tamanho da fila 
    return ;

  struct fpnodo_t *imprime = f->prim ; //cria estrutura para percorrer a fila
  
  int pos = 0; //inicializa a posição com zero 
  while(imprime != NULL) { //percorre a fila até chegar no final 

    if(pos>0) //se a posição não estiver no começo, dá o espaço
      printf(" ") ;
    printf("(%d %d)", imprime->tipo,  imprime->prio) ;
    imprime = imprime->prox ;
    pos++ ;
  }
}
