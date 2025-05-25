#include <stdio.h>
#include <stdlib.h>
#include "parser.h" // Supondo que este header contenha as funções do parser.

void exibirUso(char *nomePrograma) {
    printf("Uso: %s <arquivo.lpn>\n", nomePrograma);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos está correto.
    if (argc != 2) {
        exibirUso(argv[0]);
    }

    // Abre o arquivo de entrada.
    FILE *entrada = fopen(argv[1], "r");
    if (entrada == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Cria o arquivo de saída.
    FILE *saida = fopen("programa.asm", "w");
    if (saida == NULL) {
        perror("Erro ao criar o arquivo de saída");
        fclose(entrada);
        return 1;
    }

    // Chama o parser para processar o arquivo.
    int resultado = processarArquivo(entrada, saida); // Supondo que "processarArquivo" esteja em parser.c
    if (resultado != 0) {
        printf("Erro: Código inválido no arquivo de entrada.\n");
    } else {
        printf("Arquivo programa.asm gerado com sucesso.\n");
    }

    // Fecha os arquivos.
    fclose(entrada);
    fclose(saida);

    return resultado;
}
