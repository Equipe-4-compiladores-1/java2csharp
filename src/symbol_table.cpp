#include "symbol_table.h"

SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes.push_back(std::unordered_map<std::string, SymbolInfo>());
}   

void SymbolTable::exitScope() {
    if (scopes.size() > 1) {
        scopes.pop_back();
    }
}

bool SymbolTable::defineSymbol(const std::string& name, const SymbolInfo& info) {
    if (scopes.empty()) return false;

    auto& currentScope = scopes.back();

    if (currentScope.find(name) != currentScope.end()) {
        return false;
    }

    currentScope[name] = info;
    return true;
}

bool SymbolTable::defineSymbol(const std::string& name, const std::string& type, SymbolKind kind, bool initialized, int line) {
    return defineSymbol(name, SymbolInfo{name, type, kind, initialized, line});
}

bool SymbolTable::resolveSymbol(const std::string& name, SymbolInfo& outInfo) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            outInfo = found->second;
            return true;
        }
    }
    return false;
}

bool SymbolTable::isDeclaredInCurrentScope(const std::string& name) const {
    if (scopes.empty()) return false;
    return scopes.back().find(name) != scopes.back().end();
}