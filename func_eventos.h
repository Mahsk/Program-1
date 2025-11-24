#ifndef  FUNC_EVENTOS
#define FUNC_EVENTOS

struct evento_t *cria_evento(struct fprio_t *lef, int tempo, int heroi, int base) ;

struct evento_t *cria_e_insere_lef(struct mundo_t *world, struct fprio_t *lef) ;


#endif