[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/3XHcMjDV)
Lucas Aguilhar Ribeiro

## Descrição
Este projeto implementa um compilador, assembler e executor para interpretar e executar programas na linguagem Neander.

## Requisitos
- Ambiente Linux 64 bits
- GCC instalado
- Opcional: Make para utilizar o script Makefile incluído.
## Estrutura do Projeto
- **`compilador/`**: Contém o compilador que traduz `.lpn` para `.asm`.
- **`assembler/`**: Contém o assembler que traduz `.asm` para `.bin`.
- **`executor/`**: Contém o executor que interpreta e executa o `.bin`.

## Instruções para Compilação
1. Use o script Makefile para compilar os três executáveis:
   ```bash
   make


---

### **2. Script Makefile**
Crie um Makefile que automatize a compilação dos três programas:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g

all: compilador assembler executor

compilador: compilador/main.c compilador/parser.c
    $(CC) $(CFLAGS) -o compilador compilador/main.c compilador/parser.c

assembler: assembler/main.c assembler/assembler.c
    $(CC) $(CFLAGS) -o assembler assembler/main.c assembler/assembler.c

executor: executor/main.c executor/executor.c
    $(CC) $(CFLAGS) -o executor executor/main.c executor/executor.c

clean:
    rm -f compilador assembler executor
s
