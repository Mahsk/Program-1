/*#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"
#include "func_eventos.h"

int main() {

    // cria mundo
    struct mundo_t *world = cria_mundo();
    if (!world) {
        printf("Erro ao criar mundo\n");
        return 1;
    }

    // cria LEF
    struct fprio_t *lef = fprio_cria();
    if (!lef) {
        printf("Erro ao criar LEF\n");
        return 1;
    }

        int h = 0;
int b = 0;

// Força paciência baixa
world->herois[h].paciencia = 5;

// Força fila com 1 herói
fila_insere(world->bases[b].espera, 123);

// Chama CHEGA
evento_chega(world, lef, 20, h, b);

// Imprime conteúdo da LEF
struct fpnodo_t *n = lef->prim;
if (n) {
    struct evento_t *ev = n->item;
    printf("LEF recebeu evento: tipo=%d tempo=%d h=%d b=%d\n",
        ev->tipo, ev->tempo, ev->heroi, ev->base);
}

    // limpeza
    fprio_destroi(lef);

    destroi_mundo(world);
    return 0 ;
}*/

#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "fila.h"
#include "conjunto.h"
#include "fprio.h"
#include "eventos.h"
#include "func_eventos.h"

int main() {

    printf("\n=== TESTE DO EVENTO ENTRA ===\n");

    // cria mundo
    struct mundo_t *world = cria_mundo();
    struct fprio_t *lef = fprio_cria();

    // define herói e base
    int heroi = 1;
    int base = 5;
    int tempo = 50000;

    // coloca o herói como presente na base (como se o AVISA já tivesse admitido)
    cjto_insere(world->bases[base].presentes, heroi);

    // mostra antes
    printf("ANTES DO ENTRA:\n");
    printf("paciência do heroi = %d\n", world->herois[heroi].paciencia);
    printf("presentes = %d\n", cjto_card(world->bases[base].presentes));
    printf("lotação = %d\n\n", world->bases[base].lotacao);

    // chama o evento ENTRA
    evento_entra(world, lef, tempo, heroi, base);

    // imprime resultado na LEF
    printf("\nEVENTOS GERADOS NA LEF:\n");

    int tipo, prio;
    struct evento_t *ev;

    while ((ev = fprio_retira(lef, &tipo, &prio)) != NULL) {
        printf("SAI tempo=%d heroi=%d base=%d\n", ev->tempo, ev->heroi, ev->base);
        free(ev);
    }

    printf("=== FIM DO TESTE ===\n");

    free(world);
    return 0 ;
}



