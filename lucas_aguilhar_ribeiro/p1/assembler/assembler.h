#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define MAX_LINHA 100
#define MAX_LABELS 50
#define TAM_MEMORIA 256

// Estrutura para representar labels
typedef struct {
    char label[20];
    int endereco;
} Label;

// Estrutura para representar instruções
typedef struct {
    char mnem[10];
    int opcode;
} Instrucao;

// Função para obter o opcode de uma instrução pelo mnemônico
int get_opcode(char *mnem);

// Função para adicionar uma nova label à tabela
void adicionar_label(const char *nome, int endereco);

// Função para obter o endereço de uma label pela tabela
int get_endereco_label(const char *nome);

// Função para verificar se uma linha contém uma label
int is_label(const char *linha);

int processarAssembly(FILE *entrada, FILE *saida);

#endif
