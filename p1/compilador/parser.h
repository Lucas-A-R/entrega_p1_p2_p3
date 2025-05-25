#ifndef PARSER_H
#define PARSER_H

#include <stdio.h> // Necessário para o tipo FILE

// Estrutura para representar os erros do parser
typedef struct {
    int linha;         // Linha onde ocorreu o erro
    int coluna;        // Coluna onde ocorreu o erro
    char mensagem[256]; // Mensagem descrevendo o erro
} ErroParser;

// Função para processar o arquivo de entrada e gerar o Assembly correspondente
int processarArquivo(FILE *entrada, FILE *saida);

// Função para exibir os detalhes do erro de forma legível
void exibirErro(ErroParser erro);

#endif
