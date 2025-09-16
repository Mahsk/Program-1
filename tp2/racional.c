#include <stdio.h>
#include <stdlib.h>
#include "racional.h"



/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max) {
  
  return min + rand() % (max - min + 1) ;
 
}

/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
long mdc (long a, long b) {
  long resto, temp ;

  if (b > a) { //Se b for maior, ele inverte a posição com a

    temp = a ;
    a = b ;
    b = temp ;

  } 

    while (b != 0) { 

      resto = a % b ;
      a = b ;
      b = resto; 


    }
  
    return a ;
}



/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b) {

  return (a * b) / mdc(a,b) ; 

}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r) {
  long max_divisor;

  if (!valido_r(r)) // se nao for válido, devolve sem chamar o mdc
    return r ;

  max_divisor = mdc(r.num, r.den) ;

  //Simplifica
  if (max_divisor != 0) {
    r.num /= max_divisor ;
    r.den /= max_divisor ;  
  }

  /* Se ambos numerador e denominador forem negativos, deve retornar um positivo.
  Se o denominador for negativo, o sinal deve migrar para o numerador. */
  if (r.den < 0) {

    r.num = - r.num ;
    r.den = - r.den ;

 }

    return r ;
}

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r (long numerador, long denominador) {
  struct racional r ;

  r.num = numerador ;
  r.den = denominador ;

  return r ;

}

/* Retorna 1 se o racional r for válido ou 0 se for inválido.
 * Um racional é inválido se seu denominador for zero */
int valido_r (struct racional r) {

  if (r.den == 0) 
    return 0 ;
  return 1 ;

}

/* Retorna um número racional aleatório na forma simplificada.
 * o racional gerado pode ser válido ou inválido.
 * O numerador e o denominador devem ser inteiros entre min e max. */
struct racional sorteia_r (long min, long max) {
  struct racional r ;

  //Sorteio do numerador e depois o denominador
  r.num = aleat(min, max) ;
  r.den = aleat(min, max) ;

  //Cria o racional
  r = cria_r(r.num, r.den) ;

  //Se o racional for valido, ele simplifica
  if (valido_r(r))
    r = (simplifica_r(r)) ;
  return r ;

}

/* Imprime um racional r, respeitando estas regras:
   - o racional deve ser impresso na forma simplificada;
   - não imprima espaços em branco e não mude de linha;*/

  void imprime_r (struct racional r) {

    r = simplifica_r(r) ;
    //se o racional for inválido, deve imprimir a mensagem "NaN";
    if (!valido_r(r)) {
      printf ("NaN");
      return ;
    }

    //Se o numerador for 0, deve imprimir somente "0";
    if(r.num == 0){
      printf("0") ;
      return ;
    }

    //Se o denominador for 1, deve imprimir somente o numerador;
    if (r.den == 1){
      printf("%ld",r.num) ;
      return ;
    }

    //Se o numerador e denominador forem iguais, deve imprimir somente "1";
    if (r.num == r.den) {
      printf("1") ;
      return ;
    }

    printf("%ld/%ld",r.num,r.den) ;
    return ;
    
}

/* Compara dois racionais r1 e r2. Retorno: -2 se r1 ou r2 for inválido,
 * -1 se r1 < r2, 0 se r1 = r2 ou 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2) {

  //Verifica se r1 ou r2 sao inválidos
  if (!valido_r(r1) || !valido_r(r2))
    return -2 ;
  
  //Se o denominador for negativo, ele troca o sinal 
  if (r1.den < 0) {
    r1.num = -r1.num ;
    r1.den = -r1.den ;
  }

  if (r2.den < 0) {
    r2.num = -r2.num ;
    r2.den = -r2.den ;
  }
  
  //Se r1 < r2, retorna -1
  if (r1.num * r2.den < r2.num * r1.den)
    return -1;

  //Se r1 > r2, retorna 1
  if (r1.num * r2.den > r2.num * r1.den)
    return 1 ;
  return 0 ; //Se r1 = r2

}

  int soma_r (struct racional r1, struct racional r2, struct racional *r3) {
    long soma, den ;
    struct racional r ;

    // 0 se r1 ou r2 for inválido ou se *r3 for nulo
    if (!valido_r(r1) || !(valido_r(r2)) || r3 == NULL)
      return 0 ;

    den = mmc(r1.den, r2.den) ;
    soma = (r1.num * den / r1.den) + (r2.num * den) / r2.den ;
    
    
    r.num = soma ;
    r.den = den ;

    *r3 = cria_r(r.num,r.den) ; //Retorna a soma dos racionais r1 e r2 no parametro *r3
  
    return 1 ; //Retorna 1 se a operacao foi bem sucedida
  
}


int subtrai_r (struct racional r1, struct racional r2, struct racional *r3) {
  long subtrai, den ;
  struct racional r ;

  //0 se r1 ou r2 for inválido ou se *r3 for nulo
  if (!valido_r(r1) || !(valido_r(r2)) || r3 == NULL)
    return 0 ;

  den = mmc(r1.den, r2.den) ;
  subtrai = (r1.num * den) / r1.den - (r2.num * den) / r2.den ;

  r.num = subtrai ;
  r.den = den ;


  *r3 = cria_r(r.num,r.den) ; //Retorna a subtracao dos racionais r1 e r2 no parametro *r3

  return 1 ; //Retorna 1 se a operacao foi bem sucedida 

}


int multiplica_r (struct racional r1, struct racional r2, struct racional *r3) {
  long num,den ;
  struct racional r ;

  // 0 se r1 ou r2 for inválido ou se *r3 for nulo
  if (!valido_r(r1) || !(valido_r(r2)) || r3 == NULL)
    return 0 ;

  num = r1.num * r2.num ;
  den = r1.den * r2.den ;

  r.num = num ;
  r.den = den ;

  *r3 = cria_r(r.num,r.den) ; //Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3

  return 1 ; // Retorna 1 se a operacao foi bem sucedida 

}

int divide_r (struct racional r1, struct racional r2, struct racional *r3) {
  long num, den ;
  struct racional r ;

  //0 se r1 ou r2 for inválido ou se *r3 for nulo
  if (!valido_r(r1) || !(valido_r(r2)) || r3 == NULL)
    return 0 ;

  num = r1.num * r2.den;
  den = r1.den * r2.num;

  r.num = num ;
  r.den = den ;

  *r3 = cria_r(r.num,r.den) ; //Retorna a divisao dos racionais r1 e r2 no parametro *r3

  return 1 ; //Retorna 1 se a operacao foi bem sucedida 

}