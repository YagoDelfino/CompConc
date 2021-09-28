#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>


sem_t cond1, cond2, cond3, cond4;

void* T1(void* id){
    sem_wait(&cond1);
    puts("Fique à vontade.");
    sem_post(&cond3);      
    pthread_exit(NULL);
}

void* T2(void* id){
    puts("Seja bem vindo!");
    sem_post(&cond1); 
    sem_post(&cond4);   
    pthread_exit(NULL);
}

void* T3(void* id){
    sem_wait(&cond2); 
    sem_wait(&cond3);
    puts("Volte sempre!");
    pthread_exit(NULL);
}

void* T4(void* id){
    sem_wait(&cond4);
    puts("Sente-se por favor.");
    sem_post(&cond2);
    pthread_exit(NULL);

}

typedef struct{
 int id;
}tArgs;

int main (int argc, char* argv[]){
    pthread_t tid[4];
    tArgs *args;

    sem_init(&cond1, 0, 0);
    sem_init(&cond2, 0, 0);
    sem_init(&cond3, 0, 0);
    sem_init(&cond4, 0, 0);
      
    // Alocação das estruturas
    args = (tArgs*) malloc(sizeof(tArgs)*4);
    if(args == NULL) {printf("Não conseguiu alocar\n"); return 2;}
    for(int i = 0; i<4; i++){
        (args+i)->id = i;
    }
    if(pthread_create(&tid[0], NULL, T1, (void*) (args))){
        printf("Erro ao criar as threads\n"); exit(-1);
    }
    
    if(pthread_create(&tid[1], NULL, T2, (void*) (args+1))){
        printf("Erro ao criar as threads\n"); exit(-1);
    }

    if(pthread_create(&tid[2], NULL, T3, (void*) (args+2))){
        printf("Erro ao criar as threads\n"); exit(-1);
    }

    if(pthread_create(&tid[3], NULL, T4, (void*) (args+2))){
        printf("Erro ao criar as threads\n"); exit(-1);
    }
   
   
     //Espera todos os processos terminarem
    for(int i = 0; i<4; i++){
        pthread_join(tid[i], NULL);    
    }
     
    //libera o espaço armazenado
    free(args);
    return 0;
}