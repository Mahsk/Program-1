#include <stdlib.h>
#include "fila.h"
#include "mundo.h"


int main() {
    struct mundo_t *world = malloc(sizeof(struct mundo_t));

    world->NHerois = 3;
    world->NBases = 2;
    world->NMissoes = 2;
    world->NHabilidades = 10; // Defina um valor válido!

    // Criar heróis
    world->heroi = calloc(world->NHerois, sizeof(struct heroi_t));
    for (int i = 0; i < world->NHerois; i++) {
        world->heroi[i].habilidades = cjto_cria(world->NHabilidades);
    }

    // Criar bases
    world->bases = calloc(world->NBases, sizeof(struct bases_t));
    for (int i = 0; i < world->NBases; i++) {
        world->bases[i].presentes = cjto_cria(world->NHabilidades);
        world->bases[i].espera = fila_cria();
    }

    // Criar missões
    world->missoes = calloc(world->NMissoes, sizeof(struct missoes_t));
    for (int i = 0; i < world->NMissoes; i++) {
        world->missoes[i].habilidades = cjto_cria(world->NHabilidades);
    }

    destroi_mundo(world);
    return 0;
}
