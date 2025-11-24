#include <stdio.h>
#include <stdbool.h>
#include "mundo.h"
#include "entidade.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"



void evento_chega(struct mundo_t *world,int tempo, int heroi, int base) {

    if(!world)
        return ;

    //Ponteiro para heroi e base
    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;

    //Atualiza a base de heroi 
    h->base_atual = base ;

    //
    int tamanho_fila_espera = fila_tamanho(b->espera) ;
    int presentes = cjto_card(b->presentes) ;
    int vagas = (b->lotacao - presentes) ;

    bool espera ;
    if(vagas > 0 && tamanho_fila_espera == 0) //Há vagas e a fila está vazia
        espera = true ;
    else    
        espera = (h->paciencia) > (10*tamanho_fila_espera) ;

    struct evento_t *evento = malloc(sizeof(struct evento_t)) ;
        evento->tempo = tempo ;
        evento->heroi = heroi ;
        evento->base = base ;


    if(espera) {
        fprio_insere(lef, )


}
