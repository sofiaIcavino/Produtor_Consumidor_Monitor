#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define TAM_BUFFER 5          // Tamanho do buffer circular
#define N_PRODUTORES 6        // Quantidade de threads produtoras
#define N_CONSUMIDORES 6      // Quantidade de threads consumidoras

#define N_QUANTIDADE 5        // Quantidade de inserções/remoções por thread


int buffer[TAM_BUFFER];       // Buffer circular
int in = 0, out = 0;          // Índices de inserção e remoção
int count = 0;                // Contador de itens no buffer

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER;

// Função para gerar uma seed local pseudo-aleatória 
unsigned int lcg_rand(unsigned int* seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return *seed;
}

// Função da thread produtor
void* produtor(void* arg) {
    int id = *((int*)arg);
    int cont=0; //Varável contadora de produções

    // Gera uma seed única baseada no tempo e no ID da thread
    unsigned int  seed = time(NULL) ^ (id * 7919);
    
    while(cont<N_QUANTIDADE){
        
        int numero_inserido = lcg_rand(&seed) % 100; // Novo valor aleatório a cada iteração

        pthread_mutex_lock(&lock);

        // Seção crítica
        /* Verifica quando não houver espaço disponível, ou seja, count é igual o tamanho do buffer.
        Assim, o produtor fica em espera e não pode inserir.*/
        while (count==TAM_BUFFER) { // condição de uso
            pthread_cond_wait(&cond_prod, &lock);
        }

        buffer[in]= numero_inserido; // Inserindo um novo valor
        in = (in + 1) % TAM_BUFFER; // Avança o índice circular
        count++; // Incrementa o contador de itens

        printf("[Produtor %d] Inseriu: %d\n",id,numero_inserido); // Registro da inserção
        
        pthread_cond_signal(&cond_cons); // Sinaliza o consumidor
        pthread_mutex_unlock(&lock);

        cont++; // Incrementa o contador de produções
        sleep(1); // Espera simbólica para simular trabalho
    }
    
    return NULL;
}

// Função da thread consumidor
void* consumidor(void* arg) {
    int id = *((int*)arg);
    int cont=0; // Variável contadora de produção

    while(cont<N_QUANTIDADE){
    
        pthread_mutex_lock(&lock);
        
        // Seção crítica
        /* Verifica quando não houver nenhum item no buffer, ou seja, count é igual a 0.
        Assim, o consumidor fica em espera e não pode remover.*/
        while (count==0) { // condição de uso
            pthread_cond_wait(&cond_cons, &lock);
        }

        int numero_removido=buffer[out]; // Salva o numero removido
        buffer[out]=-1; // Remove o item

        out=(out+1)%TAM_BUFFER; // Avança o índice circular
        count--; // Decrementa o contador de itens

        printf("[Consumidor %d] Removeu: %d\n",id,numero_removido); // Registro de remoção
    
        pthread_cond_signal(&cond_prod);// Sinaliza o produtor
        pthread_mutex_unlock(&lock);

        cont++;
        sleep(1);
    }

   return NULL;
}

int main() {
    pthread_t produtores[N_PRODUTORES], consumidores[N_CONSUMIDORES];
    int ids_prod[N_PRODUTORES], ids_cons[N_CONSUMIDORES];

    srand(time(NULL));
    for (int i = 0; i < N_PRODUTORES; i++) {
        ids_prod[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, &ids_prod[i]);
    }

    for (int i = 0; i < N_CONSUMIDORES; i++) {
        ids_cons[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, &ids_cons[i]);
    }

    for (int i = 0; i < N_PRODUTORES; i++)
        pthread_join(produtores[i], NULL);
    for (int i = 0; i < N_CONSUMIDORES; i++)
        pthread_join(consumidores[i], NULL);

    return 0;
}
