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
