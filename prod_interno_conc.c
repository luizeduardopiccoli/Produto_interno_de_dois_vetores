/* Disciplina: Programação Concorrente */
/* Lab3:  */
/* Luiz Eduardo Lahm Piccoli - DRE: 119.022.214 */
/* Codigo: Calcula o produto interno de dois vetores de floats de forma concorrente */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h> //necessario para o fabs
#include "timer.h"

//variaveis globais
//vetores de elementos
float *vetor1_global, *vetor2_global;

//estrutura de dados para passar argumentos para a thread
typedef struct {
   //tamanho dos vetores
   long int n;
   //numero de threads 
   short int nthreads;
   //identificador da thread
   short int id;
} t_args;  


//funcao executada pelas threads
//estrategia de divisao de tarefas: blocos de n/nthreads elementos
void *ProdutoInterno(void *args) {
  t_args *arg = (t_args*) args; //argumentos da thread
  int ini, fim, fatia; //auxiliares para divisao do vetor em blocos
  double produto_local = 0.0, *ret; //somatorio local
  
  fatia = arg->n / arg->nthreads; //tamanho do bloco de dados de cada thread
  ini = arg->id * fatia; //posicao inicial do vetor
  fim = ini + fatia; //posicao final do vetor
  if (arg->id == (arg->nthreads-1)) fim = arg->n; //a ultima thread trata os elementos restantes no caso de divisao nao exata

  //soma os produtos dos valores 
  for(int i = ini; i < fim; i++) {
     produto_local += (double)vetor1_global[i] * (double)vetor2_global[i];
  }

  //retorna o resultado do produto interno
  ret = (double*) malloc(sizeof(double));
  if (ret != NULL) *ret = produto_local;
  else printf("--ERRO: malloc() thread\n");
  pthread_exit((void*) ret);
}

//funcao principal do programa
int main(int argc, char *argv[]) {
  long int n; //tamanho dos vetores
  short int nthreads; //numero de threads 
  FILE *arq; //arquivo de entrada
  size_t ret; //retorno da funcao de leitura no arquivo de entrada
  double prod_interno_seq; //produto interno registrado no arquivo (sequencial)
  double prod_interno_par_global = 0.0; //resultado do produto interno concorrente
  double *prod_retorno_threads; //auxiliar para retorno das threads
  
  pthread_t *tid_sistema; //vetor de identificadores das threads no sistema
  double inicio, fim, delta; //variaveis para a tomada de tempo

  //valida e recebe os valores de entrada
  if(argc < 3) { printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]); exit(-1); }

  //abre o arquivo de entrada
  arq = fopen(argv[1], "rb");
  if(arq == NULL) { printf("--ERRO: fopen()\n"); exit(-1); }

  //le o tamanho dos vetores
  ret = fread(&n, sizeof(long int), 1, arq);
  if(!ret) {
     fprintf(stderr, "Erro de leitura das dimensoes\n");
     return 3;
  }

  //aloca espaco de memoria e carrega os vetores de entrada
  vetor1_global = (float*) malloc(sizeof(float) * n);
  if(vetor1_global == NULL) { printf("--ERRO: malloc() vetor1\n"); exit(-1); }
  ret = fread(vetor1_global, sizeof(float), n, arq);
  if(ret < n) {
     fprintf(stderr, "Erro de leitura dos elementos do vetor1\n");
     return 4;
  }
  vetor2_global = (float*) malloc(sizeof(float) * n);
  if(vetor2_global == NULL) { printf("--ERRO: malloc() vetor2\n"); exit(-1); }
  ret = fread(vetor2_global, sizeof(float), n, arq);
  if(ret < n) {
     fprintf(stderr, "Erro de leitura dos elementos do vetor2\n");
     return 5;
  }

  //le o numero de threads da entrada do usuario 
  nthreads = atoi(argv[2]);
  //limita o numero de threads ao tamanho do vetor
  if(nthreads > n) nthreads = n;

  //aloca espaco para o vetor de identificadores das threads no sistema
  tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  if(tid_sistema == NULL) { printf("--ERRO: malloc() tid_sistema\n"); exit(-1); }

  GET_TIME(inicio); //inicio da tomada de tempo
  
  //cria as threads
  for(long int i = 0; i < nthreads; i++) {
    t_args *args;
    args = (t_args*) malloc(sizeof(t_args));
    if(args == NULL) {    
       printf("--ERRO: malloc argumentos\n"); exit(-1);
    }
    args->n = n;
    args->nthreads = nthreads;
    args->id = i;
    if (pthread_create(&tid_sistema[i], NULL, ProdutoInterno, (void*) args)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //espera todas as threads terminarem e calcula o produto interno total
  for(int i = 0; i < nthreads; i++) {
     if (pthread_join(tid_sistema[i], (void *) &prod_retorno_threads)) {
        printf("--ERRO: pthread_join()\n"); exit(-1);
     }
     prod_interno_par_global += *prod_retorno_threads;
     free(prod_retorno_threads);
  }

  GET_TIME(fim); //fim da tomada de tempo
  delta = fim - inicio;

  //le o produto interno registrado no arquivo
  ret = fread(&prod_interno_seq, sizeof(double), 1, arq); 
  if(ret < 1) {
     fprintf(stderr, "Erro de leitura do produto interno sequencial\n");
     return 6;
  }

  //calcula a variacao relativa
  double variacao_relativa = fabs((prod_interno_seq - prod_interno_par_global) / prod_interno_seq);

  //imprime os resultados
  printf("\n");
  printf("Produto Interno Sequencial   = %.16e\n", prod_interno_seq);
  printf("Produto Interno Concorrente  = %.16e\n", prod_interno_par_global);
  printf("Variação Relativa            = %.16e\n", variacao_relativa);
  printf("Tempo de execução (processamento) = %.16e segundos\n", delta);
  printf("\n");

  //desaloca os espacos de memoria
  free(vetor1_global);
  free(vetor2_global);
  free(tid_sistema);
  //fecha o arquivo
  fclose(arq);

  return 0;
}