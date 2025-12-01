#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "mundo.h"
#include "entidade.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"
#include "func_eventos.h"


void evento_chega(struct mundo_t *world,struct fprio_t *lef, int tempo, int heroi, int base) {

    //Ponteiro para heroi e base
    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;

    //Atualiza a base de heroi 
    h->base_atual = base ;

    int tamanho_fila_espera = fila_tamanho(b->espera) ;
    int presentes = cjto_card(b->presentes) ;
    int vagas = (b->lotacao - presentes) ;

    int espera = 0 ;
    //Se há vagas e a lista de espera está vazia, o heroi espera, caso contrario ver a paciencia
    if(vagas > 0 && tamanho_fila_espera == 0) 
        espera = 1 ;
    else    
        espera = (h->paciencia) > (10*tamanho_fila_espera) ;

    //Decide o próximo evento. Se espera, insere na lef ESPERA, caso contrário insere na lef DESISTE
    if(espera == 1) {
        insere_lef(lef,tempo,ESPERA,heroi,base,-1) ;
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, heroi, base, presentes, b->lotacao) ;
    }
    else {
        insere_lef(lef,tempo, DESISTE,heroi,base,-1) ;
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, heroi, base, presentes,b->lotacao)  ;
    }
}

void evento_espera(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct bases_t *b = &world->bases[base] ;

    //heroi entra na fila de espera
    fila_insere(b->espera, heroi );

    //Cria o evento avisa para o porteiro 
    insere_lef(lef,tempo, AVISA,-1, base,-1); //-1 no lugar de heroi e missao, pq só a base é utilizada

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, heroi, base, fila_tamanho(b->espera)) ;
}

void evento_desiste(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base ) {

    //Escolhe uma base destino aleatoria 
    int d = aleat(0, world->NBases -1) ; 

    //Cria e insere na lef VIAJA
    insere_lef(lef, tempo, VIAJA, heroi, d,-1) ; 

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base) ;
}

void evento_avisa(struct mundo_t *world, struct fprio_t *lef, int tempo, int base ) {
    
    struct bases_t *b = &world->bases[base] ;
    
    //Enquanto houver vaga e tiver herois esperando na fila
    while((b->lotacao - cjto_card(b->presentes)) > 0 && fila_tamanho(b->espera) > 0) {
        
        //Retira o primeiro heroi da fila 
        int prim_fila ; 
        int tamanho_max ;
        fila_retira(b->espera,&prim_fila) ;

        //adiciona o heroi na base
        cjto_insere(b->presentes,prim_fila) ;

        //Atualiza a base
        world->herois[prim_fila].base_atual = base ;

        //Ver o tamanho maximo da base
        tamanho_max = fila_tamanho(b->espera) ;
        if(tamanho_max > b->fila_max)
            b->fila_max =  tamanho_max ;

        //Insere o evento ENTRA (agora, H’, B)
        insere_lef(lef,tempo,ENTRA,prim_fila,base,-1) ;

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n",tempo, base, prim_fila) ;
    }

}

void evento_entra(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {
     
    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;

    //Ve os presentes da base
    int presentes = cjto_card(b->presentes) ;

    //Atualiza a habilidades do heroi que entrou na base
    uniao_habilidades(world,b) ;

    //Calculo do tempo de permanencia na base
    int TPB = 15 + ((h->paciencia) * aleat(1,20));
    int tempo_de_saida = tempo + TPB;

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, heroi,base, presentes,b->lotacao, tempo_de_saida);

    //cria e insere na LEF o evento SAI (agora + TPB, H, B)
    insere_lef(lef, tempo_de_saida, SAI, heroi, base,-1) ;
}

void evento_sai(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct bases_t *b = &world->bases[base] ;

    //Retira heroi do conjunto presente na base
    cjto_retira(b->presentes, heroi) ; 

    //Ve os presentes
    int presentes = cjto_card(b->presentes) ;

    //Atualiza as habilidades
    uniao_habilidades(world,b) ;

    //Escolhe uma base destino aleatoria 
    int d = aleat(0, world->NBases -1) ; 

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, heroi, base, presentes, b->lotacao);

    //Heroi ao sair da base escolhe uma base destino para viajar
    insere_lef(lef, tempo, VIAJA, heroi, d,-1) ;

    //O porteiro é avisado 
    insere_lef(lef, tempo, AVISA, -1, base,-1) ;
}

void evento_viaja(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int d) {

    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *dest = &world->bases[d] ;

    int base = h->base_atual ;
    int destn = dest->id ; 

    //Calcula a duracao da viagem 
    int distancia = distancia_cartesiana(world, base,destn) ; 
    int duracao = distancia / h->velocidade ;
    int tempo_chegada = tempo + duracao ;

    //Atualiza a base atual para ver a base apta, pq ela é usada antes de chega
    h->base_atual = destn ;
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, heroi, base, destn, distancia, h->velocidade, tempo_chegada) ;

    insere_lef(lef,tempo_chegada, CHEGA, heroi, d,-1) ;
}

void evento_morre(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) {

    struct herois_t *h = &world->herois[heroi] ;
    struct bases_t *b = &world->bases[base] ;
    int missao = h->missao_atual ;

    //Retira heroi do conjunto de presentes da base
    cjto_retira(b->presentes, heroi) ;

    //Muda o status de heroi para morto
    h->vivo = false ;

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, heroi, missao ) ;

    //cria o evento avisa
    insere_lef(lef, tempo, AVISA, -1, base,-1) ;
}  

void evento_missao(struct mundo_t *world, struct fprio_t *lef, int tempo, int missao) {

    struct missoes_t *m = &world->missoes[missao] ;
    int menor_distancia = world->TamanhoMundo*2;
    int menor_distancia_naoApta = world->TamanhoMundo*2 ;
    int BMP = -1;
    int B_apta_MP = -1;
    
    //Contador de tentativas
    int tenta = m->tentativas++ ;
    printf("%6d: MISSAO %d TENT %d HAB REQ: [", tempo, missao, tenta);
    cjto_imprime(m->habilidades) ;
    printf("]\n") ;  

    //Percorre as bases
    for (int i = 0; i <  world-> NBases; i++) {

        //Calcula a distancia de cada base ao local da missao M
        int distancia = distancia_cart_BM(world->bases[i].local, world->missoes[missao].local) ;

        int eh_apta = base_apta(world,&world->bases[i], &world->missoes[missao]) ;

        //Se a base for apta
        if(eh_apta == 1)  {

            //verifica qual base é mais perto e apta
            if (distancia < menor_distancia) {
                menor_distancia = distancia ;
                B_apta_MP = i ;
            }
        } else //Verifica a base nao apta com a distancia mais curta
                if ((distancia < menor_distancia_naoApta) && (cjto_card(world->bases[i].presentes))) {
                    menor_distancia_naoApta = distancia ;
                    BMP = i ;
                }
            }
    
    //Se a base for apta
    if (B_apta_MP != -1) {

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ",tempo, missao, B_apta_MP, menor_distancia) ;
        cjto_imprime(world->bases[B_apta_MP].presentes);
        printf(" ]\n") ;
        
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ",tempo, missao, B_apta_MP) ;
        cjto_imprime(world->bases[B_apta_MP].habilidades);
        printf(" ]\n") ;
        
        //Marca a missao como cumprida
        world->missoes_cumpridas++ ;
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ",tempo, missao, B_apta_MP) ;
        cjto_imprime(world->bases[B_apta_MP].habilidades) ;
        printf(" ]\n") ;

        //Conta as missoes das bases
        world->bases[B_apta_MP].contador_missoes++ ;

        //Percorre os herois
        for(int h = 0; h < world->NHerois; h++){
            //incrementa a experiencia dos herois presentes na base mais proxima
            incrementa_experiencia(world, B_apta_MP) ;

            printf("%6d: MISSAO %d HAB HEROI %2d: [  ",tempo, missao, h) ;
            cjto_imprime(world->herois[h].habilidades) ;
            printf(" ]\n") ;
        }
    }else {

        //Se há compostos V e o tempo for multiplo de 2500
        if(world->NCompostosV > 0 && tempo % 2500 == 0 && BMP != -1) {

            //Decrementa a quantidade de compostosV
            world->NCompostosV--;

            //Marca a missao como cumprida
            world->missoes_cumpridas++;
            
            //Chama o heroi mais experiente
            int h_experiente = heroi_experiente(world, BMP) ;

            //O heroi mais experiente morre apos o uso do Composto V
            insere_lef(lef, tempo, MORRE, h_experiente, BMP,-1) ;

            //Incrementa a experiencia dos outros herois presentes
            incrementa_experiencia(world, BMP) ;
        } else { 
            //Se nao houver base apta e nem puder usar o compostoV, marca a missao como impossivel e adia 24 horas
            printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao) ;        
            insere_lef(lef, tempo + (24*60), MISSAO,-1,-1,missao); 
        }
    }
}

void evento_fim(struct mundo_t *world,int tempo) {

    printf("%6d: FIM\n", tempo) ;

    int cont_vivo = 0 ;
    //percorre os herois
    for(int h = 0; h < world->NHerois; h++){
        if(world->herois[h].vivo == true) {
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ ", h, world->herois[h].paciencia, world->herois[h].velocidade, world->herois[h].experiencia) ;
            cjto_imprime(world->herois[h].habilidades) ;
            printf(" ]\n") ;
            cont_vivo++ ;
        }else {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [", h, world->herois[h].paciencia, world->herois[h].velocidade, world->herois[h].experiencia) ;
            cjto_imprime(world->herois[h].habilidades) ;
            printf(" ]\n") ;
        }
    }

    //Percorre as bases
    for(int i = 0; i < world->NBases; i++) {

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", world->bases[i].id, world->bases[i].lotacao,world->bases[i].fila_max, world->bases[i].contador_missoes) ;
    } 

    //Calcula os eventos tratados
    printf("EVENTOS TRATADOS: %d\n",world->eventos_tratados) ;

    int missao_cumprida = world->missoes_cumpridas ;
    int total_missao = world->NMissoes ;
    float media_missoes = ((float)missao_cumprida/total_missao) * 100 ;

    //Missoes cumpridas
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missao_cumprida, total_missao, media_missoes) ;
    
    int minimo_tent = minimo_tentativas(world) ;
    int maximo_tent = maximo_tentativas(world) ;
    int total_tent = total_tentativas(world);
    printf("total_missao: %d\n", total_missao) ;
    printf("total de tentativas: %d\n", total_tent) ;
    float media_tentativas = (float)total_tent/ total_missao;

    //Tentativas de tentativas 
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", minimo_tent, maximo_tent,media_tentativas) ;

    int total_de_heroi = world->NHerois ;
    int herois_mortos = (total_de_heroi - cont_vivo) ;
    float media_mortalidade = ((float)herois_mortos/total_de_heroi)*100 ;

    //Calcula taxa de mortalidade
    printf("TAXA MORTALIDADE: %.1f%%\n", media_mortalidade) ;
}
     