#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Função auxiliar para verificar a conformidade com a gramática BNF.
static int verificarGramatica(const char *linha, ErroParser *erro) {
    if (linha == NULL || strlen(linha) == 0) {
        erro->linha = 0; 
        erro->coluna = 0;
        snprintf(erro->mensagem, sizeof(erro->mensagem), "Linha vazia ou inválida.");
        return 1;
    }

    // Aqui, você deve adicionar regras específicas da gramática BNF.
    // Por exemplo: validar estrutura da linha conforme tokens permitidos.
    // Exemplo fictício:
    if (strncmp(linha, "INSTRUCAO", 9) != 0) {
        erro->linha = 0;
        erro->coluna = 1;
        snprintf(erro->mensagem, sizeof(erro->mensagem), "Linha não reconhecida como instrução válida.");
        return 1;
    }

    return 0; // Sucesso, linha válida.
}

// Processa o arquivo de entrada e gera o código Assembly correspondente no arquivo de saída.
int processarArquivo(FILE *entrada, FILE *saida) {
    char buffer[1024];
    int numeroLinha = 1;
    ErroParser erro;

    while (fgets(buffer, sizeof(buffer), entrada)) {
        // Remove o caractere de nova linha, se existir.
        buffer[strcspn(buffer, "\n")] = '\0';

        // Validação da linha baseada na gramática.
        if (verificarGramatica(buffer, &erro)) {
            erro.linha = numeroLinha;
            fprintf(stderr, "Erro na linha %d: %s\n", numeroLinha, erro.mensagem);
            return 1;
        }

        // Geração de código Assembly para a linha válida.
        fprintf(saida, "ASSEMBLY_%d: %s\n", numeroLinha, buffer); // Placeholder para o Assembly real.

        numeroLinha++;
    }

    return 0; // Sucesso.
}

// Exibe erros de forma amigável.
void exibirErro(ErroParser erro) {
    fprintf(stderr, "Erro na linha %d, coluna %d: %s\n", erro.linha, erro.coluna, erro.mensagem);
}
