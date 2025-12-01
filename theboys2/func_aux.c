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

/////////////////////////// Inicialização na MAIN ///////////////////////////
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

/////////////////////////// Criaçaõ e Inserção na LEF ///////////////////////////

struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base,int missao) {

    //Aloca memoria para evento 
    struct evento_t *evento = malloc(sizeof(struct evento_t)) ;

   //Se a alocação falhar 
    if(!evento)
        return NULL ;

    //Inicializa as estruturas do novo evento
    evento->tempo = tempo ;
    evento->tipo = tipo ;
    evento->heroi = heroi;
    evento->base = base ;
    evento->missao = missao ;

    return evento ;
}

struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base,int missao) {
    
    //Cria o novo evento
    struct evento_t *novo_evento = cria_evento(tempo,tipo,heroi,base,missao) ;

    if(!novo_evento)
        return NULL;

    //Insere o novo evento na lef
    fprio_insere(lef, novo_evento, tipo,tempo) ;

    return novo_evento ;
}

/////////////////////////// Funções auxiliares para os EVENTOS ///////////////////////////


int distancia_cartesiana(struct mundo_t *world,int origem, int destino) {

    //Vejo a localidade de origem e destino
    int x1 = world->bases[origem].local.x ;
    int x2 = world->bases[destino].local.x ;
    int y1 = world->bases[origem].local.y ;
    int y2 = world->bases[destino].local.y ;

    //Calculo da distancia cartesiana
    int dx = x2 -x1  ;
    int dy = y2 - y1 ;
    double dc = (double)(dx * dx) + (double)(dy * dy) ;

    return (int)(sqrt(dc)) ;
} 

int distancia_cart_BM(struct coordenadas_t base, struct coordenadas_t missao) {

    //Calculo da distancia cartesiana Base - Missao
    double dc ;
    dc = pow((missao.x - base.x),2) + pow((missao.y - base.y),2) ;
    
    return (int)(sqrt(dc)) ;
} 

void uniao_habilidades(struct mundo_t *world, struct bases_t *bases) {

    //Cria um conjunto de uniao de habilidades
    struct cjto_t *uniao_hab = cjto_cria(world->NHabilidades) ;

    //Percorre o numero de herois
    for(int h = 0; h < world->NHerois; h++) {
       //Se o heroi esta na base 
        if(cjto_pertence(bases->presentes,h)) {
            struct cjto_t *novo = cjto_uniao(uniao_hab, world->herois[h].habilidades) ;
            cjto_destroi(uniao_hab) ;
            uniao_hab = novo ;
        }
    }
    //Destroi o conjunto de habilidades antigo da base
    if(bases->habilidades)
        cjto_destroi(bases->habilidades) ;

    //Substitui pelo novo conjunto
    bases->habilidades = uniao_hab ;
}

int base_apta(struct mundo_t *world,struct bases_t *bases, struct missoes_t *missoes) {

    //Chama a função uniao de habilidades
    uniao_habilidades(world, bases) ;

    //Faz a interseção das habilidades da bases e das missoes
    struct cjto_t *intersecao = cjto_inter(bases->habilidades, missoes->habilidades) ;

    //Se a interseção for igual da missao, retorna 1, caso contratio, recebe 0
    int apta = cjto_iguais(intersecao,missoes->habilidades) ;

    cjto_destroi(intersecao) ;
    return apta ;
}

int heroi_experiente(struct mundo_t *world, int BMP) {

    int id_heroi_maisXP = -1 ;
    int maior_exp = -1;

    //Percorre os herois
    for(int h = 0; h < world->NHerois; h++) {
        //Se o heroi da base estiver na base mais proxima NAO apta e se a experiencia for maior
        if(world->herois[h].base_atual == BMP  && world->herois[h].experiencia > maior_exp) {
            //Atualiza a maior experienca
            maior_exp = world->herois[h].experiencia ;
            //Atualiza o id do heroi mais experiente
            id_heroi_maisXP = h ;
        }
    }
    return id_heroi_maisXP ;
}

void incrementa_experiencia(struct mundo_t *world, int BMP) {

    //Percorre os herois
    for(int h = 0; h < world->NHerois; h++){
        //Verifica se esta na BMOO e se o heroi pertence a BMP
        if(world->herois[h].base_atual == BMP && cjto_pertence(world->bases[BMP].presentes,h))
            world->herois[h].experiencia++ ;
    }
}

int minimo_tentativas(struct mundo_t *world) {

    int min_tent = world->missoes[0].tentativas ;

    //Percorre as missoes
    for(int i = 0; i < world->NMissoes ; i++) {
        //Se o numero de tentativa for menor do que a do começo, atualiza
        if(world->missoes[i].tentativas < min_tent)
            min_tent = world->missoes[i].tentativas ;
    }
    return min_tent ;
}


int maximo_tentativas(struct mundo_t *world) {

    int max_tent = world->missoes[0].tentativas ;

    //Percorre as missoes
    for(int i = 0; i < world->NMissoes ; i++) {
        //Se o numero de tentativa for maior do que a do começo, atualiza
        if(world->missoes[i].tentativas > max_tent)
            max_tent = world->missoes[i].tentativas ;
    }
    return max_tent ;
}

int total_tentativas(struct mundo_t *world) {

    int t_tent = 0;

    //Percorre as missoes
    for(int i = 0; i < world->NMissoes; i++) {
        //Soma as tentativas de cada missao
        t_tent += world->missoes[i].tentativas ;
    }   
    return(t_tent) ;
}