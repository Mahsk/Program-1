#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "mundo.h"
#include "evento.h"  
#include "fprio.h"

int main() {
    srand(time(NULL)); // semente para aleatoriedade

    struct mundo *world = cria_mundo();
    struct fprio_t *lef = fprio_cria();

    if (!world || !lef) {
        return 1;
    }

    iniciar_evento(lef, world); // insere eventos iniciais na LEF

    int tipo, prio;

    while (fprio_tamanho(lef) > 0) {
        struct evento *ev = fprio_retira(lef, &tipo, &prio);
        if (!ev) continue;

        world->relogio = ev->tempo;

        switch (tipo) {
            case CHEGA:
                evento_chega(lef, world, ev);
                break;
            case ESPERA:
                evento_espera(lef, world, ev);
                break;
            case DESISTE :
                evento_desiste(lef, world, ev) ;
                break ;
            case AVISA :
                evento_avisa(lef, world, ev) ;
                break ; 
            case ENTRA :
                evento_entra(lef, world, ev) ;
                break;
            case SAI:
                evento_sai(lef, world, ev) ;
                break;
            case VIAJA :
                evento_viaja(lef, world, ev) ;
                break ;
            case MORRE:
                evento_morre(lef, world, ev) ;
                break ;
            case MISSAO:
                evento_missao(lef, world, ev) ;
                break;
            case FIM:
                break;
            // outros eventos
        }

        free(ev); // libera memória do evento processado
    }

    fprio_destroi(lef);     // libera a fila e os nós
    destroi_mundo(world);   // libera toda memória associada ao mundo

    return 0;
}
