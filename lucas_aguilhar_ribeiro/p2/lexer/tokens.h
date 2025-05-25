#ifndef TOKENS_H
#define TOKENS_H

// Enumeração dos tipos de tokens que o lexer pode reconhecer
typedef enum {
    TOKEN_PROGRAMA,   // Palavra-chave "PROGRAMA"
    TOKEN_INICIO,     // Palavra-chave "inicio"
    TOKEN_FIM,        // Palavra-chave "fim"
    TOKEN_VARIAVEL,   // Variável identificada na linguagem
    TOKEN_ATRIBUICAO, // Atribuição de variável (ex: x = 5)
    TOKEN_EXPRESSAO,  // Expressões matemáticas (ex: y = x + 2)
    TOKEN_TERMO,      // Termo dentro de uma expressão (palavra ou número)
    TOKEN_OPERADOR,   // Operadores matemáticos (+, -, *, /)
    TOKEN_NUMERO,     // Números inteiros
    TOKEN_PALAVRA,    // Identificadores (nomes de variáveis, labels)
    TOKEN_DESCONHECIDO // Qualquer coisa que não se encaixe nos tokens definidos
} TokenType;

#endif // TOKENS_H
