#include <stdio.h>
#include <stdlib.h>
#include "executor.h"

void exibirUso(char *nomePrograma) {
    printf("Uso: %s <arquivo.bin>\n", nomePrograma);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exibirUso(argv[0]);
    }

    FILE *entrada = fopen(argv[1], "rb");
    if (!entrada) {
        perror("Erro ao abrir o arquivo binário");
        return 1;
    }

    int resultado = executarBinario(entrada);

    fclose(entrada);
    return resultado;
}
