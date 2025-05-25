#include "executor.h"
#include <stdlib.h>

// Função que executa o programa binário carregado
int executarBinario(FILE *entrada) {
    unsigned char memoria[TAM_MEMORIA] = {0};
    int total_bytes = 0;
    int c;

    // Carregar o programa binário na memória
    while ((c = fgetc(entrada)) != EOF && total_bytes < TAM_MEMORIA) {
        memoria[total_bytes++] = (unsigned char)c;
    }

    // Inicializar registradores
    int pc = 0; // Program Counter
    int acc = 0; // Acumulador

    // Executar instruções
    while (pc < total_bytes) {
        unsigned char opcode = memoria[pc];
        unsigned char operando = memoria[pc + 1];
        printf("PC: %d | Opcode: 0x%02X | Operando: 0x%02X | ACC: %d\n", pc, opcode, operando, acc);

        switch (opcode) {
            case 0x00: // NOP
                pc += 2;
                break;
            case 0x10: // STA
                if (operando < TAM_MEMORIA) {
                    memoria[operando] = (unsigned char)acc;
                }
                pc += 2;
                break;
            case 0x20: // LDA
                if (operando < TAM_MEMORIA) {
                    acc = memoria[operando];
                }
                pc += 2;
                break;
            case 0x30: // ADD
                if (operando < TAM_MEMORIA) {
                    acc += memoria[operando];
                }
                pc += 2;
                break;
            case 0x40: // OR
                if (operando < TAM_MEMORIA) {
                    acc |= memoria[operando];
                }
                pc += 2;
                break;
            case 0x50: // AND
                if (operando < TAM_MEMORIA) {
                    acc &= memoria[operando];
                }
                pc += 2;
                break;
            case 0x60: // NOT
                acc = ~acc;
                pc += 2;
                break;
            case 0x70: // JMP
                pc = operando;
                break;
            case 0x80: // JN
                if ((signed char)acc < 0)
                    pc = operando;
                else
                    pc += 2;
                break;
            case 0x90: // JZ
                if (acc == 0)
                    pc = operando;
                else
                    pc += 2;
                break;
            case 0xF0: // HLT
                printf("HLT encontrado. Finalizando execução.\n");
                return 0;
            default:
                printf("Instrução desconhecida: 0x%02X na posição %d\n", opcode, pc);
                return 1;
        }
    }

    printf("Execução concluída. Acumulador: %d\n", acc);
    return 0;
}
