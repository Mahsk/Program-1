#ifndef EVENTOS
#define EVENTOS

#define CHEGA 1
#define MISSAO 2
#define FIM 3 

struct evento_t {
    int base ;
    int tempo ;
    int tipo ;
} ;

void iniciar_evento(struct fprio_t *lef, struct mundo_t *world) ;


#endif