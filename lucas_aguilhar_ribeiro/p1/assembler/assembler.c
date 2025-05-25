#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Label labels[MAX_LABELS];
int total_labels = 0;

Instrucao tabela[] = {
    {"NOP", 0x00}, {"STA", 0x10}, {"LDA", 0x20}, {"ADD", 0x30}, 
    {"OR",  0x40}, {"AND", 0x50}, {"NOT", 0x60}, {"JMP", 0x70}, 
    {"JN",  0x80}, {"JZ",  0x90}, {"HLT", 0xF0}
};
#define TAM_TABELA (sizeof(tabela) / sizeof(tabela[0]))

int get_opcode(char *mnem) {
    for (int i = 0; i < TAM_TABELA; i++) {
        if (strcmp(tabela[i].mnem, mnem) == 0) {
            return tabela[i].opcode;
        }
    }
    return -1; // Caso mnemônico não seja encontrado
}

void adicionar_label(const char *nome, int endereco) {
    if (total_labels < MAX_LABELS) {
        strcpy(labels[total_labels].label, nome);
        labels[total_labels].endereco = endereco;
        total_labels++;
    } else {
        printf("Erro: Número máximo de labels excedido!\n");
    }
}

int get_endereco_label(const char *nome) {
    for (int i = 0; i < total_labels; i++) {
        if (strcmp(labels[i].label, nome) == 0) {
            return labels[i].endereco;
        }
    }
    return -1; // Label não encontrada
}

int is_label(const char *linha) {
    return strchr(linha, ':') != NULL;
}

int processarAssembly(FILE *entrada, FILE *saida) {
    char linha[MAX_LINHA];
    int endereco = 0;

    // Primeira passagem: Resolver labels
    while (fgets(linha, sizeof(linha), entrada)) {
        if (linha[0] == ';') continue; // Ignorar comentários

        char *token = strtok(linha, " \t\n");
        if (!token) continue;

        if (is_label(token)) {
            token[strlen(token) - 1] = '\0'; // Remover ':'
            adicionar_label(token, endereco);
        } else {
            endereco++;
        }
    }
    rewind(entrada); // Voltar ao início do arquivo

    // Segunda passagem: Montar o binário
    while (fgets(linha, sizeof(linha), entrada)) {
        if (linha[0] == ';') continue; // Ignorar comentários

        char mnem[10], operando[20];
        int opcode, endereco_operando = 0;

        sscanf(linha, "%s %s", mnem, operando);

        opcode = get_opcode(mnem);
        if (opcode == -1) continue;

        // Resolver operando
        if (isalpha(operando[0])) { // Se for uma label
            endereco_operando = get_endereco_label(operando);
            if (endereco_operando == -1) {
                printf("Erro: Label desconhecida %s\n", operando);
                continue;
            }
        } else { // Operando numérico
            endereco_operando = atoi(operando);
        }

        // Escrever no arquivo binário
        fputc(opcode, saida);
        fputc(endereco_operando, saida);
    }

    return 0; // Sucesso
}
