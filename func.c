#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "fprio.h"

int aleat(int min, int max) {
    return min + rand() % (max - min + 1) ;
}


