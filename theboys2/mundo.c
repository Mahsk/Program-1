#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mundo.h"
#include "entidade.h"
#include "fila.h"
#include "fprio.h"
#include "func_eventos.h"
#include "eventos.h"

//Criar mundo 

struct mundo_t *cria_mundo() {

    struct mundo_t *world = malloc(sizeof(struct mundo_t)) ;
   
    if(!world) //se nao conseguir alocar o ponteiro 
        return NULL ;

    //Inicializa os dados globais 
    world->NHerois = N_HEROIS ;
    world->NBases = N_BASES ;
    world->NMissoes = T_FIM_DO_MUNDO / 100 ;
    world->NHabilidades = N_HABILIDADES ;
    world->NCompostosV = N_COMPOSTOS_V ;
    world->TamanhoMundo = N_TAMANHO_MUNDO ;
    world->relogio = 0 ;
    world->missoes_cumpridas = 0 ;

    //Alocar os heróis 
    world->herois = malloc(sizeof(struct herois_t) * world->NHerois) ; //espaço para a estrutura do heroi para a quantidade de herois
    
    if(!world->herois) {
        free(world->herois) ;
        free(world) ;
        return NULL ;
    }
    
    //Inicializa os herois  
    for(int i = 0; i < world->NHerois; i++) {
        world->herois[i].id = i ;
        world->herois[i].experiencia = 0 ;
        world->herois[i].paciencia = aleat(1,100) ; 
        world->herois[i].velocidade = aleat(50,5000) ;
        world->herois[i].vivo = true ;
        world->herois[i].base_atual = -1;
        world->herois[i].missao_atual = -1 ;

        //Cria habilidades aleatórias 
        int qntdd_habilidades = aleat(1,3) ;
        world->herois[i].habilidades = cjto_aleat(qntdd_habilidades, world->NHabilidades) ;

        if(!world->herois[i].habilidades) {
            free(world->herois) ; //libera os herois
            free(world) ; 
            return NULL ;
        }

    }

    //Aloca as bases
    world->bases = malloc(sizeof(struct bases_t) * world->NBases) ;

    if(!world->bases) {
        free(world->bases) ;
        free(world) ;
        return NULL ;
    }

    for(int i = 0; i < world->NBases; i++) {
        world->bases[i].id = i ;
        world->bases[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        world->bases[i].local.y = aleat(0, N_TAMANHO_MUNDO -1 ) ;
        world->bases[i].lotacao = aleat(3,10) ;
        world->bases[i].presentes = cjto_cria(world->NHerois) ;
        world->bases[i].espera = fila_cria() ;
        world->bases[i].habilidades = cjto_cria(world->NHabilidades) ;

    }

    //Aloca as missoes
    world->missoes = malloc(sizeof(struct missoes_t) * world->NMissoes) ; 

    if(!world->missoes) {
        free(world->missoes) ;
        free(world) ;
        return NULL ;
    }
    
    //Inicialização de cada missao
    for (int i = 0; i < world->NMissoes; i++) {
        world->missoes[i].id = i ;
        world->missoes[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1) ;
        world->missoes[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1) ;

        int cjto_habilidades = aleat(6,10) ;
        world->missoes[i].habilidades = cjto_aleat(cjto_habilidades, world->NHabilidades) ;
    
    } 
    return world ;
}
    //Cria e insere na lef os eventos iniciais 
    void iniciar_evento(struct fprio_t *lef, struct mundo_t *world) {
        

        //Inicia evento para heroi 
        for (int i = 0; i < world->NHerois; i++) {
            struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
                evento->heroi = i ;
                evento->base = aleat(0, world->NBases -1) ;
                world->herois[i].base_atual = evento->base ;
                evento->tempo = aleat(0,4320) ;
                evento->tipo = CHEGA ;
                fprio_insere(lef, evento, CHEGA, evento->tempo) ;

                
        }

        //Inicia evento para missao
        for(int i = 0; i < world->NMissoes; i++) {
            struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
                evento->heroi = -1 ;
                evento->base = -1 ;
                evento->tempo = aleat(0, T_FIM_DO_MUNDO) ;
                evento->tipo = MISSAO ;
                fprio_insere(lef, evento, MISSAO, evento->tempo) ;
        }

        //Evento FIM 
        struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
            evento->tempo = T_FIM_DO_MUNDO ;
            evento->tipo = FIM ;
            fprio_insere(lef, evento, FIM, evento->tempo) ;
    }
        //Destroi mundo
        void destroi_mundo(struct mundo_t *world) {
            
            //Destroi herois
            if(world->herois) {
                for(int i = 0; i < world->NHerois; i++) {
                    if(world->herois[i].habilidades)
                        cjto_destroi(world->herois[i].habilidades) ;
                }
                free(world->herois) ;
            }

            //Destroi bases
            if(world->bases) {
                for(int i = 0; i < world->NBases; i++) {
                    if(world->bases[i].presentes)
                        cjto_destroi(world->bases[i].presentes) ;
                    if(world->bases[i].espera) 
                        fila_destroi(world->bases[i].espera) ;
                    if(world->bases[i].habilidades)
                        cjto_destroi(world->bases[i].habilidades) ;
                }
                free(world->bases) ;
            }

            //Destroi missões
            if(world->missoes) {
                for(int i = 0; i < world->NMissoes; i++) {
                    if(world->missoes[i].habilidades)
                        cjto_destroi(world->missoes[i].habilidades) ;
                }
                free(world->missoes) ;
            }
            free(world) ;
        }   
        





