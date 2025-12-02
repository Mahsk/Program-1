#ifndef  FUNC_EVENTOS
#define FUNC_EVENTOS

//Faz a função aleatoria
int aleat(int min, int max) ;

//Cria o mundo na main
void cria_mundo_MAIN(struct mundo_t **world, struct fprio_t **lef) ;

//Inicia o mundo na main
void inicia_mundo_MAIN(struct mundo_t *world, struct fprio_t *lef) ;

//Destroi o mundo na main
void destroi_mundo_MAIN(struct mundo_t *world, struct fprio_t *lef) ;

//Cria a estrutura do novo evento
struct evento_t *cria_evento(int tempo, int tipo, int heroi, int base,int missao) ;

//Insere os eventos na lef
struct evento_t *insere_lef(struct fprio_t *lef, int tempo, int tipo, int heroi, int base,int missao) ;

//Calculo da distancia cartesiana em VIAGEM
int distancia_cartesiana(struct mundo_t *world,int origem, int destino) ;

//Calculo da distancia cartesiana Base - Missao
int distancia_cart_BM(struct coordenadas_t base, struct coordenadas_t missao) ;

//Une as habilidades dos herois presentes na base
void uniao_habilidades(struct mundo_t *world, struct bases_t *bases) ;

//Verifica se o conjunto de habilidades do heroi contem as habilidades da missao 
int base_apta(struct mundo_t *world,struct bases_t *bases, struct missoes_t *missoes) ;

//Ve o heroi mais experiente da base NAO apta 
int heroi_experiente(struct mundo_t *world, int BMP) ;

//Incrementa a experiencia do herois presentes na base
int incrementa_experiencia(struct mundo_t *world, int BMP) ;

//Informa o minimo de tentativas para colocar na estatistica da função FIM
int minimo_tentativas(struct mundo_t *world) ;

//Informa o máximo de tentativas para colocar na estatistica da função FIM
int maximo_tentativas(struct mundo_t *world) ;

//Informa o total de tentativas para colocar na estatistica da função FIM
int total_tentativas(struct mundo_t *world) ;




#endif