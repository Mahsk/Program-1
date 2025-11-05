#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "mundo.h"
#include "evento.h"
#include "fila.h"
#include "conjunto.h"
#include "fprio.h"


//Criando o mundo 
struct mundo *cria_mundo() {
  struct mundo *world = malloc(sizeof(struct mundo)) ;

  if (!world) //se nao conseguir alocar o mundo
    return NULL ;
  
  /*Inicializa dados globais*/
  world->NHerois = N_HEROIS ;
  world->NBases = N_BASES ;
  world->NMissoes = N_MISSOES ;
  world->NHabilidades = N_HABILIDADES ;
  world->NTamanhoMundo = N_TAMANHO_MUNDO ;
  world->NCompostosV = N_COMPOSTOS_V ;
  world->relogio = 0 ;


  //Alocar os heróis 
  world->herois = malloc(sizeof(struct heroi) * world->NHerois) ;
  if (!world->herois) {
    free(world) ;
    return NULL ;
  }

  for (int i=0; i < world->NHerois; i++) {
    world->herois[i].id = i ;
    world->herois[i].experiencia = 0 ;
    world->herois[i].paciencia = aleat(0,100) ; //{0,100}
    world->herois[i].velocidade = aleat(50,5000) ;
    world->herois[i].base = -1 ; //o heroi começa sem base
    world->herois[i].vivo = true ;

    /*Cria as habilidades aleatorias de 1 a 3*/
    int qntdd_habil = aleat(1,3) ; 

    world->herois[i].habilidades = cjto_aleat(qntdd_habil, world->NHabilidades) ;

    if (!world->herois[i].habilidades) { //trata possíveis falhas
      free(world->herois) ;
      free(world);
      return NULL ;
    }
  } 

  //Aloca as bases
  world->bases = malloc(sizeof(struct base) * world->NBases) ;

  if (!world->bases) {
    free(world->herois) ;
    free(world);
    return NULL ;
  }

  for (int i = 0 ; i < world->NBases; i++){

    world->bases[i].id = i ;
    world->bases[i].coordena_l.x = aleat (0,N_TAMANHO_MUNDO -1) ;
    world->bases[i].coordena_l.y = aleat (0, N_TAMANHO_MUNDO -1) ;
    world->bases[i].lotacao = aleat(3,10) ;
    world->bases[i].presentes = cjto_cria(world->bases[i].lotacao) ;
    world->bases[i].espera = fila_cria() ;

  }

  //Aloca as missões
  world->missoes = malloc(sizeof(struct missao) * world->NMissoes) ;

  if (!world->missoes) {
    free(world->herois) ;
    free(world) ;
    return NULL ;
  }
    
    for (int i = 0; i < world->NMissoes; i++){

    world->missoes[i].id = i ;
    

    world->missoes[i].coordena_l.x = aleat(0,N_TAMANHO_MUNDO - 1) ;
    world->missoes[i].coordena_l.y = aleat(0, N_TAMANHO_MUNDO - 1) ;

    int cjto_hab = aleat(6,10) ;
    world->missoes[i].habilidades = cjto_aleat(cjto_hab, world->NHabilidades) ;
    world->missoes[i].tentativas = 0 ;

  }
  return world ;

}
int aleat(int min, int max) {
    return min + rand() % (max - min + 1) ;
} 

//Cria e insere na lef eventos iniciais
void iniciar_evento(struct fprio_t *lef, struct mundo *world) {
  
  /*Evento para herois */
  for (int i = 0; i < world->NHerois; i++) {
    struct evento *ev = malloc(sizeof(struct evento)) ;

    if (!ev) {
      exit(1) ;
    }

    ev->tempo = aleat(0,4320) ; //4320 = 3 dias em minutos
    ev->tipo = CHEGA ;
    ev->heroi = i ;
    ev->base = aleat(0, world->NBases -1) ;

    fprio_insere(lef, ev, CHEGA, ev->tempo) ;
  }

  //Evento missao 
  for (int i = 0; i < world->NMissoes; i++) {
    struct evento *ev = malloc(sizeof(struct evento)) ;
    
    if (!ev) { //se nao conseguir alocar [
      exit(1);
    }

    ev->tempo = aleat(0,T_FIM_DO_MUNDO) ;
    ev->tipo = MISSAO ;
    ev->missao = world->missoes[i].id ;

    
    fprio_insere(lef, ev, MISSAO, ev->tempo) ;
  }

  //Evento FIM (único, nao precisa do for)
  struct evento *ev = malloc(sizeof(struct evento)) ;
  if (!ev) {
    exit(1) ;
  }

  ev->tempo = T_FIM_DO_MUNDO ;
  ev->tipo = FIM ;

  fprio_insere(lef, ev, FIM, ev->tempo) ;
  
}



//Destruir mundo
void destroi_mundo(struct mundo *world) {
  if (!world)
    return ;
  
  //Destroi herois 
  if (world->herois) {

    for (int i = 0; i < world->NHerois; i++) {

      if (world->herois[i].habilidades) 
        cjto_destroi(world->herois[i].habilidades) ;

    }
    free(world->herois) ;
  }
  //Destroi bases 
  if (world->bases) {
    
    for(int i = 0; i <  world->NBases; i++) {

      if (world->bases[i].presentes)
        cjto_destroi(world->bases[i].presentes) ;
      if (world->bases[i].espera) {
        int *id = fila_retira(world->bases[i].espera) ;
        while (id != NULL) {
          if (id)
              free(id) ;
        /*int *pid;
        // retira cada ID alocado e libera
        while ((pid = fila_retira(world->bases[i].espera)) != NULL) {
            free(pid);*/
        }
          fila_destroi(world->bases[i].espera);
      }
      /*if (world->bases[i].espera)
        fila_destroi(world->bases[i].espera) ;*/
    }
    free(world->bases) ;
  }

  //Destroi missões
  if (world->missoes) {

    for (int i = 0; i < world->NMissoes; i++) {
      if (world->missoes[i].habilidades)
        cjto_destroi(world->missoes[i].habilidades) ;
    }
    free(world->missoes) ;
  }
  free(world) ;
}


  






