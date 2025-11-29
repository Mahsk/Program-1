#ifndef ENTIDADE
#define ENTIDADE

#include <stdbool.h>

struct coordenadas_t {
    int x ;
    int y ;
} ;


struct herois_t {
    int id ;
    struct cjto_t *habilidades ; 
    int paciencia ;
    int velocidade ;
    int experiencia ;
    int base_atual ;
    int missao_atual ;
    bool vivo ;
} ;

struct bases_t {
    int id ;
    int lotacao ;
    struct cjto_t *presentes ;
    struct fila_t *espera ;
    struct coordenadas_t local ;
    struct cjto_t *habilidades ;
} ;

struct missoes_t {
    int id ;
    struct cjto_t *habilidades ;
    struct coordenadas_t local ;
    int tentativas ;
} ;

struct mundo_t {
    int NHerois ;
    struct herois_t *herois;
    int NBases ;
    struct bases_t *bases ;
    int NMissoes ;
    struct missoes_t *missoes ;
    int NHabilidades;
    int NCompostosV ;
    int TamanhoMundo ;
    int relogio ;
    int missoes_cumpridas ;
    int composto_V ;
} ;



#endif