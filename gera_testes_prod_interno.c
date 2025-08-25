/* Disciplina: Programação Concorrente 
   Lab3  
   Luiz Eduardo Lahm Piccoli - DRE: 119.022.214 
   Programa auxiliar para gerar vetores de entrada para o problema do produto interno */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 //valor maximo de um elemento do vetor

int main(int argc, char*argv[]) {
   long int n; //dimensao dos vetores
   float *vetor1, *vetor2; //vetores de entrada
   double produto_interno = 0.0; //produto interno total
   int fator = 1; //fator multiplicador para gerar numeros negativos
   FILE *descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida

   //recebe os argumentos de entrada
   if(argc < 3) {
      fprintf(stderr, "Digite: %s <dimensao> <nome arquivo saida>\n", argv[0]);
      return 1;
   }
   n = atoi(argv[1]);

   //aloca memoria para os vetores
   vetor1 = (float*) malloc(sizeof(float) * n);
   if(!vetor1) {
      fprintf(stderr, "Erro de alocacao da memoria do vetor1\n");
      return 2;
   }
   vetor2 = (float*) malloc(sizeof(float) * n);
   if(!vetor2) {
      fprintf(stderr, "Erro de alocacao da memoria do vetor2\n");
      return 3;
   }

   //preenche os vetores com valores float aleatorios e calcula o produto interno
   srand(time(NULL));
   for(long int i = 0; i < n; i++) {
        vetor1[i] = (rand() % MAX) / 3.0 * fator;
        fator *= -1;
        vetor2[i] = (rand() % MAX) / 3.0 * fator;
        fator *= -1;
        produto_interno += vetor1[i] * vetor2[i]; //acumula o produto
   }

   //escreve os dados no arquivo binario
   descritorArquivo = fopen(argv[2], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 4;
   }
   //escreve a dimensao
   ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
   //escreve os elementos do primeiro vetor
   ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
   if(ret < n) {
      fprintf(stderr, "Erro de escrita no arquivo (vetor1)\n");
      return 5;
   }
   //escreve os elementos do segundo vetor
   ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
   if(ret < n) {
      fprintf(stderr, "Erro de escrita no arquivo (vetor2)\n");
      return 6;
   }
   //escreve o produto interno
   ret = fwrite(&produto_interno, sizeof(double), 1, descritorArquivo);

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(vetor1);
   free(vetor2);
   return 0;
}