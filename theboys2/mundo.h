#ifndef MUNDO 
#define MUNDO 
#include "conjunto.h"
#include "entidade.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_COMPOSTOS_V N_HABILIDADES * 3


struct mundo_t *cria_mundo() ;

void destroi_mundo(struct mundo_t *world) ;


#endif