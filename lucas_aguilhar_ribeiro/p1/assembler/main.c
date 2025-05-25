#include <stdio.h>
#include <stdlib.h>
#include "assembler.h" // Inclui o header com as declarações necessárias

void exibirUso(char *nomePrograma) {
    printf("Uso: %s <arquivo.asm>\n", nomePrograma);
    exit(1);
}

int main(int argc, char *argv[]) {
    // Verificar se o número de argumentos está correto
    if (argc != 2) {
        exibirUso(argv[0]);
    }

    // Abrir o arquivo de entrada (.asm)
    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    // Criar o arquivo de saída (.bin)
    FILE *saida = fopen("programa.bin", "wb");
    if (!saida) {
        perror("Erro ao criar o arquivo de saída");
        fclose(entrada);
        return 1;
    }

    // Processar o arquivo Assembly e gerar o binário
    int resultado = processarAssembly(entrada, saida);
    if (resultado != 0) {
        printf("Erro: Falha ao processar o arquivo Assembly.\n");
    } else {
        printf("Arquivo programa.bin gerado com sucesso.\n");
    }

    // Fechar os arquivos
    fclose(entrada);
    fclose(saida);

    return resultado;
}
