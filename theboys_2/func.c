#include <stdio.h>
#include <stdlib.h>
#include "func.h"


int aleat(int min, int max) {
    return min + rand() % (max - min + 1) ;
}