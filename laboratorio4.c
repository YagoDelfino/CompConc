#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N_THREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond_a, x_cond_b;

void *entrada(void *arg) {
  int gastar_tempo1, gastar_tempo2;
  printf("Seja bem-vindo!\n");
  /* faz alguma coisa pra gastar tempo... */
  gastar_tempo1=10000; gastar_tempo2=-100; while (gastar_tempo2 < gastar_tempo1) gastar_tempo2++;
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_broadcast(&x_cond_a);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

void *vontade(void *arg) {
  pthread_mutex_lock(&x_mutex);
  if(x == 0)
    pthread_cond_wait(&x_cond_a, &x_mutex);
  x++;
  printf("Fique a vontade.\n");
  if(x == 3) {
    pthread_cond_signal(&x_cond_b);
  }
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

void *sentar(void *arg) {
  pthread_mutex_lock(&x_mutex);
  if(x == 0)
    pthread_cond_wait(&x_cond_a, &x_mutex);
  x++;
  printf("Sente-se por favor.\n");
  if(x == 3) {
    pthread_cond_signal(&x_cond_b);
  }
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);  
}

void *saida(void *arg) {
  pthread_mutex_lock(&x_mutex);
  if(x < 3)
    pthread_cond_wait(&x_cond_b, &x_mutex);
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

int main() {
  int i;
  pthread_t threads[N_THREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond_a, NULL);
  pthread_cond_init (&x_cond_b, NULL);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, entrada, NULL);
  pthread_create(&threads[0], NULL, vontade, NULL);
  pthread_create(&threads[3], NULL, sentar, NULL);
  pthread_create(&threads[2], NULL, saida, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < N_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond_a);
  pthread_cond_destroy(&x_cond_b);
}