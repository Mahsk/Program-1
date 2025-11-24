#ifndef EVENTOS
#define EVENTOS

#define CHEGA 1
#define MISSAO 2
#define ESPERA 3
#define DESISTE 4 
#define FIM 5

struct evento_t {
    int base ;
    int tempo ;
    int tipo ;
    int heroi ;
} ;

void iniciar_evento(struct fprio_t *lef, struct mundo_t *world) ;
void evento_chega(struct mundo_t *world,int tempo, int heroi, int base) ;


#endif