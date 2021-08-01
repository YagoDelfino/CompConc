#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int *vetor;
long long int dim;
int nthreads;
double inicio, fim;

typedef struct{
 int id;
}tArgs;

void * tarefa(void *arg){
  long long int menor, maior; //variaveis locais onde ficarão guardados o maior e o menor valor do vetor
  maior = vetor[0];
  menor = vetor[0];
  
  int *resultado;
  resultado = (int*) malloc(sizeof(int)*2);
      if(resultado == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }

  tArgs *args = (tArgs*) arg;
  
  long long int Bloco = dim/nthreads;
  long long int inicio = args->id * Bloco;
  long long int fim;

  if(args->id == nthreads - 1) fim = dim;
  else fim = inicio + Bloco;
  for(long long int i = inicio; i < fim; i++)
    {
      if(vetor[i] > maior) maior = vetor[i];
      if(vetor[i] < menor) menor = vetor[i]; 
    }
    resultado[0] = maior;
    resultado[1] = menor;
  pthread_exit((void *) resultado);
}

int main (int argc, char *argv[]){
  
  switch(argc){
    case 1: 
      printf("Digite %s <dimensão do vetor> para sequencial\n", argv[0]);
      printf("Ou %s <dimensão do vetor> <número de threads> para concorrente\n", argv[0]);
     return 1;
     
    case 2:
      /* SEQUENCIAL */
      dim = atoll(argv[1]);
      int maior, menor;
      //Armazenando espaço na memória para o vetor
      vetor = (int*) malloc(sizeof(int)*dim);
      if(vetor == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }
      
      //Preenchendo o vetor
      for(long int i = 0; i<dim; i++){
        vetor[i] = i%dim;
      }
      //Procurando os maiores e os menores números do vetor
      maior = vetor[0];
      menor = vetor[0];
      GET_TIME(inicio);
      for(long int i = 0; i<dim; i++){
        if(vetor[i] > maior) maior = vetor[i];
        if(vetor[i] < menor) menor = vetor[i]; 
      }
      GET_TIME(fim);
      //Imprimindo o vetor
     /* puts("O vetor é:");
      for(long int i = 0; i<dim; i++){
        printf("%d ", vetor[i]);
      }
      puts("\n");*/
      printf("O maior valor do vetor é: %d\n", maior);
      printf("O menor valor do vetor é: %d\n", menor);
      printf("Tempo de execução Sequencial: %lf\n", fim-inicio);
      
      //libera o espaço armazenado
      free(vetor);
      return 2;
      
    case 3:
      /* CONCORRENTE */
      dim = atoll(argv[1]);
      nthreads = atoi(argv[2]);
      pthread_t *tid;
      tArgs *args;
      int *retorno;
      
      if(nthreads > dim) nthreads = dim;
   	printf("Começado com %d thread(s) e dimensão %lld\n", nthreads,dim);
   	
      //Armazenando espaço na memória para o vetor	
      vetor = (int*) malloc(sizeof(int)*dim);
      if(vetor == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }
      retorno = (int*) malloc(sizeof(int)*2);
      if(retorno == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }
      
      //Preenchendo o vetor
      for(long int i = 0; i<dim; i++){
        vetor[i] = i%dim;
      }
      GET_TIME(inicio);
      // Alocação das estruturas
      tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
      if(tid == NULL) {printf("Não conseguiu alocar\n"); return 2;}
      args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
      if(args == NULL) {printf("Não conseguiu alocar\n"); return 2;}
      for(int i = 0; i<nthreads; i++){
        (args+i)->id = i;
        if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
     	  printf("Erro ao criar as threads\n"); return 3;
     }
   }
   
   
      //Espera todos os processos terminarem
      for(int i = 0; i<nthreads; i++){
        pthread_join(*(tid+i), (void**) &retorno);
        menor = retorno[1];
        maior = retorno[0];
     }
     GET_TIME(fim);
     /*puts("O vetor é:");
      for(long int i = 0; i<dim; i++){
        printf("%d ", vetor[i]);
      }
      puts("\n");*/
      printf("O maior valor do vetor é: %d\n", maior);
      printf("O menor valor do vetor é: %d\n", menor);
      printf("Tempo de execução Concorrente: %lf\n", fim-inicio);
      
      //libera o espaço armazenado
      free(tid);
      free(args);
      free(vetor);
      return 3;
  }
  printf("Digite %s <dimensão do vetor> para sequencial\n", argv[0]);
  printf("Ou %s <dimensão do vetor> <número de threads> para concorrente\n", argv[0]);
  return 0;
}

