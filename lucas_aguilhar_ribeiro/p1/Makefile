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