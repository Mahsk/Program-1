#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"
#include "entidade.h"

//Faz a função aleatoria
int aleat(int min, int max) {

    return (rand()%(max - min + 1)) + min;
}

void cria_mundo_MAIN(struct mundo_t **world, struct fprio_t **lef) {

    *world = cria_mundo();
    *lef = fprio_cria();
}

void inicia_mundo_MAIN(struct mundo_t *world, struct fprio_t *lef) {

    //Chama a função inicia evento
    iniciar_evento(lef, world);

    //Para nao inicializar com lixo de memoria
    int tipo = 0;
    int tempo = 0;
    //Inteiro para saber se a simulação terminou ou nao 
    int simulacao_chegou_fim = 0;
    //Ponteiro para o evento retirado da fila
    struct evento_t *evento = NULL;

    //Retira o proximo evento da fila de prioridade
    while (!simulacao_chegou_fim && (evento = fprio_retira(lef,&tipo,&tempo)) != NULL) {
        
        //Contador de eventos que será utilizado no FIM
        world->eventos_tratados++ ;
        //Atualza o relogio com o tempo do evento
        world->relogio = evento->tempo;

        //Caso o evento seja do tipo x, executa
        switch (evento->tipo) {
            case CHEGA:
                evento_chega(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case ESPERA:
                evento_espera(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case DESISTE:
                evento_desiste(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case AVISA:
                evento_avisa(world, lef, evento->tempo,  evento->base);
                break;
            case ENTRA:
                evento_entra(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;
            case SAI:
                evento_sai(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;
            case VIAJA:
                evento_viaja(world, lef, evento->tempo, evento->heroi,  evento->base);
                 break; 
            case MISSAO:
                evento_missao(world,  lef, evento->tempo, evento->missao);
                break; 
            case MORRE:
                evento_morre(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;   
            case FIM:
                evento_fim(world,evento->tempo);
                simulacao_chegou_fim = 1;
                break;
        }
        //Libera o ponteiro do evento
        free(evento) ;
    }
}

void destroi_mundo_MAIN(struct mundo_t *world, struct fprio_t *lef) {
    destroi_mundo(world);
    fprio_destroi(lef);
}

struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base,int missao) {

    
    struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
    if(!evento)
        return NULL ;

    evento->tempo = tempo ;
    evento->tipo = tipo ;
    evento->heroi = heroi;
    evento->base = base ;
    evento->missao = missao ;

    return evento ;
}

struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base,int missao) {
    struct evento_t *novo_evento = cria_evento(tempo,tipo,heroi,base,missao) ;

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
        if(world->herois[h].base_atual == BMP && cjto_pertence(world->bases[BMP].presentes,h))
            world->herois[h].experiencia++ ;
    }
}

int minimo_tentativas(struct mundo_t *world) {

    int min_tent = world->missoes[0].tentativas ;
    for(int i = 0; i < world->NMissoes ; i++) {
        if(world->missoes[i].tentativas < min_tent)
            min_tent = world->missoes[i].tentativas ;
    }
    return min_tent ;
}


int maximo_tentativas(struct mundo_t *world) {

    int max_tent = world->missoes[0].tentativas ;
    for(int i = 0; i < world->NMissoes ; i++) {
        if(world->missoes[i].tentativas > max_tent)
            max_tent = world->missoes[i].tentativas ;
    }
    return max_tent ;
}

int total_tentativas(struct mundo_t *world) {

    int t_tent = 0;
    for(int i = 0; i < world->NMissoes; i++) {
        t_tent += world->missoes[i].tentativas ;
    }   
    return(t_tent) ;
}