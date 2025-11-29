#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entidade.h"
#include "fprio.h"
#include "func.h"
#include "eventos.h"


int aleat(int min, int max) {
    //return min + rand() % (max - min + 1) ;
    return (rand()%(max - min + 1)) + min;
}

int distancia_cartesiana(struct mundo_t *world,int origem, int destino) {

    int x1 = world->bases[origem].local.x ;
    int x2 = world->bases[destino].local.x ;
    int y1 = world->bases[origem].local.y ;
    int y2 = world->bases[destino].local.y ;

    int dx = x2 -x1  ;
    int dy = y2 - y1 ;
    double dc = (double)(dx * dx) + (double)(dy * dy) ;

    return (int)(sqrt(dc)) ;
} 



