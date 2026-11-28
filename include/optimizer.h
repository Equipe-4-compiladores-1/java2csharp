#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"
#include <memory>

void foldConstants(const std::shared_ptr<ClassDeclNode>& root);

#endif
