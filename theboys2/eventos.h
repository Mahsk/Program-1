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

/*O her´oi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
porteiro de B ´e avisado, pois uma vaga foi liberada:
SAI (T, H, B):
retira H do conjunto de her´ois presentes em B
escolhe uma base destino D aleat´oria
cria e insere na LEF o evento VIAJA (agora, H, D)
cria e insere na LEF o evento AVISA (agora, B)*/
void evento_sai(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

/*O her´oi H se desloca para uma base D (que pode ser a mesma onde j´a est´a):
VIAJA (T, H, D):
calcula dura¸c~ao da viagem:
dist^ancia = dist^ancia cartesiana entre a base atual de H e a base D
dura¸c~ao = dist^ancia / velocidade de H
cria e insere na LEF o evento CHEGA (agora + dura¸c~ao, H, D)*/
void evento_viaja(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int d) ;

void evento_morre(struct mundo_t *world, struct fprio_t *lef, int tempo, int heroi, int base) ;

void evento_missao(struct mundo_t *world, /* struct fprio_t *lef,  */int tempo, int missao) ;


#endif