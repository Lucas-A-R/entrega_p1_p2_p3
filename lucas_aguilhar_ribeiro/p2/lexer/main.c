#include <stdio.h>
#include "lexer.h"

int main() {
    // Código de teste diretamente na string (já que não lemos arquivos)
    const char *codigo =
        "PROGRAMA teste:\n"
        "inicio\n"
        "x = 5\n"
        "y = x + 2\n"
        "z = y * 3\n"
        "fim";

    printf("Análise léxica:\n");
    lexer(codigo); // Chamando a função do lexer

    return 0;
}
