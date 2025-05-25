#include <stdio.h>
#include <string.h>
#include "bfe.h"

#define MEMORY_SIZE 30000

void interpret_brainfuck(const char *bf_code) {
    char memory[MEMORY_SIZE] = {0};
    char *ptr = memory;
    int codeLen = (int)strlen(bf_code);
    for (int i = 0; i < codeLen; i++) {
        char cmd = bf_code[i];
        switch (cmd) {
            case '>': 
                ptr++; 
                break;
            case '<': 
                ptr--; 
                break;
            case '+': 
                (*ptr)++; 
                break;
            case '-': 
                (*ptr)--; 
                break;
            case '.': 
                putchar(*ptr); 
                break;
            case ',': 
                // Não implementado
                break;
            case '[':
                if (*ptr == 0) {
                    int loop = 1;
                    while (loop > 0) {
                        i++;
                        if (bf_code[i] == '[')
                            loop++;
                        else if (bf_code[i] == ']')
                            loop--;
                    }
                }
                break;
            case ']':
                if (*ptr != 0) {
                    int loop = 1;
                    while (loop > 0) {
                        i--;
                        if (bf_code[i] == '[')
                            loop--;
                        else if (bf_code[i] == ']')
                            loop++;
                    }
                }
                break;
            default:
                // Outros caracteres são ignorados.
                break;
        }
    }
}
