#ifndef EVENTOS
#define EVENTOS

#include "fprio.h"


struct mundo ;

#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4 
#define VIAJA 5
#define ENTRA 6
#define SAI 7
#define MORRE 8
#define MISSAO 9
#define FIM 10



struct evento {
    int base ;
    int tempo ;
    int tipo  ;
    int heroi ;
    int destino ;
    int missao ;
};

void iniciar_evento(struct fprio_t *lef, struct mundo *world) ;

void evento_chega (struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_espera (struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_desiste(struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_avisa(struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_entra(struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_sai(struct fprio_t *lef, struct mundo *world, struct evento *ev ) ;

void evento_viaja(struct fprio_t *lef, struct mundo *world, struct evento *ev) ;

void evento_morre(struct fprio_t *lef, struct mundo *world, struct evento *ev ) ;

void evento_missao(struct fprio_t *lef, struct mundo *world, struct evento *ev) ;




#endif