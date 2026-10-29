# Java2CSharp — Transpilador Java para C#

![C++](https://img.shields.io/badge/C++-00599C?logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)
![GNU](https://img.shields.io/badge/Flex_&_Bison-A42E2B?logo=gnu&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)

## Visão geral

O **Java2CSharp** é um transpilador desenvolvido para a disciplina de **Compiladores 1**. Ele foca no subconjunto imperativo e procedural da linguagem **Java**, validando a estrutura do código e convertendo-o para uma representação equivalente em **C#**.

O projeto utiliza um fluxo de compilação em etapas desacopladas, validando regras de escopo e gerando código C# através de uma classe estática wrapper.

## Capacidades

- **Análise Léxica:** Conversão do código fonte `.java` em fluxo de tokens utilizando Flex.
- **Análise Sintática e AST:** Validação gramatical e construção da Árvore de Sintaxe Abstrata (AST) via Bison.
- **Análise Semântica:** Uso de Tabela de Símbolos para validar existência de variáveis, escopos de bloco aninhados, redefinições, conflitos de parâmetros e compatibilidade de tipos (atribuições, retornos, operações e condições).
- **Estruturas de Controle:** Desvios condicionais (`if/else`) e laços de repetição (`while` e `for`).
- **Operadores:** Aritméticos (`+`, `-`, `*`, `/`), relacionais (`==`, `!=`, `<`, `<=`, `>`, `>=`) e incremento/decremento pós-fixo (`++`, `--`).
- **Geração de Código (Visitor Pattern):** Emissão de código C# percorrendo a AST hierárquica (comandos, expressões e funções).
- **Cobertura Automatizada:** Integração com `gcovr` para relatórios detalhados de cobertura de testes no terminal e em HTML.

## Módulos

| Módulo | Responsabilidade | Guia |
| --- | --- | --- |
| **Gramáticas** | Especificações léxicas (`lexer.l`) e regras sintáticas/AST (`parser.y`). | `grammar/` |
| **Cabeçalhos** | Definições C++ (`.h`) da AST, Visitor de geração de código e Tabela de Símbolos. | `include/` |
| **Fonte** | Implementação dos módulos da compilação e ponto de entrada da CLI (`main.cpp`). | `src/` |
| **Testes** | Suíte de validação contendo arquivos imperativos de entrada e saídas esperadas. | `tests/` |
| **Build** | Automação e orquestração da compilação de Flex, Bison e C++. | `CMakeLists.txt` |

## Comece aqui

Pré-requisitos: **GCC / Clang** (C++17+), **Flex** (v2.6+), **Bison** (v3.0+) e **CMake** (v3.12+).

```bash
git clone https://github.com/Equipe-4-compiladores-1/java2csharp
cd java2csharp

# Crie o diretório de build e gere os arquivos de compilação
cmake -B build
cmake --build build

# Execute o transpilador passando o arquivo de entrada
cd build/ && ./java2csharp ../tests/input/success/IfElse_01.java

# ou para digitar no terminal (CTRL + D para encerrar e compilar)
cd build/ && ./java2csharp
```

O código C# gerado é exibido no terminal e gravado automaticamente no arquivo `Saida.cs`, criado no diretório de onde o executável foi chamado (não há opção para escolher outro nome de saída).

## Validação e métricas

A garantia de qualidade é feita através de uma suíte de testes de regressão e relatórios de cobertura do código-fonte gerados pelo `gcovr`.

### Executando os testes

Após compilar o projeto na pasta `build`, execute a suíte de testes com:

```bash
make test

```

### Relatório de Cobertura

Para medir o alcance do código testado, certifique-se de ter o `gcovr` instalado (`sudo pacman -S gcovr` no Arch Linux ou `sudo apt install gcovr` em distros Debian-based).

Na pasta `build`, escolha o formato do relatório:

* **Terminal:** `make coverage`
* **HTML:** `make coverage_html` (O relatório ficará disponível em `build/coverage_report/index.html`)

## Referências e contribuição

1. Crie uma branch a partir de `main` (ex: `feature/nova-regra-semantica`).
2. Adicione casos de teste na pasta `tests/input/` para validar a nova funcionalidade.
3. Certifique-se de que `make test` e a análise de cobertura sejam executados com sucesso.
4. Abra um Pull Request detalhando a alteração na gramática ou na geração de código.
