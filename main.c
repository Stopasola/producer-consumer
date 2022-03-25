#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>

#define N 5
int buffer[5];
int count=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *produtor(){
    int i=0;
    int in=0;
    for (i=0; i < 6; i++){
        while (count == N); //do nothing , não pode produzir mais pq não tem espaço no buffer
        buffer[in]=i; // insere dentro do buffer compartilhado o valor i (poderia ser um pacote da placa de rede)
        //printf("Produtor posicao %d, valor %d\n", in, i);
        in = (in+1)%N; // Serve para percorrer as posições do buffer de maneira circular
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void *consumidor(){
    int i=0;
    int out=0;
    int temp;
    for (i=0; i < 6; i++){
        while (count == 0 ); //do nothing, n pode consumir se o buffer estiver vazio
        temp = buffer[out];
        //printf("Consumidor posicao %d valor %d\n", out, temp);
        out= (out+1)%N;
        pthread_mutex_lock(&mutex);
        count--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main()
{
    pthread_t p1, p2;
    //printf("Produtor consumidor iniciado \n");
    pthread_create(&p1, NULL, &produtor, NULL);
    pthread_create(&p2, NULL, &consumidor, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    // printf("Produtor consumidor finalizado \n");
    return 0;
}


