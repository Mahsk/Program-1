#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"

struct evento_t *cria_evento(struct fprio_t *lef, int tempo, int heroi, int base) {

    


}

struct evento_t *cria_e_insere_lef(struct mundo_t *world, struct fprio_t *lef, int tempo, int tipo, int heroi, int base) {
    struct evento_t *novo_evento = malloc(sizeof(struct evento_t))
    if(!world || !lef)
        return ;