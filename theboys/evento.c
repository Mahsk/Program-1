#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "evento.h"
#include "mundo.h"

void evento_chega (struct fprio_t *lef, struct mundo *world, struct evento *ev) {
    
    if (!world || !ev) 
      return ;

    int h = ev->heroi ; //Extrai o ID do heroi do evento
    int b = ev->base ; //Extrai o ID da base do evento 

    if (h < 0|| h >= world->NHerois) 
      return ;
    if (b < 0|| b >= world->NBases)
      return ;

    struct heroi *herois = &world->herois[h] ; //Ponteiro para o herói atual
    struct base *bases = &world->bases[b] ; //Ponteiro para a base atual 

    herois->base = b ; //Atualiza a base do heroi 

    int ocupado = cjto_card(bases->presentes) ;
    int vagas = (bases->lotacao - ocupado) ;//Calcula as vagas livres = lotação - quantos herois ja estao na base
    int tam_fila_esp = fila_tamanho(bases->espera) ; //Tamanho da fila de espera
    
    bool espera ;

    if (vagas > 0 && tam_fila_esp == 0) { //se há vaga e fila vazia
        espera = true ; 
        cjto_insere(bases->presentes, h) ;
    }
    else {
        if (herois->paciencia > 10 * tam_fila_esp ) {//decide se espera
              espera = true ;

              /*Aloca a memória para guardar o ID do heroi(tive que usar, pois a fila trabalha com void e o tipo usado é int)*/
              int *id = malloc(sizeof(int)) ;
                if (!id)
                  exit(1) ;
              *id = h ; //Guarda o valor do ID dentro do ponteiro alocado
              fila_insere(bases->espera, id) ;
        }
        else
              espera = false ;
    }

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ", ev->tempo, h, b, ocupado, bases->lotacao ) ;
    if (espera)
      printf("ESPERA\n") ;
    else
      printf("DESISTE\n") ;

    struct evento *novo_ev = malloc(sizeof(struct evento)) ;
      if (!novo_ev) {
        exit(1) ;
      }

        novo_ev->tempo = ev->tempo ;
        novo_ev->heroi = h ;
        novo_ev->base = b ;

      if (espera)
        novo_ev->tipo = ESPERA ;
      else
        novo_ev->tipo = DESISTE ;

    fprio_insere(lef, novo_ev, novo_ev->tipo, novo_ev->tempo) ;

    
}
void evento_espera (struct fprio_t *lef, struct mundo *world, struct evento *ev) {

    if (!lef||!ev || !world) //Verifica os ponteiros
        return ;

    int h = ev->heroi ; //ID do herói que está esperando
    int b = ev->base ; //ID da base em que ocorre a espera
    int tempo_atual = ev->tempo ; //momento que o evento ocorre

    if (h < 0|| h >= world->NHerois) 
      return ;
    if (b < 0|| b >= world->NBases)
      return ;

    //struct base *base = &world->bases[b] ;

    /*Cria e insere o evento AVISA*/
    struct evento *evento_avisa = malloc(sizeof(struct evento)) ;
    if (!evento_avisa) {
        exit(1) ;
    }

    evento_avisa->tempo = tempo_atual ;
    evento_avisa->heroi = h ;
    evento_avisa->base = b ;
    evento_avisa->tipo = AVISA ;

    fprio_insere(lef, evento_avisa, AVISA, tempo_atual) ;
}

void evento_desiste(struct fprio_t *lef, struct mundo *world, struct evento *ev) {

  if (!lef|| !world|| !ev)
    return ;

  int h = ev->heroi ;
  int b = ev->base ;
  int tempo_atual = ev->tempo ;


  if (h < 0|| h >= world->NHerois) 
    return ;
  if (b < 0|| b >= world->NBases)
    return ;

  printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo_atual, h, b) ;

  int d = aleat(0, world->NBases - 1) ; //Escolhe uma base destino aleatória

  struct evento *evento_viaja = malloc(sizeof(struct evento)) ;

  if (!evento_viaja)
    exit(1)  ;
  
  evento_viaja->tipo = VIAJA ;
  evento_viaja->tempo = ev->tempo ;
  evento_viaja->heroi = h ;
  evento_viaja->base = d ;

  fprio_insere(lef, evento_viaja, VIAJA, ev->tempo) ;

}

void evento_avisa(struct fprio_t *lef, struct mundo *world, struct evento *ev) {

  if (!lef || !world|| !ev)
    return ;
  
  int b = ev->base ; //Id da base onde avisa esta ocorrendo
  int tempo_atual = ev->tempo ; //tempo atual dele


  if (b < 0|| b >= world->NBases)
    return ;  

  printf("%6d: AVISA BASE %d\n", tempo_atual, b) ;

  struct base *base = &world->bases[b]; //Obtem o ponteiro da base correspondente 


  int vagas = base->lotacao - cjto_card(base->presentes) ; //

  while(vagas > 0 && base->espera && base->espera->num > 0) {

    int *prim_h = fila_retira(base->espera) ; //retira o primeiro heroi
      if (!prim_h)
        break ;
    int h = *prim_h  ; //obtem o id do heroi
      free(prim_h) ; //libera a memoria alocada

    cjto_insere(base->presentes, h) ; //adiciona o heroi ao conjunto de herois presentes na base

    struct evento *entra = malloc(sizeof(struct evento)) ;
      entra->tipo = ENTRA ;;
      entra->tempo = tempo_atual ;
      entra->heroi = h ;
      entra->base = b ;

      fprio_insere(lef, entra, ENTRA, tempo_atual) ; //insere ENTRA na LEF

      vagas-- ; //reduz o numero de vagas ;
  }

}

void evento_entra(struct fprio_t *lef, struct mundo *world, struct evento *ev) {
  if (!lef || ! world|| !ev)
    return ;
  
  int h = ev->heroi ;
  int b = ev->base ;
  int tempo_atual = ev->tempo ;

  if (h < 0|| h >= world->NHerois) 
    return ;
  if (b < 0|| b >= world->NBases)
    return ;

  printf("%6d: ENTRA HEROI %d BASE %d\n", tempo_atual, h, b) ;

  /*Calcula o TPB(tempo de permanencia na base)*/
  int paciencia = world->herois[h].paciencia ;
  int TPB = 15 + paciencia * aleat(1,20) ;

  struct evento *sai = malloc(sizeof(struct evento)) ;
  if (!sai) {
    exit(1) ;
  }
  
  sai->tipo = SAI;
  sai->tempo = tempo_atual + TPB ;
  sai->heroi = h ;
  sai->base = b ;
  
  fprio_insere(lef, sai, SAI, tempo_atual + TPB) ;

}  

void evento_sai(struct fprio_t *lef, struct mundo *world, struct evento *ev ) {
  if (!lef || !world|| !ev) 
    return  ;
  
  int h = ev->heroi ; //ID do heroi
  int b = ev->base ; //ID da base
  int tempo_atual = ev->tempo ; //tempo atual do evento
  
  if (h < 0|| h >= world->NHerois) 
    return ;
  if (b < 0|| b >= world->NBases)
    return ;

  printf("%6d: SAI HEROI %d BASE %d\n", tempo_atual, h, b) ;

  cjto_retira(world->bases[b].presentes, h) ; //remove o heroi do conjunto de presentes da base

  int d = (b + 1 + aleat(0, world->NBases - 2)) % world->NBases; //escolhe uma base destino "d" aleatória

  /*Cria e insere na LEF o evento VIAJA (agora, h, d)*/
  struct evento *viaja = malloc(sizeof(struct evento)) ;
  if (!viaja)  {
    exit(1) ;
  }
    viaja->tipo = VIAJA ;
    viaja->tempo = tempo_atual ;
    viaja->heroi = h ;
    viaja->base = d;

    fprio_insere(lef, viaja, VIAJA, tempo_atual) ;

  /*Cria e insere na LEF o evento AVISA(agora,b)*/
  struct evento *avisa = malloc(sizeof(struct evento)) ;
    if (!avisa)
      exit(1) ;

    avisa->tipo = AVISA ;
    avisa->tempo = tempo_atual ;
    avisa->heroi = h ;
    avisa->base = b ;
    fprio_insere(lef, avisa, AVISA, tempo_atual) ;

}

void evento_viaja(struct fprio_t *lef, struct mundo *world, struct evento *ev) {
  if (!lef|| !world|| !ev) 
    return ;
  
  int h = ev->heroi ;
  int d = ev->base ;
  int tempo_atual = ev->tempo ;

  if (h < 0|| h >= world->NHerois) 
    return ;
  if (d < 0|| d >= world->NBases)
    return ;
  printf("%6d: VIAJA HEROI %d DESTINO BASE %d\n", tempo_atual, h, d) ;


  int origem = world->herois[h].base ; //posição atual do heroi

  int x1 = world->bases[origem].coordena_l.x ; //base x de origem
  int y1 = world->bases[origem].coordena_l.y ; //base y de origem
  int x2 = world->bases[d].coordena_l.x ; //base x de destino
  int y2 = world->bases[d].coordena_l.y ; //base y de destino

  /*Calcula a distancia */  
  double dx = x2 -x1 ;
  double dy = y2 - y1 ;
  double distancia = sqrt(dx * dx + dy * dy) ; //sqrt((x2-x1)² + (y2-y1)^2)

  /*Calcula a duração*/
  double duracao = distancia / world->herois[h].velocidade ;

  int tempo_fut_chegada = tempo_atual + (int) duracao ;


  struct evento *chega = malloc(sizeof(struct evento)) ;

  if (!chega) 
    exit(1) ;
  
  chega->tipo = CHEGA ;
  chega->tempo = tempo_fut_chegada ;
  chega->heroi = h ;
  chega->base = d ;

  fprio_insere(lef, chega, CHEGA, tempo_fut_chegada) ;

}

void evento_morre(struct fprio_t *lef, struct mundo *world, struct evento *ev ) {
  if (!lef || !world|| !ev)
    return ;
  
  int h = ev->heroi ;
  int b = ev->base ;
  int tempo_atual = ev->tempo ;
  int m = ev->missao ;

  if (h < 0|| h >= world->NHerois|| b < 0|| b >= world->NBases) 
    return ;
  
  struct heroi *heroi = &world->herois[h] ;
  struct base *base = &world->bases[b] ;

  cjto_retira(base->presentes, h) ; //o heroi é retirado da base

  heroi->vivo = false ; //marca o heroi como morto 

  printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo_atual, h, m) ;

  /*Cria evento AVISA(para liberar a vaga da base)*/
  struct evento *avisa = malloc(sizeof(struct evento)) ;
  if (!avisa) 
    exit(1) ;
  
  avisa->tipo = AVISA ;
  avisa->tempo = tempo_atual ;
  avisa->heroi = h ;
  avisa->base = b ;

  fprio_insere(lef, avisa, AVISA, tempo_atual) ;

}

void evento_missao(struct fprio_t *lef, struct mundo *world, struct evento *ev) {
  if (!lef|| !world|| !ev) 
    return  ;
  
  int m = ev->missao ; //ID da missao 
  int tempo_atual = ev->tempo ; //tempo do evento atual 

  if (m < 0|| m >= world->NMissoes)
    return ; //verifica se o ID da missao é valido
  
  struct missao *missao_atual = &world->missoes[m] ; //ponteiro para missao atual
  missao_atual->tentativas++ ;

  printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo_atual, m, missao_atual->tentativas) ; //imprime informação da missao
  for (int i = 0; i < world->NHabilidades; i++) {
    if (cjto_pertence(missao_atual->habilidades, i))
      printf(" %d ",i) ; //imprime cada habilidade pedida
  }
  printf("]\n") ;
  
  /*Variáves para armazenar a base mais próxima apta*/
  struct base *bmp = NULL ; //ponteiro para a base mais próxima apta 
  double menor_dist = -1 ; //armazena a menor distancia encontrada


  for (int i = 0; i < world->NBases; i++) { //Percorre todas as bases do mundo para encontrar a base mais proxima que seja apta
    struct base *base_atual = &world->bases[i] ;

    /*Calcula distancia entre base e missao*/
    double dx = base_atual->coordena_l.x - missao_atual->coordena_l.x ;
    double dy = base_atual->coordena_l.y - missao_atual->coordena_l.y ;
    double dist = sqrt(dx*dx + dy*dy) ;

    struct cjto_t *uniao_habil = cjto_cria(world->NHabilidades) ; //conjunto vazio para armazenar as habilidades
    if (!uniao_habil) 
      return;
    /*Percorre todos os herois para adicionar as habilidades do heroi na base*/
    for(int h = 0; h < world->NHerois; h++) {
      if (world->herois[h].vivo && cjto_pertence(base_atual->presentes, h)) {
        for (int hab = 0; hab < world->NHabilidades; hab++)
          if (cjto_pertence(world->herois[h].habilidades, hab))
            cjto_insere(uniao_habil, hab) ;

        }
    }
  

    
    /*Verifica se a base é apta*/
    bool apta = cjto_contem(uniao_habil, missao_atual->habilidades) ;

    if(apta && (menor_dist < 0|| dist < menor_dist)) { //se a base é apta e mais proxima que a anterior, atualiza a escolha

      menor_dist = dist ; //atualizamenor distancia
      bmp = base_atual ; //atualiza base mais poxima da missao
    }
    cjto_destroi(uniao_habil) ;
  }

  if (bmp) { //Se encontrou uma base apta
    

  printf("%6d: MISSAO %d CUMPRIDA BASE %d (HABS:[ ", tempo_atual, missao_atual->id, bmp->id);
    
    // Imprime habilidades dos heróis na base
    for (int h = 0; h < world->NHerois; h++) {                                                          
        if (world->herois[h].vivo && cjto_pertence(bmp->presentes,h)) {
            world->herois[h].experiencia += 10;
            // Imprime habilidades deste herói
            for (int hab = 0; hab < world->NHabilidades; hab++) {
                if (cjto_pertence(world->herois[h].habilidades, hab))
                    printf("%d ", hab);
            }
        }
    }
    printf("])\n");


    struct base *next_bmp = NULL ;
    menor_dist = -1 ;

    /*Busca a base mais proxima*/
    for (int i = 0; i < world->NBases; i++) {
      struct base *b = &world->bases[i] ;
      int dx = b->coordena_l.x - missao_atual->coordena_l.x;
      int dy = b->coordena_l.y - missao_atual->coordena_l.y ;
      int dist = sqrt(dx*dx + dy*dy);

      if(menor_dist < 0|| dist < menor_dist) {
        next_bmp = b;
        menor_dist = dist ;
      }
    }

    if (tempo_atual % 2500 == 0 && world->NCompostosV > 0 && next_bmp) {
      int escolhido = -1, max_xp = -1;

      for (int h = 0; h < world->NHerois; h++) {
        if (world->herois[h].vivo && cjto_pertence(next_bmp->presentes, h)) {
          if (world->herois[h].experiencia > max_xp) {
            escolhido = h ;
            max_xp = world->herois[h].experiencia;
          }
      }
    }
  
    if (escolhido != -1) {
      printf("%6d: MISSAO %d CUMPRIDA COMPOSTO V HEROI %d BASE %d\n", tempo_atual, m , escolhido, next_bmp->id);
      world->NCompostosV--;
      

      for (int h = 0; h < world->NHerois; h++)
        if (h != escolhido && world->herois[h].vivo && cjto_pertence(next_bmp->presentes, h))
          world->herois[h].experiencia += 10 ;
      
      struct evento *morre = malloc(sizeof(struct evento));
        morre->tipo = MORRE;
        morre->tempo = tempo_atual;
        morre->heroi = escolhido ;
        morre->base = next_bmp->id ;
        morre->missao = m;
        fprio_insere(lef, morre, MORRE,tempo_atual) ;
        return ;
  }
    int proximo = tempo_atual + 24*60;

    /*Se nao encontrou a missao*/
    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo_atual, m) ;

    if (proximo <= T_FIM_DO_MUNDO) {
      struct evento *nova_missao = malloc(sizeof(*nova_missao));
      if (!nova_missao)
           exit(1);

        nova_missao->tipo   = MISSAO;
        nova_missao->tempo  = proximo;
        nova_missao->missao = m;
      fprio_insere(lef, nova_missao, MISSAO, proximo);
      }
     }
  }
}









