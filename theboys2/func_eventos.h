#ifndef  FUNC_EVENTOS
#define FUNC_EVENTOS

struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base) ;

struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base) ;

int distancia_cart_BM(struct coordenadas_t base, struct coordenadas_t missao) ;

struct cjto_t *uniao_habilidades(struct mundo_t *world, struct bases_t *bases) ;

int base_apta(struct mundo_t *world,struct bases_t *bases, struct missoes_t *missoes) ;

int heroi_experiente(struct mundo_t *world, int BMP) ;

int incrementa_experiencia(struct mundo_t *world, int BMP) ;


#endif