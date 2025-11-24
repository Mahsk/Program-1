#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "fprio.h"

int aleat(int min, int max) {
    return min + rand() % (max - min + 1) ;
}

struct evento_t cria_e_insere_lef(struct mundo_t *world, struct fprio_t *lef,  )