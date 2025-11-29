
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mundo.h"
#include "entidade.h"
#include "eventos.h"
#include "fprio.h"

int main() {
    srand(0);
    
    struct mundo_t *world = cria_mundo();
    struct fprio_t *lef = fprio_cria();

    if (!world || !lef) {
        return 1;
    }

    iniciar_evento(lef, world);

    printf("\n");
    fprio_imprime(lef);
    printf("\n");

    /*printf("DEBUG: T_FIM_DO_MUNDO = %d\n", T_FIM_DO_MUNDO);
    printf("DEBUG: world->relogio inicial = %d\n", world->relogio);*/

    while (world->relogio < T_FIM_DO_MUNDO) {
        int tipo, prio;
        struct evento_t *evento = fprio_retira(lef, &tipo, &prio);

        if (!evento) {
           // printf("DEBUG: Evento NULL, saindo...\n");
            break;
        }

        //printf("DEBUG: Evento %d - tipo=%d tempo=%d, relogio antes=%d\n", 
               //evento->base, evento->tipo, evento->tempo, world->relogio);
        
       // printf("PROCESSANDO: tipo=%d tempo=%d heroi=%d base=%d\n",
               //evento->tipo, evento->tempo, evento->heroi, evento->base);
        
        world->relogio = evento->tempo;
        
        //printf("DEBUG: relogio depois=%d, T_FIM_DO_MUNDO=%d\n", 
               //world->relogio, T_FIM_DO_MUNDO);

        switch (evento->tipo) {
            case CHEGA:
                evento_chega(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case ESPERA:
                evento_espera(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case DESISTE:
                evento_desiste(world, lef, evento->tempo, evento->heroi, evento->base);
                break;
            case AVISA:
                evento_avisa(world, lef, evento->tempo,  evento->base);
                break;
            case ENTRA:
                evento_entra(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;
            case SAI:
                evento_sai(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;
            case VIAJA:
                evento_viaja(world, lef, evento->tempo, evento->heroi,  evento->base);
                 break; 
            case MISSAO:
                evento_missao(world, /*  lef, */ evento->tempo, evento->missao);
                break; 
            case MORRE:
                evento_morre(world, lef, evento->tempo, evento->heroi,  evento->base);
                break;   
   /*          case FIM:
                printf("DEBUG: Evento FIM encontrado\n");
                break;
            default:
                printf("DEBUG: Tipo de evento desconhecido: %d\n", evento->tipo); */ 
        }
        
        free(evento);
    }
    
    destroi_mundo(world);
    fprio_destroi(lef);
    return 0;
}