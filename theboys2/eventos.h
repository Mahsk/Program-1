#ifndef EVENTOS
#define EVENTOS

#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define VIAJA 5 
#define ENTRA 6
#define SAI 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

struct fprio_t;
struct mundo_t;

struct evento_t {
    int base ;
    int tempo ;
    int tipo ;
    int heroi ;
    int missao ;
} ;

//Inicia o evento em Mundo
void iniciar_evento(struct fprio_t *lef, struct mundo_t *world) ;

/*Representa um heroi chegando em uma base em um tempo. Ao chegar, o
heroi analisa o tamanho da fila e decide se espera para entrar ou desiste*/
void evento_chega(struct mundo_t *world,struct fprio_t *lef, int tempo, int heroi, int base) ;

/*O heroi entra na fila de espera da base B. Assim que H entrar na fila, o
porteiro da base B deve ser avisado para verificar a fila*/
void evento_espera(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

/*O heroi desiste de entrar na base B, escolhe uma base aleatoria D e viaja
para la*/
void evento_desiste(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base ) ;

//O porteiro da base B trata a fila de espera
void evento_avisa(struct mundo_t *world, struct fprio_t *lef, int tempo, int base ) ;

/*O heroi entra na base B. Ao entrar, ele decide quanto tempo vai ficar e
agenda sua saida da base*/
void evento_entra(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

/*O heroi sai da base B. Ao sair, escolhe uma base de destino para viajar; o
porteiro de B eh avisado, pois uma vaga foi liberada*/
void evento_sai(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

//O heroi se desloca para uma base D (que pode ser a mesma onde ja esta)
void evento_viaja(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int d) ;

/*O heroi morre no instante T, liberando uma vaga na base.O porteiro de B deve ser avisado da nova vaga.
Eventos futuros de um heroi morto sao ignorados.*/
void evento_morre(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

/*Verifica aptidao da base. Se ela for apta e mais proxima, a missao é cumprida. Caso contrario,  verifica
a quantidade de compostos V e o tempo multiplo de 2500. Se for, o heroi mais experiente, usa o composto V
e morre. Se o tempo nao for multiplo ou nao tiver compostos v, a missao é dada como impossivel*/
void evento_missao(struct mundo_t *world, struct fprio_t *lef,  int tempo, int missao) ;

//Encerra a simulacao e dá as estatísticas
void evento_fim(struct mundo_t *world,int tempo) ;



#endif