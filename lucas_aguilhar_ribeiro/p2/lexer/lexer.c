#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

// Implementação do lexer que percorre o código-fonte e identifica tokens
void lexer(const char *codigo) {
    int i = 0;
    while (codigo[i] != '\0') {
        // Ignorar espaços em branco e caracteres de nova linha
        if (isspace(codigo[i])) {
            i++;
            continue;
        }

        // Detectar números inteiros
        if (isdigit(codigo[i])) {
            printf("TOKEN NUMERO: ");
            while (isdigit(codigo[i])) {
                printf("%c", codigo[i]);
                i++;
            }
            printf("\n");
        }
        // Detectar palavras-chave e identificadores
        else if (isalpha(codigo[i])) {
            printf("TOKEN PALAVRA: ");
            while (isalpha(codigo[i])) {
                printf("%c", codigo[i]);
                i++;
            }
            printf("\n");
        }
        // Detectar operadores matemáticos e de atribuição
        else if (strchr("+-*/=", codigo[i])) {
            printf("TOKEN OPERADOR: %c\n", codigo[i]);
            i++;
        }
        // Caso um caractere desconhecido seja encontrado
        else {
            printf("TOKEN DESCONHECIDO: %c\n", codigo[i]);
            i++;
        }
    }
}
