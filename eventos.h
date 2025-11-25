#ifndef EVENTOS
#define EVENTOS

#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define VIAJA 5 
#define ENTRA 6
#define SAI 7
#define MISSAO 4
#define FIM 5

struct evento_t {
    int base ;
    int tempo ;
    int tipo ;
    int heroi ;
} ;

void iniciar_evento(struct fprio_t *lef, struct mundo_t *world) ;
void evento_chega(struct mundo_t *world,struct fprio_t *lef, int tempo, int heroi, int base) ;
void evento_espera(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

/*4.3 Evento DESISTE
O her´oi H desiste de entrar na base B, escolhe uma base aleat´oria D e viaja
para l´a:
DESISTE (T, H, B):
escolhe uma base destino D aleat´oria
cria e insere na LEF o evento VIAJA (agora, H, D)*/
void evento_desiste(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base ) ;

void evento_avisa(struct mundo_t *world, struct fprio_t *lef, int tempo, int base ) ;

/*O her´oi H entra na base B. Ao entrar, o her´oi decide quanto tempo vai ficar e
agenda sua sa´ıda da base:
ENTRA (T, H, B):
calcula TPB = tempo de perman^encia na base:
TPB = 15 + paci^encia de H * aleat´orio [1...20]
cria e insere na LEF o evento SAI (agora + TPB, H, B)*/
void evento_entra(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;


#endif