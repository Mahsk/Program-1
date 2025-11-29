#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entidade.h"
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"

int aleat(int min, int max) {
    //return min + rand() % (max - min + 1) ;
    return (rand()%(max - min + 1)) + min;
}

struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base) {
    struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
    if(!evento)
        return NULL ;

    evento->tempo = tempo ;
    evento->tipo = tipo ;
    evento->heroi = heroi;
    evento->base = base ;

    return evento ;
}

struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base) {
    struct evento_t *novo_evento = cria_evento(tempo,tipo,heroi,base) ;

    if(!novo_evento)
        return NULL;

    fprio_insere(lef, novo_evento, tipo,tempo) ;

    return novo_evento ;
}

int distancia_cartesiana(struct mundo_t *world,int origem, int destino) {

    int x1 = world->bases[origem].local.x ;
    int x2 = world->bases[destino].local.x ;
    int y1 = world->bases[origem].local.y ;
    int y2 = world->bases[destino].local.y ;

    int dx = x2 -x1  ;
    int dy = y2 - y1 ;
    double dc = (double)(dx * dx) + (double)(dy * dy) ;

    return (int)(sqrt(dc)) ;
} 

int distancia_cart_BM(struct coordenadas_t base, struct coordenadas_t missao) {

    double dc ;
    dc = pow((missao.x - base.x),2) + pow((missao.y - base.y),2) ;
    
    return (int)(sqrt(dc)) ;
} 

void uniao_habilidades(struct mundo_t *world, struct bases_t *bases) {

    struct cjto_t *uniao_hab = cjto_cria(world->NHabilidades) ;

    for(int h = 0; h < world->NHerois; h++) {
        if(cjto_pertence(bases->presentes,h)) {
            struct cjto_t *novo = cjto_uniao(uniao_hab, world->herois[h].habilidades) ;
            cjto_destroi(uniao_hab) ;
            uniao_hab = novo ;
        }
    }
    //destroi o conjunto de habilidades antigo da base
    if(bases->habilidades)
        cjto_destroi(bases->habilidades) ;
    //substitui pelo novo conjunto
    bases->habilidades = uniao_hab ;
}

int base_apta(struct mundo_t *world,struct bases_t *bases, struct missoes_t *missoes) {

    uniao_habilidades(world, bases) ;

    struct cjto_t *intersecao = cjto_inter(bases->habilidades, missoes->habilidades) ;

    int apta = cjto_iguais(intersecao,missoes->habilidades) ;

    cjto_destroi(intersecao) ;
    return apta ;
}


int heroi_experiente(struct mundo_t *world, int BMP) {

    int id_heroi_maisXP = -1 ;
    int maior_exp = -1;
    for(int h = 0; h < world->NHerois; h++) {
        if(world->herois[h].base_atual == BMP  && world->herois[h].experiencia > maior_exp) {
            maior_exp = world->herois[h].experiencia ;
            id_heroi_maisXP = h ;
        }
    }
    return id_heroi_maisXP ;
}

void incrementa_experiencia(struct mundo_t *world, int BMP) {

    for(int h = 0; h < world->NHerois; h++){
        if(world->herois[h].base_atual == BMP)
            world->herois[h].experiencia++ ;
    }
}
