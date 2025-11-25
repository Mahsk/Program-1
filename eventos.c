#include <stdio.h>
#include <stdbool.h>
#include "mundo.h"
#include "func.h"
#include "entidade.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"
#include "func_eventos.h"


void evento_chega(struct mundo_t *world,struct fprio_t *lef, int tempo, int heroi, int base) {

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

    int espera ;
    if(vagas > 0 && tamanho_fila_espera == 0) //Há vagas e a fila está vazia
        espera = 1 ;
    else    
        espera = (h->paciencia) > (10*tamanho_fila_espera) ;

    /*se espera:
    cria e insere na LEF o evento ESPERA (agora, H, B)
    senao:
    cria e insere na LEF o evento DESISTE (agora, H, B)*/
    //Decide o próximo evento 
    if(espera == 1) {
        insere_lef(lef,tempo,ESPERA,heroi,base) ;
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, heroi, base, presentes, b->lotacao) ;
    }
    else {
        insere_lef(lef,tempo, DESISTE,heroi,base) ;
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, heroi, base, presentes,b->lotacao)  ;
    }
}

/*O her´oi H entra na fila de espera da base B. Assim que H entrar na fila, o
porteiro da base B deve ser avisado para verificar a fila:
ESPERA (T, H, B):
5
adiciona H ao fim da fila de espera de B
cria e insere na LEF o evento AVISA (agora, B)
*/
void evento_espera(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct bases_t *b = &world->bases[base] ;

    //heroi entra na fila de espera
    fila_insere(b->espera, heroi );

    //Cria o evento avisa para o porteiro 
    insere_lef(lef,tempo, AVISA,-1, base ); //-1 no lugar de heroi, pq só a base é utilizada
}

/*4.3 Evento DESISTE
O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
para l´a:
DESISTE (T, H, B):
escolhe uma base destino D aleat´oria
cria e insere na LEF o evento VIAJA (agora, H, D)*/
void evento_desiste(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base ) {

    int d = aleat(0, world->NBases -1) ; //escolhe uma base destino aleatoria 

    insere_lef(lef, tempo, VIAJA, heroi, d) ; 

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base) ;
}



/*Evento AVISA
O porteiro da base B trata a fila de espera:
AVISA (T, B):*/
void evento_avisa(struct mundo_t *world, struct fprio_t *lef, int tempo, int base ) {
    
    struct bases_t *b = &world->bases[base] ;
 

    while((b->lotacao - cjto_card(b->presentes)) > 0 && fila_tamanho(b->espera) > 0) {
        
        //retira o primeiro heroi da fila 
        int prim_fila ; 
        fila_retira(b->espera,&prim_fila) ;

        //adiciona o heroi na base
        cjto_insere(b->presentes,prim_fila) ;

        //insere o evento ENTRA (agora, H’, B)
        insere_lef(lef,tempo,ENTRA,prim_fila,base) ;

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n",tempo, base, prim_fila) ;
    }

}

/*O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
agenda sua sa´ıda da base:
ENTRA (T, H, B):
calcula TPB = tempo de perman^encia na base:
TPB = 15 + paci^encia de H * aleat´orio [1...20]
cria e insere na LEF o evento SAI (agora + TPB, H, B)*/

void evento_entra(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {
     
    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;


    //Calculo do tempo de permanencia na base
    int TPB = 15 + ((h->paciencia) * aleat(1,20));

    int tempo_de_saida = tempo + TPB;
    int presentes = cjto_card(b->presentes) ;

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d", tempo, heroi, base,presentes,b->lotacao, tempo_de_saida);

    //cria e insere na LEF o evento SAI (agora + TPB, H, B)
    insere_lef(lef, tempo_de_saida, SAI, heroi, base) ;
}

/*O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
porteiro de B ´e avisado, pois uma vaga foi liberada:
SAI (T, H, B):
retira H do conjunto de her´ois presentes em B
escolhe uma base destino D aleat´oria
cria e insere na LEF o evento VIAJA (agora, H, D)
cria e insere na LEF o evento AVISA (agora, B)*/
void evento_sai(s)





