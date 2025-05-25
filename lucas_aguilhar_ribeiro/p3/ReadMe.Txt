/*
================================================================================
README.txt
================================================================================

Projeto: Gerador e Interpretador Brainfuck
--------------------------------------------
Este projeto consiste em uma ferramenta que converte um programa escrito 
em uma sintaxe simplificada para atribuição (no formato <variável> = <expressão>)
em código Brainfuck. Posteriormente, o código Brainfuck gerado é interpretado 
para imprimir o resultado de cada expressão na tela.

Formato de saída: Cada linha da entrada gera uma saída no formato:
    <variável>/= <resultado>

Estrutura do Projeto:
   - main.c     : Arquivo principal que configura o ambiente, lê as 
                  instruções do usuário e chama as funções de geração 
                  e interpretação.
   - bfc.c/.h   : Responsáveis pelo parsing de expressões, avaliação e 
                  geração de código Brainfuck.
   - bfe.c/.h   : Implementação do interpretador Brainfuck.

Como Funciona:
   1. O usuário insere linhas no formato "variável = expressão". A entrada
      é finalizada com uma linha vazia.
   2. O gerador de código analisa cada linha, avalia a expressão e gera uma 
      saída no formato "<variável>/= <resultado>" convertido para Brainfuck.
   3. O interpretador percorre e executa o código Brainfuck gerado, exibindo o 
      resultado.

Requisitos, Compilação e Ambiente:
   - **OnlineGDB:** Este projeto foi originalmente baseado no ambiente 
     online (https://www.onlinegdb.com/), e algumas dinâmicas podem variar.
   - **Linux:** Este projeto pode ser compilado e executado normalmente 
     em um ambiente Linux. Certifique-se de possuir um compilador C (por exemplo,
     o gcc) instalado.

Exemplo de comando para compilar no Linux:
   
   gcc -o brainfuck main.c bfc.c bfe.c -Wall -std=c99

Para executar:
   
   ./brainfuck

Considerações:
   - O programa utiliza buffers de tamanho fixo (por exemplo, program[4096], 
     line[256] e bf_code[8192]). Para entradas muito grandes, isso pode causar 
     estouro de buffer.
   - O parsing não possui uma checagem completa de erros; entradas mal formatadas 
     podem produzir resultados inesperados.
   - Futuramente, melhorias podem incluir buffers dinâmicos, otimizações na 
     concatenação de strings e suporte a mais operações ou parênteses nas expressões.

================================================================================
*/

/*
================================================================================
bfc.h
================================================================================
*/

#ifndef BFC_H
#define BFC_H

// Gera código Brainfuck a partir do texto do programa, que deve conter
// instruções no formato:
//    <variável> = <expressão>
// Cada linha gera uma saída no formato "<variável>/= <resultado>\n"
// Todas as operações são avaliadas em tempo de execução.
void generate_brainfuck(const char *programText, char *bf_code);

#endif

/*
================================================================================
bfc.c
================================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>
#include "bfc.h"

#define MAX_VARS 100  // Número máximo de variáveis na tabela de símbolos

typedef struct {
    char name[64];
    int value;
} Variable;

static Variable symtab[MAX_VARS];
static int symCount = 0;

// --- Função de Tabela de Símbolos --- //
static void setVariable(const char *name, int value) {
    for (int i = 0; i < symCount; i++) {
        if (strcmp(symtab[i].name, name) == 0) {
            symtab[i].value = value;
            return;
        }
    }
    if (symCount < MAX_VARS) {
        strcpy(symtab[symCount].name, name);
        symtab[symCount].value = value;
        symCount++;
    }
}

static int getVariable(const char *name) {
    for (int i = 0; i < symCount; i++) {
        if (strcmp(symtab[i].name, name) == 0)
            return symtab[i].value;
    }
    return 0;  // Se não definida, retorna 0
}

// --- Funções de Parsing de Expressões --- //

static void skipSpaces(const char **s) {
    while (**s && isspace((unsigned char)**s))
        (*s)++;
}

// <Factor> ::= <Número> | <Palavra>
// Se for uma palavra, retorna o valor armazenado na tabela de símbolos.
static int parseFactor(const char **s) {
    skipSpaces(s);
    int result = 0;
    if (isalpha((unsigned char)**s)) {
        char word[64];
        int i = 0;
        while (**s && isalpha((unsigned char)**s) && i < 63) {
            word[i++] = **s;
            (*s)++;
        }
        word[i] = '\0';
        result = getVariable(word);
    } else if (isdigit((unsigned char)**s)) {
        while (**s && isdigit((unsigned char)**s)) {
            result = result * 10 + (**s - '0');
            (*s)++;
        }
    }
    return result;
}

// <Termo> ::= <Factor> { ("*" | "/") <Factor> }
static int parseTerm(const char **s) {
    int value = parseFactor(s);
    skipSpaces(s);
    while (**s == '*' || **s == '/') {
        char op = **s;
        (*s)++;
        int factor = parseFactor(s);
        if (op == '*')
            value *= factor;
        else {
            if (factor != 0)
                value /= factor;
            else
                value = 0;  // Divisão por zero: tratamos como zero
        }
        skipSpaces(s);
    }
    return value;
}

// <Expressão> ::= <Termo> { ("+" | "-") <Termo> }
static int parseExpression(const char **s) {
    int value = parseTerm(s);
    skipSpaces(s);
    while (**s == '+' || **s == '-') {
        char op = **s;
        (*s)++;
        int term = parseTerm(s);
        if (op == '+')
            value += term;
        else
            value -= term;
        skipSpaces(s);
    }
    return value;
}

// --- Função para Gerar Literal em Brainfuck com UTF-8 --- //
static void generateLiteralUTF8(const char *str, char *bf_code) {
    // Converte a string multibyte para wide string
    wchar_t wide[256];
    int count = mbstowcs(wide, str, 256);
    if (count < 0) {
        size_t len = strlen(str);
        for (size_t i = 0; i < len; i++) {
            unsigned char c = (unsigned char)str[i];
            strcat(bf_code, "[-]");
            for (int j = 0; j < c; j++) {
                strcat(bf_code, "+");
            }
            strcat(bf_code, ".");
            strcat(bf_code, ">");
        }
    } else {
        char mb[8];  // Buffer para caractere multibyte (UTF-8 pode ter até 4 bytes)
        for (int i = 0; i < count; i++) {
            int len = wctomb(mb, wide[i]);
            if (len < 0)
                continue;
            for (int j = 0; j < len; j++) {
                unsigned char c = (unsigned char)mb[j];
                strcat(bf_code, "[-]");
                for (int k = 0; k < c; k++) {
                    strcat(bf_code, "+");
                }
                strcat(bf_code, ".");
                strcat(bf_code, ">");
            }
        }
    }
}

// --- Função Principal de Geração de Código Brainfuck --- //
// Cada linha de 'programText' deve estar no formato:
//    <variável> = <expressão>
//
// Para cada linha, calcula o resultado, atualiza a tabela de símbolos e gera
// uma linha de saída no formato "<variável>/= <resultado>\n" via Brainfuck.
void generate_brainfuck(const char *programText, char *bf_code) {
    bf_code[0] = '\0';
    symCount = 0;  // Reinicia a tabela de símbolos

    // Duplica o texto para uso com strtok (que modifica a string)
    char *buffer = strdup(programText);
    if (!buffer)
        return;
    
    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        if (strlen(line) == 0) {
            line = strtok(NULL, "\n");
            continue;
        }
        const char *s = line;
        skipSpaces(&s);
        char varName[64] = "";
        int i = 0;
        // Lê o nome da variável (somente letras)
        while (*s && isalpha((unsigned char)*s) && i < 63) {
            varName[i++] = *s;
            s++;
        }
        varName[i] = '\0';
        skipSpaces(&s);
        if (*s == '=') {
            s++;  // pula o '='
            skipSpaces(&s);
            int result = parseExpression(&s);
            setVariable(varName, result);
            char outputLine[128];
            sprintf(outputLine, "%s/= %d", varName, result);
            generateLiteralUTF8(outputLine, bf_code + strlen(bf_code));
            generateLiteralUTF8("\n", bf_code + strlen(bf_code));
        }
        line = strtok(NULL, "\n");
    }
    free(buffer);
}

/*
================================================================================
bfe.h
================================================================================
*/

#ifndef BFE_H
#define BFE_H

// Interpreta o código Brainfuck gerado.
void interpret_brainfuck(const char *bf_code);

#endif

/*
================================================================================
bfe.c
================================================================================
*/

#include <stdio.h>
#include <string.h>
#include "bfe.h"

#define MEMORY_SIZE 30000

void interpret_brainfuck(const char *bf_code) {
    char memory[MEMORY_SIZE] = {0};
    char *ptr = memory;
    int codeLen = (int)strlen(bf_code);
    for (int i = 0; i < codeLen; i++) {
        char cmd = bf_code[i];
        switch (cmd) {
            case '>': 
                ptr++; 
                break;
            case '<': 
                ptr--; 
                break;
            case '+': 
                (*ptr)++; 
                break;
            case '-': 
                (*ptr)--; 
                break;
            case '.': 
                putchar(*ptr); 
                break;
            case ',': 
                // Não implementado
                break;
            case '[':
                if (*ptr == 0) {
                    int loop = 1;
                    while (loop > 0) {
                        i++;
                        if (bf_code[i] == '[')
                            loop++;
                        else if (bf_code[i] == ']')
                            loop--;
                    }
                }
                break;
            case ']':
                if (*ptr != 0) {
                    int loop = 1;
                    while (loop > 0) {
                        i--;
                        if (bf_code[i] == '[')
                            loop--;
                        else if (bf_code[i] == ']')
                            loop++;
                    }
                }
                break;
            default:
                // Outros caracteres são ignorados.
                break;
        }
    }
}

/*
================================================================================
main.c
================================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "bfc.h"
#include "bfe.h"

int main() {
    // Configura a locale para UTF-8
    setlocale(LC_ALL, "");

    char program[4096] = "";
    char line[256];

    printf("Digite as instruções (um por linha no formato <variável> = <expressão>).\n");
    printf("Para terminar, digite uma linha vazia.\n");

    // Lê linhas até uma linha em branco ser digitada
    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(line, "\n") == 0)
            break;
        strcat(program, line);
    }
    
    char bf_code[8192] = "";
    generate_brainfuck(program, bf_code);

    printf("\nCódigo Brainfuck Gerado:\n%s\n", bf_code);
    printf("Executando Brainfuck...\n");
    
    interpret_brainfuck(bf_code);
    printf("\n");
    
    return 0;
}
