#include <stdio.h>
#include <stdlib.h>
#include "racional.h"
#define MAX 100

/*Realiza a troca entre os racionais no ordena_racionais*/
void troca(struct racional *a, struct racional *b) {
  struct racional temp = *a ;
    *a = *b ;
    *b = temp ;

}

/*Ordenação seguindo Selection Sort*/
void ordena_racionais(struct racional v[], int n) {
  int i, j, menor ;

  for (i=0; i<n-1; i++) { //loop externo que irá considerar o primeiro como menor
    menor = i ;

    for (j = i+1; j<n; j++ ) {

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
  int n ;
  struct racional  v[MAX], soma_total = {0,1};

  scanf("%d", &n) ; //Lê o n 
  if (n <= 0 || n >=100)
    return 0 ;


  for (int i = 0 ; i <= (n - 1) ; i++ )
    scanf("%ld %ld",&v[i].num, &v[i].den) ; //Verifica o numerador e o denominador 
  
  //Imprime todos os racionais do vetor 
  printf("VETOR = ");
  for (int i = 0 ; i <= n -1; i++) {
    imprime_r(v[i]) ;
    printf(" ") ;
  }
  printf("\n") ;

  //Imprime os racionais do vetor sem os elementos invalidos 
  printf("VETOR = ") ;
  for (int i = 0; i <= n -1 ; i++) {
    if (valido_r(v[i])) { //Verifica se o número é valido para imprimir ele 
      imprime_r(v[i]) ;
      printf(" ") ;
    }
  }
  printf("\n") ;

  //Imprime os racionais de forma ordenada
  ordena_racionais(v, n) ; 
  printf("VETOR = ") ;
  for (int i = 0; i <= (n-1); i++) {
    if (valido_r(v[i])) {
      imprime_r(v[i]) ;
      printf(" ") ;
    } ;
  }
  printf("\n") ;

  //Imprime a soma dos racionais 
  printf("SOMA = ") ;
  for (int i = 0; i < n; i++) {
    if (valido_r(v[i]))
    soma_r(soma_total, v[i], &soma_total) ;
  }
  imprime_r(soma_total) ;
  printf("\n") ;

  return (0) ;
}
  