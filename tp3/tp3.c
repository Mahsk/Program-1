#include <stdio.h>
#include <stdlib.h>
#include "racional.h"
#define MAX 100
/* coloque aqui as funções auxiliares que precisar neste arquivo */

void troca(struct racional **a, struct racional **b) {
  struct racional *temp = *a ;
    *a = *b ;
    *b = temp ;

}

/*Ordenação seguindo Selection Sort*/
void ordena_racionais(struct racional *v[], int tam) {
  int i, j, menor ;

  for (i=0; i<tam-1; i++) { //loop externo que irá considerar o primeiro como menor
    menor = i ;

    for (j = i+1; j<tam; j++ ) {

      if (compara_r(v[menor], v[j]) == 1) //loop interno verificará se os próximos são menores que o primeiro, se for, irá realizar a troca

        menor = j ;
    }
      if (menor != i) {
        troca(&v[i], &v[menor]) ;
    }
  }
}

/* programa principal */
int main () {
  int n, i ;
  struct racional **vetor ;

  scanf("%d", &n) ;

  if (n > 100)
    return 0 ;
  
    vetor = malloc(n * sizeof (struct racional*)) ;
    if (!vetor)
      return -1 ;
    
    for (i = 0; i < n; i++) {
      long num, den ; //num e den são recriadas a cada loop, então os valores não são sobrescritos
      struct racional *r ;

      scanf("%ld %ld", &num, &den) ; //lê o numerador e o denominador 
      r = cria_r(num, den) ;
      vetor[i]= r ;

    }
  return (0) ;
}

