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
