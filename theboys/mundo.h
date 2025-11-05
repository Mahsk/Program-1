#ifndef MUNDO_H
#define MUNDO_H

#include <stdbool.h>
#include "fila.h"
#include "conjunto.h"

struct evento ;

#define T_INICIO  0
#define T_FIM_DO_MUNDO 4000
#define N_TAMANHO_MUNDO 200
#define N_HABILIDADES 10 
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_COMPOSTOS_V N_HABILIDADES * 3


/*Estrutura do heroi*/
struct heroi {
    int id ;
    int experiencia ;
    int paciencia ;
    int velocidade ;
    struct cjto_t *habilidades ;
    int base ; //ID da base atual 
    bool vivo ; //status de vida do heroi 
} ;

struct local_t {
    int x ;
    int y ;
} ;

struct base {
    int id ;
    int lotacao ;
    struct cjto_t *presentes ;
    struct fila_t *espera ;
    struct local_t coordena_l ;
    
} ;

struct missao {
    int id ;
    struct local_t coordena_l ;
    struct cjto_t *habilidades ;
    int tentativas ;

} ;



int aleat (int min, int max) ;


struct mundo {
    int NHerois ;
    int NBases ;
    int NMissoes ;
    int NHabilidades ;
    int NCompostosV ;
    int NTamanhoMundo ;
    int relogio ;
    struct heroi *herois ; //Vetor de herois
    struct base *bases ; //Vetor de bases 
    struct missao *missoes ; //Vetor de missao 
    struct fprio_t *lef ;
} ;

struct mundo *cria_mundo(void) ;

void destroi_mundo(struct mundo *world) ;



#endif