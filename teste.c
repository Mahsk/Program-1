#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "fprio.h"
#include "eventos.h"

int main() {
    struct mundo_t *w = malloc(sizeof(struct mundo_t));

    w->NHerois = 3;
    w->NBases = 2;
    w->NMissoes = 2;

    w->heroi = calloc(w->NHerois, sizeof(struct heroi_t));
    w->bases = calloc(w->NBases, sizeof(struct bases_t));
    w->missoes = calloc(w->NMissoes, sizeof(struct missoes_t));

    struct fprio_t *lef = fprio_cria();

    iniciar_evento(lef, w);

    printf("\nEventos inseridos na LEF: %d\n", fprio_tamanho(lef));
    printf("Esperado: %d\n", w->NHerois + w->NMissoes + 1);

    printf("\nLEF = ");
    fprio_imprime(lef);
    printf("\n");

    return 0;
}
