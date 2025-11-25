#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"

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
    
    
