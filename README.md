# Produto_interno_de_dois_vetores
Programa que calcula o produto interno de dois vetores

Laboratório 3: Programação Concorrente com Paralelismo de Dados
Este repositório contém a solução para a Atividade 4 (Exercício 1) do Laboratório 3 de Programação Concorrente, ministrado pela Profa. Silvana Rossetto. O objetivo é projetar e implementar uma solução concorrente para o problema de calcular o produto interno de dois vetores de números reais.

Estrutura do Repositório
gera_testes_prod_interno.c: Programa sequencial auxiliar para gerar os vetores de entrada e o resultado esperado do produto interno em um arquivo binário.
prod_interno_conc.c: Programa concorrente que lê os vetores do arquivo de entrada, calcula o produto interno usando múltiplas threads e avalia o desempenho.
timer.h: Arquivo de cabeçalho para medição de tempo de execução.
resultados.txt: Arquivo com a saída bruta dos testes de desempenho.
README.md: Este arquivo.

Instruções de Compilação e Execução
Os programas foram desenvolvidos em C e devem ser compilados e executados em um ambiente Linux (como o WSL).
Compilação
Programa gera_testes_prod_interno:
Este programa não requer bibliotecas adicionais.
Comando de compilação: gcc -o gera_testes_prod_interno gera_testes_prod_interno.c
Programa prod_interno_conc:
Este programa utiliza a biblioteca Pthreads e a biblioteca matemática.
Comando de compilação: gcc -o prod_interno_conc prod_interno_conc.c -lpthread -lm
Execução
Gerar arquivos de teste:
O programa gera_testes_prod_interno recebe a dimensão dos vetores e o nome do arquivo de saída.
Exemplo: /.gera_testes_prod_interno 1000000 teste_1M.bin
Executar o cálculo concorrente:
O programa prod_interno_conc recebe o nome do arquivo de entrada e o número de threads.
Exemplo para rodar com 4 threads: /.prod_interno_conc teste_1M.bin 4

Resultados do Experimento
Os resultados detalhados dos testes estão disponíveis no arquivo resultados.txt. A análise de desempenho demonstra que a solução concorrente apresenta ganhos significativos para vetores de grandes dimensões, superando a sobrecarga de threads.
A variação relativa entre o resultado sequencial e o concorrente foi consistentemente baixa (na ordem de 10^−10), o que comprova a precisão do algoritmo.
