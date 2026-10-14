#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

enum class SymbolKind {
    VARIABLE, 
    PARAMETER,
    FUNCTION,
};

struct SymbolInfo {
    std::string name;
    std::string type;
    SymbolKind kind;
    bool isInitialized = false;
    int line = 0;
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, SymbolInfo>> scopes;
public:
    SymbolTable();

    void enterScope();
    void exitScope();

    bool defineSymbol(const std::string& name, const SymbolInfo& info);
    bool defineSymbol(const std::string&name, const std::string& type, SymbolKind kind = SymbolKind::VARIABLE, bool initialized = false, int line = 0);

    bool resolveSymbol(const std::string& name, SymbolInfo& outInfo);

    bool isDeclaredInCurrentScope(const std::string& name) const;
};

#endif