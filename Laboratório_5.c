#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>//necessário p/ função time()

int *vetor;
long long int dim;
long long int nthreads;
long long int contador;
pthread_mutex_t mutex;
pthread_cond_t cond_bar;

typedef struct{
 int id;
}tArgs;

void barreira(int id){
  pthread_mutex_lock(&mutex);
  contador--;
  if (contador > 0){
    printf("A thread %d chegou na barreira\n", id);
    pthread_cond_wait(&cond_bar, &mutex);
  } else {
    contador=nthreads;
    pthread_cond_broadcast(&cond_bar);
    printf("A última thread %d chegou, liberando todas as threads\n", id);
  }
  pthread_mutex_unlock(&mutex);
}

void * tarefa(void *arg){

  tArgs *args = (tArgs*) arg;
  int *somatorio = (int*) malloc(sizeof(int));
  if(somatorio == NULL){
    printf("Não foi possível alocar espaço para o vetor\n");
      exit(-1);
  }

  for(int i = 0; i < dim; i++)
    {
      *somatorio += vetor[i];
      barreira(args->id);
      vetor[args->id] = rand() % 10;
      barreira(args->id);
    }

  pthread_exit((void *) somatorio);
} 

int main (int argc, char *argv[]){
  srand(time(NULL));
  switch(argc){
    case 1: 
      printf("Digite %s <dimensão do vetor> \n", argv[0]);
      printf("O <número de threads> será igual a dimensão do vetor\n", argv[0]);
     return 1;
      
    case 2:
      /* CONCORRENTE */
      dim = atoll(argv[1]);
      nthreads = atoll(argv[1]);
      contador = atoll(argv[1]);
      pthread_t *tid;
      tArgs *args;
      int *somatorio;
      int *resultado;
      
      //Armazenando espaço na memória para o vetor	
      vetor = (int*) malloc(sizeof(int)*dim);
      if(vetor == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }

      resultado = (int*) malloc(sizeof(int)*dim);
      if(resultado == NULL){
        printf("Não foi possível alocar espaço para o vetor\n");
        return 3;
      }

      //Preenchendo o vetor
      for(long int i = 0; i < dim; i++){
        vetor[i] = rand() % 10; //Números aleatórios entre 0 e 9
      }

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
        pthread_join(*(tid+i), (void**) &somatorio);
        resultado[i] = *somatorio;
     }

      puts("Os somatórios:");
      for(int i = 0; i<nthreads; i++){
        printf("%d\n", resultado[i]);
     }
      //libera o espaço armazenado
      free(tid);
      free(args);
      free(vetor);
      return 3;
  }
  printf("Digite %s <dimensão do vetor> \n", argv[0]);
  printf("O <número de threads> será igual a dimensão do vetor\n", argv[0]);
  return 0;
}

