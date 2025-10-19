#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementação com lista encadeada dupla circular-não

// define a estrutura interna do item
struct item_t {

  struct item_t *ant ;
  struct item_t *prox ;
  int valor ;

} ;

// estrutura de uma lista
struct lista_t {

  // define a estrutura interna da lista
  struct item_t *primeiro ;
  struct item_t *ultimo ;
  int tam ;

} ;

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria () {
  struct lista_t *lst = malloc(sizeof(struct lista_t)) ; //Aloca memória para lista

  if (!lst) // Se a alocação falhar retorna nulo
    return NULL ;

  memset(lst, 0, sizeof(struct lista_t)) ; //Inicia os campos da lista com 0
  return lst ;

}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst) {

  if (!lst)
    return NULL ;

  struct item_t *aux ; //estrutura auxiliar que vai apontar para cada elemento da lista
  while (lst->primeiro != NULL) {

    aux = lst-> primeiro ; //salva o primeiro item
    lst-> primeiro = aux-> prox ; //avança para o proximo
    free(aux) ; //desaloca o item atual 

  }
  free(lst) ; //libera a estrutura de dados
  return NULL ;
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos) {

    if (!lst) //Verifica se o ponteiro da lista é valido 
      return -1 ;
    
    if (pos < -1) //Se a posição for inválida, retorna errro
      return -1 ;

    struct item_t *novo_item = malloc (sizeof(struct item_t)) ; // aloca um novo nó na lista
    if (!novo_item) // se falhar retorna -1
      return -1;
    
    memset (novo_item, 0, sizeof(struct item_t)) ; // zera o conteudo da nova estrutura
    novo_item->valor = item ; //Atribui o valor

    /*Se a lista estiver vazia, o primeiro e o ultimo termo serao o novo item*/
    if (lst->tam == 0) {
      lst->primeiro = novo_item ; //O primeiro da lista recebe o novo item
      lst->ultimo = novo_item ;  //O ultimo também recebe o novo item
      lst->tam++; //Acrescenta um novo espaço na lista
      return lst->tam ;
    }
    /*Se a posição for além do fim da lista ou -1, insere no fim */
    if (pos == lst->tam || pos == -1) {
      novo_item->ant = lst->ultimo ; //O anterior do novo item será o ultimo 
      lst->ultimo->prox = novo_item ;  //O proximo do ultimo será o novo item 
      lst->ultimo = novo_item ; //O ultimo é atulizado como novo item
      lst->tam++  ; //Aumenta o tamanho
      return lst->tam ;
    }
    /*Insere no início*/
    if (pos == 0) {
      novo_item->prox = lst->primeiro ; //O proximo do novo item será o primeiro da lista antiga
      lst->primeiro->ant = novo_item ; //O anterior do primeiro da lista antiga será o novo item 
      lst->primeiro = novo_item ; //O novo primeiro será o novo item
      lst->tam++ ;
      return lst->tam ;
    }
    /*Insere no meio*/
    struct item_t *atual =  lst-> primeiro ; //Começa a buscar do inicio da lista

      for (int i=0; i<pos; i++)  //Percorre a lista
        atual = atual->prox ; 
      
      novo_item->prox = atual ; //O proximo do novo será o atual 
      novo_item->ant = atual->ant ; //O anterior do novo será  o anteior do atual 

      if (atual->ant) { //Se tem item antes do atual 
        atual->ant->prox = novo_item ; //O proximo do anterior do atual será o novo item 
        atual->ant = novo_item ; //O anterior do atual receve o novo item
      }

    lst->tam++ ;

    return lst->tam ;
} 

// Retira o item da lista da posição indicada.
// se a posição for além do fim da lista ou for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos) {
  
    if (!lst || !item) //Verifica se o ponteiro da lista é valido 
      return -1 ;
    
    if (!lst->tam) // Verifica se a lista está vazia
      return -1 ;

    
    struct item_t *remove_item = NULL ;

    /*Se a posição for além do fim da lista ou -1, insere no fim */
    if (pos == lst->tam || pos == -1) {
      remove_item = lst->ultimo ;
      *item = remove_item->valor ;//Armazena o valor do item que vai ser removido

      if (remove_item->ant) { //Se houver um item anterior, irá atualizar o ultimo da lista
        lst->ultimo = remove_item->ant ;
        lst->ultimo->prox = NULL ;
      } 
      else { //Caso contrário, a lista ficará vazia
        lst->primeiro = NULL ;
        lst->ultimo = NULL ;
      }
    
    free(remove_item) ; //Libera a memória do item removido
    lst->tam-- ; //Diminui o tamanho da lista
    return lst->tam ;
  }
    else if (pos == 0) { // Caso em que remove a primeira posição
      remove_item = lst->primeiro ;
      *item = remove_item->valor ;

      if (remove_item->prox) { //Se tiver um proximo a primeira posição
        lst->primeiro = remove_item->prox ; //O proximo será atualizado como primeiro 
        lst->primeiro->ant = NULL ; //O anterior será removido 
      }
      else { //Se nao tiver um proximo , a lista ficará vazia
        lst->primeiro = NULL ; //Libera a memória do item removido
        lst->ultimo = NULL ; //Diminui o tamanho da lista
      }
      free(remove_item) ; //Libera a memória do item removido
      lst->tam-- ; //Diminui o tamanho 
      return lst->tam ;
    }
    else { //Caso de remoção intermediároa
      remove_item = lst->primeiro ; //Atualiza para começar no primeiro
      for (int i=0; i<pos; i++ )  // Percorre a lista até a posição desejada
        remove_item = remove_item->prox ; 

      *item = remove_item->valor ; //Armazena o item a ser removido

      if (remove_item->ant) //Se ao apontar para o anterior e ele nao for nulo
      remove_item->ant->prox =  remove_item->prox ; //Atualiza o ponteiro do proximo ao anterior para apontar para o item seguinte do removido 

      if (remove_item->prox) //Se ao apontar para o proximo e ele nao for nulo 
      remove_item->prox->ant = remove_item->ant ; //Atualiza o ponteiro anterior do item seguinte para apontar para o anterior do item removido
    
    

    free(remove_item) ;
    lst->tam-- ;

    return lst->tam ;
  }
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos) {

  if (!lst || !item) //Verifica se a lista ou o item são ponteiros válidos 
    return -1 ;
  if (!lst->tam) //Verifica se a lista não está vazia
    return -1 ;
  
  struct item_t *consulta_item = NULL ;

  if (pos == -1 ) //  Se a posição for -1, consulta o fim 
    consulta_item = lst->ultimo ;
  else if (pos == 0) //Se a posição estiver no inicio
    consulta_item = lst->primeiro ; //Acessamos o primeiro item da lista
  else {
    consulta_item = lst->primeiro ; 
    for (int i=0; i<pos; i++) { //Percorre a lista
      if (!consulta_item) //Se o ponteiro for nulo, a posição será inválida
        return -1 ;
      consulta_item = consulta_item->prox ; //O consulta item receberá o próximo item 
    }
  }
  if (!consulta_item) //Verificação extra se não conseguiu o item desejado
    return -1 ;

  *item = consulta_item->valor ; //Armazena o valor do item encontrado no endereço apontado por "item"
  return lst->tam ;

}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor) {
  
  if (!lst) //Verifica se o ponteiro da lista é valido
    return -1 ;
  if (!lst->tam) //Verifica se a lista está vazia
    return -1 ;
  
  struct item_t *procura_item = lst->primeiro ; //Cria um ponteiro que aponta para o primeiro nó na lista
  int pos = 0 ; // Representa a posição atual do nó na lista

  /*Percorre enquanto nao chega no final */
  while (procura_item != NULL) {

    if (procura_item->valor == valor) // Se o valor armazenado dentro do nó for igual o valor, ele retornará a posição
      return pos;
    procura_item = procura_item->prox ; //Atualiza o ponteiro para o próximo 
    pos++ ;
  }

  return -1 ; //Valor nao encontrado
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst) { 
  if (!lst) // Retorna -1 se o ponteiro da lista for inválido
    return -1 ;
  
  return lst->tam ; //Retorna o tamanho da lista 
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst) { 

  if (!lst) //Verifica o ponteiro da lista 
    return  ;
  if (!lst->tam) //Verifica o tamanho da lista
    return ;
  
  struct item_t *imprime_item = lst->primeiro ; //Cria uma estrutura para percorrer a lista
  int pos = 0 ; //Inicializa a posição com 0


  while (imprime_item != NULL) { //Percorre enquanto nao chega no final 
    if (pos>0) //Cria o espaço entre os itens
      printf(" ") ; 
    printf("%d", imprime_item->valor) ; //Imprime o elemento 
    imprime_item = imprime_item->prox ;  //Vai para o próximo elemento 
    pos++ ;
  }
}
