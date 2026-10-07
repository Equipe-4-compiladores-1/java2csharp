cmake_minimum_required(VERSION 3.12)
project(java2csharp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Diretórios de include
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

# Gerar Parser e Lexer
BISON_TARGET(Parser grammar/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
FLEX_TARGET(Lexer grammar/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# Fontes do projeto
set(SOURCES
        src/main.cpp
        src/codegen.cpp
        src/symbol_table.cpp
        src/semantic_analyzer.cpp
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Lexer_OUTPUTS}
)

add_executable(java2csharp ${SOURCES})
# Configuração de testes
# Lista de testes (nome do arquivo sem o .java) que devem ser pulados
set(TESTS_TO_SKIP
    "ForClassico"
    "WhileSimples"
    "ForCondicaoNaoBooleana"
    "ForVariavelForaEscopo"
    "WhileCondicaoNaoBooleana"
)

enable_testing()
# 1. Sucesso
file(GLOB SUCCESS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/input/success/*.java")
foreach(INPUT_FILE IN LISTS SUCCESS_FILES)
    get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
    if(NOT TEST_NAME IN_LIST TESTS_TO_SKIP)
        add_test(NAME "Compila_${TEST_NAME}" COMMAND java2csharp ${INPUT_FILE})
        add_test(NAME "Valida_${TEST_NAME}" COMMAND diff -u -w --strip-trailing-cr Saida.cs ${CMAKE_CURRENT_SOURCE_DIR}/tests/expected/${TEST_NAME}_expected.cs)
        set_tests_properties("Valida_${TEST_NAME}" PROPERTIES DEPENDS "Compila_${TEST_NAME}")
    endif()
endforeach()

# 2. Falha: Léxico
file(GLOB FAILURE_LEXICAL "${CMAKE_CURRENT_SOURCE_DIR}/tests/input/failure/lexical/*.java")
foreach(INPUT_FILE IN LISTS FAILURE_LEXICAL)
    get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
    if(NOT TEST_NAME IN_LIST TESTS_TO_SKIP)
        add_test(NAME "Falha_Lexico_${TEST_NAME}" COMMAND java2csharp ${INPUT_FILE})
        set_tests_properties("Falha_Lexico_${TEST_NAME}" PROPERTIES  PASS_REGULAR_EXPRESSION "Erro Lexico")
    endif()
endforeach()

# 3. Falha: Sintaxe
file(GLOB FAILURE_SYNTAX "${CMAKE_CURRENT_SOURCE_DIR}/tests/input/failure/sintax/*.java")
foreach(INPUT_FILE IN LISTS FAILURE_SYNTAX)
    get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
    if(NOT TEST_NAME IN_LIST TESTS_TO_SKIP)
        add_test(NAME "Falha_Sintaxe_${TEST_NAME}" COMMAND java2csharp ${INPUT_FILE})
        set_tests_properties("Falha_Sintaxe_${TEST_NAME}" PROPERTIES  PASS_REGULAR_EXPRESSION "Erro Sintatico")
    endif()
endforeach()

# 4. Falha: Semântica
file(GLOB FAILURE_SEMANTIC "${CMAKE_CURRENT_SOURCE_DIR}/tests/input/failure/semantic/*.java")
foreach(INPUT_FILE IN LISTS FAILURE_SEMANTIC)
    get_filename_component(TEST_NAME ${INPUT_FILE} NAME_WE)
    if(NOT TEST_NAME IN_LIST TESTS_TO_SKIP)
        add_test(NAME "Falha_Semantica_${TEST_NAME}" COMMAND java2csharp ${INPUT_FILE})
        set_tests_properties("Falha_Semantica_${TEST_NAME}" PROPERTIES  PASS_REGULAR_EXPRESSION "Erro Semantico")
    endif()
endforeach()

# Fim da configuração de testes
# Configuração do relatório de cobertura de teste
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
# Gerar relatório de cobertura de teste no terminal
add_custom_target(coverage
    COMMAND mkdir -p coverage_report
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --filter "\".*/src/.*\"" --filter "\".*/include/.*\"" -e ".*/usr/.*"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Gerando relatorio de cobertura resumido no terminal..."
)

# Gerar Html de cobertura de teste
add_custom_target(coverage_html
    COMMAND mkdir -p coverage_report
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --filter "\".*/src/.*\"" --filter "\".*/include/.*\"" -e ".*/usr/.*" --html --html-details -o coverage_report/index.html
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Gerando relatorio de cobertura visual em HTML..."
)
# Fim da Configuração do relatório de cobertura de teste