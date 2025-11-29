#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
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

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, heroi, base, fila_tamanho(b->espera)) ;
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

    printf("imprime as herois antes de  entraram na base %d ",base);
    cjto_imprime(b->presentes) ;
    printf("\n") ;

    //insere o heroi na base
    cjto_insere(b->presentes,heroi) ;

    printf("imprime as herois depois de  entraram na base %d ",base);
    cjto_imprime(b->presentes) ;
    printf("\n") ;


    int presentes = cjto_card(b->presentes) ;

    //atualiza a habilidades do heroi que entrou na base
    uniao_habilidades(world,b) ;
/*     printf("imprime as habilidades dos herois que entraram na base %d ",base);
    cjto_imprime(b->habilidades) ; */

    //Calculo do tempo de permanencia na base
    int TPB = 15 + ((h->paciencia) * aleat(1,20));
    int tempo_de_saida = tempo + TPB;

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, heroi,base, presentes,b->lotacao, tempo_de_saida);

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
void evento_sai(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct bases_t *b = &world->bases[base] ;

    //retira heroi do conjunto presente na base
    cjto_retira(b->presentes, heroi) ; 
    int presentes = cjto_card(b->presentes) ;

    //atualiza as habilidades
    uniao_habilidades(world,b) ;
/*     printf("ver se removeu as habilidades da base %d ", base) ;
    cjto_imprime(b->habilidades) ; */

    //escolhe uma base destino aleatoria 
    int d = aleat(0, world->NBases -1) ; 

/*     int presentes = cjto_card(b->presentes);
 */
    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, heroi, base, presentes, b->lotacao);

    //heroi ao sair da base escolhe uma base destino para viajar
    insere_lef(lef, tempo, VIAJA, heroi, d) ;

    //o porteiro é avisado 
    insere_lef(lef, tempo, AVISA, -1, base) ;
}

/*O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a est´a):
VIAJA (T, H, D):
calcula dura¸c~ao da viagem:
dist^ancia = dist^ancia cartesiana entre a base atual de H e a base D
dura¸c~ao = dist^ancia / velocidade de H
cria e insere na LEF o evento CHEGA (agora + dura¸c~ao, H, D)*/
void evento_viaja(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int d) {

    struct herois_t *h = &world->herois[heroi] ;
/*     struct bases_t *b = &world->bases[h->base_atual] ;
 */    struct bases_t *dest = &world->bases[d] ;

    int base = h->base_atual ;
    int destn = dest->id ; 

    //Calcula a duracao da viagem 
    int distancia = distancia_cartesiana(world, base,destn) ; 
    int duracao = distancia / h->velocidade ;
    int tempo_chegada = tempo + duracao ;

    h->base_atual = destn ;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, heroi, base, destn, distancia, h->velocidade, tempo_chegada) ;

    insere_lef(lef,tempo_chegada, CHEGA, heroi, d) ;
}

/*O her´oi H morre no instante T.
• O her´oi ´e retirado da base B e libera uma vaga na base.
• O porteiro de B deve ser avisado da nova vaga.
• Eventos futuros de um her´oi morto devem ser ignorados.
MORRE (T, H, B):
retira H do conjunto de her´ois presentes em B
muda o status de H para morto
cria e insere na LEF o evento AVISA (agora, B) */
void evento_morre(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;
    int missao = h->missao_atual ;

    //retira heroi do conjunto de presentes da base
    cjto_retira(b->presentes, heroi) ;

    //muda o status de heroi para morto
    h->vivo = false ;

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, heroi, missao ) ;
    //cria o evento avisa
    insere_lef(lef, tempo, AVISA, -1, base) ;
}  

void evento_missao(struct mundo_t *world, /* struct fprio_t *lef, */ int tempo, int missao) {

    struct missoes_t *m = &world->missoes[missao] ;
    int menor_distancia = world->TamanhoMundo*2;
    int BMP = -1;
    int B_apta_MP = -1;
   
    for (int i = 0; i <  world-> NBases; i++) {

        //contador de tentativas
        int tenta = m->tentativas++ ;
        printf("%6d: MISSAO %d TENT %d HAB REQ: [", tempo, missao, tenta);
        cjto_imprime(m->habilidades) ;
        printf("]\n") ; 

        //calcula a distancia de cada base ao local da missao M
        int distancia = distancia_cart_BM(world->bases[i].local, world->missoes[missao].local) ;
        printf("verifica distancia %d\n",distancia) ;

         //verificar se base tem heróis
        printf("base %d herois presentes: %d\n", i, cjto_card(world->bases[i].presentes)); 
        
        int eh_apta = base_apta(world,&world->bases[i], &world->missoes[missao]) ;
        printf("a base É APTA: %d\n", eh_apta) ;

        if(eh_apta == 1)  {
            //verifica qual base é mais perto
            if (distancia < menor_distancia) {
                menor_distancia = distancia ;
                B_apta_MP = i ;
            }
        }
    }
        printf("menor distancia: %d\n", menor_distancia) ;
        printf("BMP: %d\n", BMP) ;
    
        int base_mais_proxima = BMP;
        printf ("base mais proxima:%d\n", base_mais_proxima);

        printf("base APTA mais proxima: %d\n", B_apta_MP) ;
    
    if (B_apta_MP != -1) {
        
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", tempo, missao, BMP) ;
        cjto_imprime(world->bases[B_apta_MP].presentes) ;
        printf(" ]\n") ;

        //marca a missao como cumprida
        printf("ver qtdd de missao cumprida ANTES: %d\n", world->missoes_cumpridas) ;
        world->missoes_cumpridas++ ;
        printf("ver qtdd de missao cumprida DEPOIS: %d\n", world->missoes_cumpridas) ;

/*         incrementa_experiencia(world, BMP) ;
 
 */
        //incrementa a experiencia dos herois presentes na base mais proxima
        world->herois[B_apta_MP].experiencia++ ; 
    } /* else {
        //se há compostos V e o tempo for multiplo de 2500
        if(world->NCompostosV != 0 && tempo % 2500 == 0) {
            //decrementa a quantidade de compostosV
            world->NCompostosV--;
            //marca a missao como cumprida
            world->missoes_cumpridas++;
            //chama o heroi mais experiente
            int h_experiente = heroi_experiente(world, BMP) ;
            //o heroi mais experiente morre apos o uso do Composto V
            insere_lef(lef, tempo, MORRE, h_experiente, -1) ;

            //incrementa a experiencia dos herois presentes
            incrementa_experiencia(world, BMP) ;
        }
        else {
            //se nao houver base apta e nem puder usar o compostoV, marca a missao como impossivel e adia 24 horas
            printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao) ;        
            insere_lef(lef, tempo + (24*60), MISSAO,missao, -1 );
        }  */
           /*  return ; */
/*     } */
}    
