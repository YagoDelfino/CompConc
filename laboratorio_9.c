#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>


sem_t lock_produtor, lock_consumidor;
sem_t cheio, vazio;

#define N 41
int buffer[N];
int g = 0;

typedef struct{
 int id;
}tArgs;

void *produtor(void *arg) {
    tArgs *args = (tArgs*) arg;
    int item = 0;
    while(1){
        sem_wait(&vazio);
        sem_wait(&lock_produtor);
        printf("Produtor %d inserindo item %d\n", args->id, item);
        buffer[g] = item;
        g++;
        sem_post(&lock_produtor);
        if(g == N){
            g = 0;
            sem_post(&cheio);
        }
        item++;
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg) { 
    int* items;
    tArgs *args = (tArgs*) arg;
   
    items = (int*) malloc(sizeof(int) * N);
    int f = 0;
    while(1){
        sem_wait(&cheio);
        sem_wait(&lock_consumidor);
        for(int i = 0; i < N; i++){
            items[i] = buffer[f];
            f = (f+1) % N;
        }
        sem_post(&lock_consumidor);

        printf("Consumidor %d consumiu:\n", args->id); 
        for(int i = 0; i < N; i++){
            printf("%d \n", items[i]);
        } 

        for(int i = 0; i < N; i++) {
            sem_post(&vazio);
        }
    }
    free(items);
    pthread_exit(NULL);
}

int main (int argc, char* argv[]){

    if(argc == 1 || argc == 2){
        printf("Digite %s <quantidade de produtores> <quantidade de consumidores>\n", argv[0]);
        return 1;
    }

    int Produtores, Consumidores, Nthreads, i;
    Produtores = atoi(argv[1]);
    Consumidores = atoi(argv[2]);
    Nthreads = Produtores + Consumidores;
    pthread_t tid[Nthreads];
    tArgs *args;

    sem_init(&lock_produtor, 0, 1);
    sem_init(&lock_consumidor, 0, 1);
    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, N);
        
    // Alocação das estruturas
    args = (tArgs*) malloc(sizeof(tArgs)*Nthreads);
    if(args == NULL) {printf("Não conseguiu alocar\n"); return 2;}
    for(i = 0; i<Nthreads; i++){
        (args+i)->id = i;
    }

    for(i = 0; i < Produtores; i++) {
        if (pthread_create(&tid[i], NULL, produtor, (void*) (args+i))) { printf("Erro ao criar as threads\n"); exit(-1); }
    }

    for(i = Produtores; i < Produtores + Consumidores; i++) {
        if (pthread_create(&tid[i], NULL, consumidor, (void*) (args+i))) { printf("Erro ao criar as threads\n"); exit(-1); }
    }
    
    
    //Espera todos os processos terminarem
     for(i = 0; i<Nthreads; i++){
            pthread_join(tid[i], NULL);    
        }
        
    //libera o espaço armazenado
    free(args);
    return 0;
}