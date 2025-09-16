#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b      */
/* calcula o mdc pelo metodo de Euclides */
long mdc (long a, long b) {
long temp, resto ;

/*Se o denominador for maior, ele troca a posição, para garantir a divisão por inteiro*/
  if (b > a) {
    temp = a ;
    a = b ;
    b = temp ;
  }

/*Enquanto, o resto nao for zero, ele continua dividindo*/
  while (b != 0) {
    resto = a % b ;
    a = b ;
    b = resto ;
  }

  return a ;
}

/* Minimo Multiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b) {

  return  (a * b) / mdc (a, b) ;

}

/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 muda para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int *simplifica_r (struct racional *r) {
  long max_divisor ; 

  /*Simplifica*/
  max_divisor = mdc (r -> num, r-> den) ; 
  r -> num /= max_divisor ;
  r -> den /= max_divisor ;
  

  /*Se ambos numerador e denominador forem negativos, deve retornar um positivo. */
  /*Se o denominador for negativo, o sinal deve migrar para o numerador. */
  if (r -> den < 0) {
      r -> num = (-1) * r -> num ; 
      r -> den = (-1) * r ->  den ;
  }
  return r;
}

/* Cria um número racional com o numerador e denominador indicados
 * e retorna um ponteiro que aponta para ele.
 * A memória para o número racional deve ser alocada dinamicamente
 * por esta função. Retorna NULL se não conseguiu alocar a memória. */
struct racional *cria_r (long numerador, long denominador) {
  struct racional *r ;

  //Aloca a memória para struct racional
  struct racional *r = malloc(sizeof(struct racional)) ;

  /*Verifica se conseguiu alocar na memória, ou seja, se r for nulo, ele não conseguiu*/
  if ( r == NULL)
    return NULL ;

  /*Atribui os valores*/
  r -> num = numerador ;
  r -> den = denominador ;

  /*Retorna o ponteiro se ele conseguiu */
  return r ;

}

/* Libera a memória alocada para o racional apontado por r  e o aterra*/
void destroi_r (struct racional *r) {

  free(r) ;
  r = NULL ;

}

/* Retorna 1 se o racional r for válido ou 0 se for inválido. Um racional
 * é inválido se o denominador for zero ou se ele não tiver sido alocado. */
int valido_r (struct racional *r) {

  if ((r -> den = 0) ||  (r == NULL))
    return 0 ;
  return 1 ;
  
}

/* Imprime um racional r, respeitando estas regras:
   - o racional deve estar na forma simplificada;
   - não use espacos em branco e não mude de linha;
   - o formato de saída deve ser "num/den", a menos dos casos abaixo;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r (struct racional *r) {
      
  /*Se o ponteiro for nulo, imprime a mensagem "NULL"*/
  if (r == NULL)
    printf("NULL") ;
  
  /*Se o racional for inválido, imprime a mensagem "NaN" (Not a Number)*/
  if (!valido_r(r))
    printf("NaN") ;
  
  /*Se o numerador for 0, imprime somente "0"*/
  if (r -> num = 0)
    printf("0") ;
  
  /*se o denominador for 1, imprime somente o numerador*/
  if (r -> den = 1)
    printf("%ld", r->num) ;
  
  /*Se o numerador e denominador forem iguais, imprime somente "1"*/
  if(r->num == r->den)
    printf("1") ;
   
  /*Se o numerador e o denominador forem negativos, o racional é positivo*/
  if (r->den < 0) {

    r->num = (-1) * r->num ;
    r->den = (-1) * r->den ;

  }
  if (r->num < 0) {

    printf("%ld/%ld", -r->num, r->den) ;  //Se o racional for negativo, o sinal é impresso antes do número
  printf("%ld/%ld", r->num, r->den) ;

  }

  printf("%ld/%ld", r->num, r->den) ;

}

/* Compara dois números racionais r1 e r2.*
 * Retorna -1 se r1 < r2; 0 se r1 = r2; 1 se r1 > r2.
 * Fazer a comparação baseado na divisão do numerador pelo denominador
 * pode gerar erro de arredondamento e falsear o resultado. */
int compara_r (struct racional *r1, struct racional *r2) {
  long long base_den, rac1, rac2 ;

  /*Retorna -2 se r1 ou r2 for inválido ou se o respectivo ponteiro for nulo.*/
  if (!valido_r(r1) || !valido_r(r2))
    return -2 ;
  
  /*Normalizar as bases pelo mmc*/
  base_den = mmc(r1->den, r2->den) ;

  rac1 = (base_den / r1->den) * r1->num ;
  rac2 = (base_den / r2->den) * r2->num ;

  /*Realiza a comparação dos racionais pelo num*/
  if (rac1 < rac2)
    return -1 ; 

  if (rac1 > rac2)
    return 1 ;

  return 0 ;

}

/* Coloca em *r3 a soma simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int soma_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  long long base_igual;

  /*Verifica se r3 é nulo*/
  if (r3 == NULL)
    return 0 ;

  /*Verifica se r1 ou r2 são inválidos*/
  if (!valido_r(r1)|| !valido_r(r2))
    return 0 ;

  /*Iguala as bases*/
  base_igual = (mmc(r1->den, r2->den)) ;

  /*Faz a operação soma*/
  r3 -> num = ((base_igual/ r1->den)*r1->num + (base_igual/r2->den)*r2->num) ;
  r3 -> den = base_igual ;

  simplifica_r(r3) ;
  return 1 ;  
}

/* Coloca em *r3 a diferença simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3) {
  long long base_igual ;

    /*Verifica se r3 é nulo*/
    if (r3 == NULL)
    return 0 ;

  /*Verifica se r1 ou r2 são inválidos*/
  if (!valido_r(r1)|| !valido_r(r2))
    return 0 ;

  /*Iguala as bases*/
  base_igual = (mmc(r1->den, r2->den)) ;

  /*Faz a operação soma*/
  r3 -> num = ((base_igual/ r1->den)*r1->num - (base_igual/r2->den)*r2->num) ;
  r3 -> den = base_igual ;

  simplifica_r(r3) ;
  return 1 ;   
}

/* Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3) {

  /*Verifica se r3 é nulo*/
  if (r3 == NULL)
    return 0 ;

  /*Verifica se r1 ou r2 são inválidos*/
  if (!valido_r(r1)|| !valido_r(r2))
    return 0 ;

  /*Faz a operação e retorna 1 se for bem sucedida*/
  r3 -> num = r1->num * r2->num ;
  r3 -> den = r1->den * r2->den ;

  simplifica_r(r3) ;

  return 1 ; 
}

/* Coloca em *r3 a divisão simplificada do racional *r1 por *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int divide_r (struct racional *r1, struct racional *r2, struct racional *r3) {

  /*Verifica se r3 é nulo*/
  if (r3 == NULL)
    return 0 ;

  /*Verifica se r1 ou r2 são inválidos*/
  if (!valido_r(r1)|| !valido_r(r2))
    return 0 ;

  /*Retorna 1 se a operacao foi bem sucedida*/
  r3 -> num = r1->num * r2->num ;
  r3 -> den = r1->den * r2->den ;
  
  simplifica_r(r3) ;

  return 1 ; 
}


