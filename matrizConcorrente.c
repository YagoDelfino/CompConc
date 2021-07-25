#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

float *m1;
float *saida;
int nthreads;

typedef struct{
 int id;
 int tam;
}tArgs;

void * tarefa(void *arg){
  int somaprod;
  tArgs *args = (tArgs*) arg;
  
  for(int i = args->id; i < args->tam; i+=nthreads)
  {
     for(int j = 0; j < (args->tam); j++)
     {
        somaprod = 0;
     	for(int k = 0; k < (args->tam); k++){
     	  somaprod += m1[i*(args->tam)+k] * m1[k*(args->tam)+j];
     	  saida[i*(args->tam)+j] = somaprod;
     	}
     }	
  }
  pthread_exit(NULL);
}

int corretude(float *matrizSaida, int tam){
  int tamanho = tam;
  int soma;
  float *matrizParametro;
  float *saidaParametro;
  float *saida = matrizSaida;

   matrizParametro = (float *) malloc(sizeof(float) * tam * tam);
   if(matrizParametro == NULL) {printf("Não conseguiu alocar\n"); return 2;}
   saidaParametro = (float *) malloc(sizeof(float) * tam * tam);
   if(saidaParametro == NULL) {printf("Não conseguiu alocar\n"); return 2;}
  
  for(int i = 0; i<tamanho; i++){
     for(int j = 0; j<tamanho; j++){
        matrizParametro[i*tamanho+j] = 1;
        saidaParametro[i*tamanho+j] = 0;
     }
   }
   
   for(int i = 0; i < tamanho; i++)
   	for(int j = 0; j < tamanho; j++){
          soma = 0;
     	  for(int k = 0; k < tamanho; k++){
     	    soma += matrizParametro[i*tamanho+k] * matrizParametro[k*tamanho+j];
     	    saidaParametro[i*tamanho+j] = soma;
         }
       }
     
     for(int i = 0; i<tamanho; i++){
     	for(int j = 0; j<tamanho; j++){
     	if(saidaParametro[i*tamanho+j] != saida[i*tamanho+j]){
     	  printf("Não é igual\n");
     	  return 0;
     	}
    }
  }
  printf("Está correto\n");
}

int main(int argc, char* argv[]) {
   int tam;
   pthread_t *tid;
   tArgs *args;
   double inicio,fim, delta;
   
   GET_TIME(inicio);
   if(argc <3){
     printf("Digite %s <tamanho da matriz> <numero de threads> \n", argv[0]);
     return 1;
   }
   tam = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if(nthreads > tam) nthreads = tam;
   printf("Começado com %d thread(s) e dimensão %d\n", nthreads,tam);
   
   m1 = (float *) malloc(sizeof(float) * tam * tam);
   if(m1 == NULL) {printf("Não conseguiu alocar\n"); return 2;}
   saida = (float *) malloc(sizeof(float) * tam * tam);
   if(saida == NULL) {printf("Não conseguiu alocar\n"); return 2;}
   
   //Inicializando o vetor
   for(int i = 0; i<tam; i++){
     for(int j = 0; j<tam; j++){
        m1[i*tam+j] = 1;
     	saida[i*tam+j] = 0;
     }
   }
   GET_TIME(fim);
   delta = fim - inicio;
   printf("O tempo de inicialização é %lf\n", delta);
   
   GET_TIME(inicio);
   // Alocação das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid == NULL) {printf("Não conseguiu alocar\n"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args == NULL) {printf("Não conseguiu alocar\n"); return 2;}
   for(int i = 0; i<nthreads; i++){
     (args+i)->id = i;
     (args+i)->tam = tam;
     if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
     	printf("Erro ao criar as threads\n"); return 3;
     }
   }
   for(int i = 0; i<nthreads; i++){
     pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim);
   delta = fim - inicio;
   printf("O tempo de multiplicação é %lf\n", delta);

   
   //corretude(saida, tam);
    GET_TIME(inicio);
   free(m1);
   free(saida);
   free(tid);
   free(args);
   GET_TIME(fim);
   delta = fim - inicio;
   printf("O tempo de finalização é %lf\n", delta);
   return 0;
   
}
