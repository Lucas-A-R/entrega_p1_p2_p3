# Projeto: Lexer em C
Este projeto implementa um analisador léxico (**lexer**) para uma linguagem simples definida em BNF.

/lexer_project
│── grammar.bnf    # Definição da gramática da linguagem
│── test.txt       # Código-fonte de exemplo para teste
│── include/       # Diretório para cabeçalhos
│   │── lexer.h    # Definição das funções do lexer
│   │── tokens.h   # Enumeração dos tipos de tokens
│── src/           # Diretório para implementação
│   │── lexer.c    # Implementação do lexer
│   │── main.c     # Arquivo principal que executa o lexer
│── build/         # Diretório para os arquivos compilados
│── Makefile       # Arquivo Makefile para compilação automatizada


---

## Como Compilar
Se sua IDE suporta `Makefile`, use:
make

Isso gerará o executável `lexer`.

Caso sua IDE **não suporte `Makefile`**, compile manualmente:
gcc -Wall -o lexer src/main.c src/lexer.c

Isso criará um executável chamado `lexer`.

---

## Como Executar
### Método 1: Rodando código direto
Se você está em uma IDE online, pode rodar diretamente **main.c** sem precisar de entrada via terminal. 

### Método 2: Usando Arquivo de Teste
Caso sua IDE permita entrada via **arquivo de texto**, execute com:
./lexer < test.txt

Isso fará com que o lexer leia e analise o código no arquivo `test.txt`.

---

## Exemplo de Saída
Se `test.txt` contiver:
PROGRAMA teste:
inicio
x = 5
y = x + 2
fim

A saída esperada será:
TOKEN PALAVRA: PROGRAMA
TOKEN PALAVRA: teste
TOKEN OPERADOR: :
TOKEN PALAVRA: inicio
TOKEN PALAVRA: x
TOKEN OPERADOR: =
TOKEN NUMERO: 5
TOKEN PALAVRA: y
TOKEN OPERADOR: =
TOKEN PALAVRA: x
TOKEN OPERADOR: +
TOKEN NUMERO: 2
TOKEN PALAVRA: fim

---

## Testes Adicionais
- Modifique `test.txt` para testar diferentes casos, como números grandes, operadores diversos e erros sintáticos.
- Se precisar validar a saída automaticamente, adicione comparações dentro do código C.
- Para futuras expansões, como um **parser**, o lexer pode ser adaptado para estruturar tokens em árvores.

Agora você tem um guia completo para rodar o lexer! Se precisar de ajustes ou quiser adicionar recursos, me avise!
