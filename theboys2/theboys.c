
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mundo.h"
#include "entidade.h"
#include "eventos.h"
#include "fprio.h"
#include "func_aux.h"

int main() {

    srand(0);

    struct mundo_t *world ;
    struct fprio_t *lef ;
    
    if (!world || !lef) 
        return 1;
    
    //Cria o mundo 
    cria_mundo_MAIN(&world,&lef) ;

    //Inicia o mundo
    inicia_mundo_MAIN(world,lef) ;

    //Destroi o mundo
    destroi_mundo_MAIN(world,lef) ;

    return 0;
}
