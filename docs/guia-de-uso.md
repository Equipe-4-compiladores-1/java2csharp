# Guia de Compilação e Uso

Este guia apresenta o passo a passo para configurar o ambiente, compilar o transpilador e executar as suas primeiras conversões de Java para C#.

## :material-hammer-wrench: Pré-requisitos

Para compilar o projeto, você precisará de uma cadeia de ferramentas C++ padrão e dos geradores de analisadores léxicos e sintáticos. 

Certifique-se de ter os seguintes pacotes instalados no seu sistema:
* **GCC** (ou Clang) com suporte a C++17
* **CMake** (versão 3.10 ou superior)
* **Flex** (Lexer)
* **Bison** (Parser)

!!! tip "Dica de Instalação (Linux)"
    Em distribuições baseadas em Arch Linux, você pode garantir todas as dependências com um único comando:
    `sudo pacman -S base-devel cmake flex bison`

!!! tip "Dica de Instalação (Ubuntu/WSL)"
    No Ubuntu executado pelo WSL, instale as ferramentas com:
    `sudo apt update && sudo apt install -y cmake flex bison g++`

## :material-cogs: Como Compilar

O projeto utiliza o CMake para gerenciar o build de forma isolada (Out-of-source build). Execute os comandos abaixo na raiz do repositório:

```bash
# 1. Crie e acesse o diretório de build
mkdir build && cd build

# 2. Gere os arquivos de configuração do CMake
cmake ..

# 3. Compile o projeto
make
```

### Compilação pelo WSL

Não reutilize o diretório `build` criado pelo Windows: o `CMakeCache.txt` guarda caminhos e o gerador da plataforma que o criou. Use um diretório separado no WSL:

```bash
cmake -S . -B build-wsl -G "Unix Makefiles"
cmake --build build-wsl
```

Se o diretório `build-wsl` também tiver sido configurado anteriormente com outra plataforma, remova-o e execute os comandos novamente:

```bash
rm -rf build-wsl
cmake -S . -B build-wsl -G "Unix Makefiles"
cmake --build build-wsl
```

O executável estará em `build-wsl/java2csharp`.

Após o término da compilação, o executável (ex: `java2csharp`) estará disponível dentro do diretório `build`.

## :material-play-circle: Como Executar

Para rodar o transpilador, basta passar o arquivo `.java` de origem como argumento para o executável.

```bash
./java2csharp ../tests/input/MeuPrograma.java
```

Também é possível digitar o código diretamente no terminal executando o transpilador sem argumentos (pressione `Ctrl + D` para encerrar a entrada e compilar):

```bash
./java2csharp
```

**Resultado Esperado:**
O programa fará a leitura, exibirá o código gerado no terminal e criará o arquivo de saída `Saida.cs` contendo o código transpilado. Caso haja algum erro léxico, sintático ou semântico, a execução será interrompida, o erro será apontado e o arquivo não será gerado.

!!! note "Arquivo de saída"
    O nome do arquivo de saída é fixo: o código é sempre gravado em `Saida.cs`, no diretório de onde o executável foi chamado (sobrescrevendo um arquivo anterior com o mesmo nome). O executável não aceita a flag `-o` — basta informar o arquivo `.java` de entrada.

Erros sintáticos informam a linha, o termo exato que não pôde ser compilado e os tokens esperados pelo parser.

```text
[Erro Sintatico] na linha 3: termo '+' nao pode ser compilado: syntax error, unexpected '+'
```

Erros semânticos são descritos na página [Análise Semântica](analise-semantica.md).

## :material-test-tube: Executando os Testes

Após compilar, execute a suíte de testes de regressão a partir do diretório `build`:

```bash
make test
```
