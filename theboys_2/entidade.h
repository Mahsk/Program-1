#ifndef ENTIDADE
#define ENTIDADE

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
} ;

struct bases_t {
    int id ;
    int lotacao ;
    struct cjto_t *presentes ;
    struct fila_t *espera ;
    struct coordenadas_t local ;
} ;

struct missoes_t {
    int id ;
    struct cjto_t *habilidades ;
    struct coordenadas_t local ;
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
} ;



#endif