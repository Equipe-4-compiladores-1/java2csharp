#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <sstream>
#include <vector>
#include "ast.h"
#include "codegen.h"
#include "semantic_analyzer.h"

extern FILE* yyin;
extern int yyparse();
extern std::vector<std::shared_ptr<FunctionDeclNode>> programFunctions;
extern std::shared_ptr<ClassDeclNode> rootNode;

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
            return 1;
        }
        yyin = file;
    } else {
        std::cout << "Lendo da entrada padrao (Ctrl+D para encerrar)..." << std::endl;
    }

    if (yyparse() == 0) {

        SemanticAnalyzerVisitor semantic_analyzer;
        if (rootNode) {
            rootNode->accept(&semantic_analyzer);
        }
        if (semantic_analyzer.hasErrors()) {
            std::cerr << "\nCompilação abortada devido a erros semânticos.\n" << std::endl;
            return 1;
        }

        // Redireciona o cout para um buffer de memória virtual
        std::stringstream buffer;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());

        // Inicia a geração de código
        CSharpCodegenVisitor codegen;
        if (rootNode) {
            codegen.generateProgram(rootNode); 
        }

        // Restaura o cout IMEDIATAMENTE para o terminal real
        std::cout.rdbuf(oldCoutBuffer);

        // Captura o código gerado em uma string
        std::string codigoCsharp = buffer.str();

        // Grava fisicamente no arquivo
        std::ofstream outFile("Saida.cs");
        outFile << codigoCsharp;
        outFile.close();

        // Imprime no terminal para a equipe visualizar
        std::cout << "\n========== CODIGO GERADO ==========\n";
        std::cout << codigoCsharp;
        std::cout << "===================================\n";
        std::cout << "Arquivo 'Saida.cs' gerado com sucesso.\n";
    } else {
        std::cerr << "Falha na compilacao." << std::endl;
        return 1;
    }

    return 0;
}
