#ifndef  FUNC_EVENTOS
#define FUNC_EVENTOS

struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base) ;

struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base) ;


#endif