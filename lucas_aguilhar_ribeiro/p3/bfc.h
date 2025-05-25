#ifndef BFC_H
#define BFC_H

// Gera código Brainfuck a partir do texto do programa, que deve conter
// instruções no formato:
//    <variável> = <expressão>
// Cada linha gera uma saída no formato "<variável>/= <resultado>\n"
// Todas as operações são avaliadas em tempo de execução.
void generate_brainfuck(const char *programText, char *bf_code);

#endif
