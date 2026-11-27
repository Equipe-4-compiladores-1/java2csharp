#include "optimizer.h"

#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <sstream>

namespace {
std::shared_ptr<ExprNode> unwrapParentheses(const std::shared_ptr<ExprNode>& expr) {
    auto parenthesized = std::dynamic_pointer_cast<ParenthesizedExprNode>(expr);
    return parenthesized ? unwrapParentheses(parenthesized->expr) : expr;
}

bool readBoolean(const std::shared_ptr<ExprNode>& expr, bool& value) {
    auto literal = std::dynamic_pointer_cast<LiteralNode>(unwrapParentheses(expr));
    if (!literal || (literal->value != "true" && literal->value != "false")) {
        return false;
    }

    value = literal->value == "true";
    return true;
}

bool readInteger(const std::shared_ptr<ExprNode>& expr, std::int32_t& value) {
    auto literal = std::dynamic_pointer_cast<LiteralNode>(unwrapParentheses(expr));
    if (!literal || literal->value.empty()) {
        return false;
    }

    errno = 0;
    char* end = nullptr;
    const long long parsed = std::strtoll(literal->value.c_str(), &end, 10);
    if (errno == ERANGE || end == literal->value.c_str() || *end != '\0' ||
        parsed < std::numeric_limits<std::int32_t>::min() ||
        parsed > std::numeric_limits<std::int32_t>::max()) {
        return false;
    }

    value = static_cast<std::int32_t>(parsed);
    return true;
}

bool readDouble(const std::shared_ptr<ExprNode>& expr, double& value) {
    auto literal = std::dynamic_pointer_cast<LiteralNode>(unwrapParentheses(expr));
    if (!literal || literal->value.empty()) {
        return false;
    }

    errno = 0;
    char* end = nullptr;
    const double parsed = std::strtod(literal->value.c_str(), &end);
    if (errno == ERANGE || end == literal->value.c_str() || *end != '\0' ||
        !std::isfinite(parsed)) {
        return false;
    }

    value = parsed;
    return true;
}

bool hasFloatingLiteral(const std::shared_ptr<ExprNode>& expr) {
    auto literal = std::dynamic_pointer_cast<LiteralNode>(unwrapParentheses(expr));
    return literal && literal->value.find('.') != std::string::npos;
}

void replaceWithLiteral(std::shared_ptr<ExprNode>& expr, const std::string& value) {
    expr = std::make_shared<LiteralNode>(value);
}

std::string formatDouble(double value) {
    std::ostringstream result;
    result << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
    std::string formatted = result.str();
    if (formatted.find_first_of(".eE") == std::string::npos) {
        formatted += ".0";
    }
    return formatted;
}

bool evaluateInteger(const std::string& op, std::int32_t left, std::int32_t right,
                     std::shared_ptr<ExprNode>& expr) {
    if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
        bool result = false;
        if (op == "==") result = left == right;
        else if (op == "!=") result = left != right;
        else if (op == "<") result = left < right;
        else if (op == "<=") result = left <= right;
        else if (op == ">") result = left > right;
        else result = left >= right;
        replaceWithLiteral(expr, result ? "true" : "false");
        return true;
    }

    std::int64_t result = 0;
    if (op == "+") result = static_cast<std::int64_t>(left) + right;
    else if (op == "-") result = static_cast<std::int64_t>(left) - right;
    else if (op == "*") result = static_cast<std::int64_t>(left) * right;
    else if (op == "/" && right != 0) {
        result = left == std::numeric_limits<std::int32_t>::min() && right == -1
            ? std::numeric_limits<std::int32_t>::min()
            : left / right;
    } else if (op == "%" && right != 0) {
        result = left == std::numeric_limits<std::int32_t>::min() && right == -1
            ? 0
            : left % right;
    }
    else return false;

    const std::uint32_t wrapped = static_cast<std::uint32_t>(result);
    const std::int64_t signedValue = wrapped <=
        static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())
        ? wrapped
        : static_cast<std::int64_t>(wrapped) - (std::int64_t{1} << 32);
    replaceWithLiteral(expr, std::to_string(static_cast<std::int32_t>(signedValue)));
    return true;
}

bool evaluateDouble(const std::string& op, double left, double right,
                    std::shared_ptr<ExprNode>& expr) {
    if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
        bool result = false;
        if (op == "==") result = left == right;
        else if (op == "!=") result = left != right;
        else if (op == "<") result = left < right;
        else if (op == "<=") result = left <= right;
        else if (op == ">") result = left > right;
        else result = left >= right;
        replaceWithLiteral(expr, result ? "true" : "false");
        return true;
    }

    double result = 0.0;
    if (op == "+") result = left + right;
    else if (op == "-") result = left - right;
    else if (op == "*") result = left * right;
    else if (op == "/" && right != 0.0) result = left / right;
    else if (op == "%" && right != 0.0) result = std::fmod(left, right);
    else return false;

    if (!std::isfinite(result)) {
        return false;
    }

    replaceWithLiteral(expr, formatDouble(result));
    return true;
}

void foldExpression(std::shared_ptr<ExprNode>& expr);

void foldExpressions(std::vector<std::shared_ptr<ExprNode>>& expressions) {
    for (auto& expression : expressions) {
        foldExpression(expression);
    }
}

void foldExpression(std::shared_ptr<ExprNode>& expr) {
    if (!expr) return;

    if (auto parenthesized = std::dynamic_pointer_cast<ParenthesizedExprNode>(expr)) {
        foldExpression(parenthesized->expr);
        bool booleanValue = false;
        std::int32_t integerValue = 0;
        double doubleValue = 0.0;
        if (readBoolean(parenthesized->expr, booleanValue) ||
            readInteger(parenthesized->expr, integerValue) ||
            readDouble(parenthesized->expr, doubleValue)) {
            expr = parenthesized->expr;
        }
        return;
    }

    if (auto unary = std::dynamic_pointer_cast<UnaryExprNode>(expr)) {
        foldExpression(unary->expr);
        bool booleanValue = false;
        if (unary->op == "!" && readBoolean(unary->expr, booleanValue)) {
            replaceWithLiteral(expr, booleanValue ? "false" : "true");
            return;
        }

        std::int32_t integerValue = 0;
        if (unary->op == "-" && readInteger(unary->expr, integerValue) &&
            integerValue != std::numeric_limits<std::int32_t>::min()) {
            replaceWithLiteral(expr, std::to_string(-integerValue));
            return;
        }

        double doubleValue = 0.0;
        if (unary->op == "-" && hasFloatingLiteral(unary->expr) &&
            readDouble(unary->expr, doubleValue)) {
            replaceWithLiteral(expr, formatDouble(-doubleValue));
        }
        return;
    }

    if (auto binary = std::dynamic_pointer_cast<BinaryExprNode>(expr)) {
        foldExpression(binary->left);
        foldExpression(binary->right);

        bool leftBoolean = false;
        bool rightBoolean = false;
        if (readBoolean(binary->left, leftBoolean) && readBoolean(binary->right, rightBoolean)) {
            if (binary->op == "&&") {
                replaceWithLiteral(expr, leftBoolean && rightBoolean ? "true" : "false");
            } else if (binary->op == "||") {
                replaceWithLiteral(expr, leftBoolean || rightBoolean ? "true" : "false");
            } else if (binary->op == "==") {
                replaceWithLiteral(expr, leftBoolean == rightBoolean ? "true" : "false");
            } else if (binary->op == "!=") {
                replaceWithLiteral(expr, leftBoolean != rightBoolean ? "true" : "false");
            }
            return;
        }

        std::int32_t leftInteger = 0;
        std::int32_t rightInteger = 0;
        if (readInteger(binary->left, leftInteger) && readInteger(binary->right, rightInteger)) {
            if (evaluateInteger(binary->op, leftInteger, rightInteger, expr)) return;
        }

        double leftDouble = 0.0;
        double rightDouble = 0.0;
        if ((hasFloatingLiteral(binary->left) || hasFloatingLiteral(binary->right)) &&
            readDouble(binary->left, leftDouble) && readDouble(binary->right, rightDouble)) {
            evaluateDouble(binary->op, leftDouble, rightDouble, expr);
        }
        return;
    }

    if (auto call = std::dynamic_pointer_cast<MethodCallExprNode>(expr)) {
        foldExpressions(call->args);
    } else if (auto update = std::dynamic_pointer_cast<NumericUpdateExprNode>(expr)) {
        foldExpression(update->expr);
    } else if (auto allocation = std::dynamic_pointer_cast<ArrayAllocNode>(expr)) {
        foldExpression(allocation->sizeExpr);
    } else if (auto access = std::dynamic_pointer_cast<ArrayAccessNode>(expr)) {
        foldExpression(access->indexExpr);
    }
}

void foldStatement(std::shared_ptr<StmtNode>& stmt);

void foldStatements(std::vector<std::shared_ptr<StmtNode>>& statements) {
    for (auto& statement : statements) {
        foldStatement(statement);
    }
}

void foldStatement(std::shared_ptr<StmtNode>& stmt) {
    if (!stmt) return;

    if (auto declaration = std::dynamic_pointer_cast<VarDeclNode>(stmt)) {
        foldExpression(declaration->initExpr);
    } else if (auto declaration = std::dynamic_pointer_cast<ArrayDeclNode>(stmt)) {
        foldExpression(declaration->initExpr);
    } else if (auto assignment = std::dynamic_pointer_cast<AssignStmtNode>(stmt)) {
        foldExpression(assignment->expr);
    } else if (auto assignment = std::dynamic_pointer_cast<ArrayAssignNode>(stmt)) {
        foldExpression(assignment->indexExpr);
        foldExpression(assignment->valueExpr);
    } else if (auto result = std::dynamic_pointer_cast<ReturnStmtNode>(stmt)) {
        foldExpression(result->expr);
    } else if (auto output = std::dynamic_pointer_cast<PrintStmtNode>(stmt)) {
        foldExpression(output->expr);
    } else if (auto output = std::dynamic_pointer_cast<PrintfStmtNode>(stmt)) {
        foldExpressions(output->args);
    } else if (auto call = std::dynamic_pointer_cast<MethodCallStmtNode>(stmt)) {
        foldExpressions(call->args);
    } else if (auto expression = std::dynamic_pointer_cast<ExprStmtNode>(stmt)) {
        foldExpression(expression->expr);
    } else if (auto conditional = std::dynamic_pointer_cast<IfStmtNode>(stmt)) {
        foldExpression(conditional->condition);
        foldStatements(conditional->thenBody);
        foldStatements(conditional->elseBody);
    } else if (auto loop = std::dynamic_pointer_cast<WhileStmtNode>(stmt)) {
        foldExpression(loop->condition);
        foldStatements(loop->body);
    } else if (auto loop = std::dynamic_pointer_cast<ForStmtNode>(stmt)) {
        foldStatement(loop->init);
        foldExpression(loop->condition);
        foldStatement(loop->increment);
        foldStatements(loop->body);
    }
}
}

void foldConstants(const std::shared_ptr<ClassDeclNode>& root) {
    if (!root) return;
    for (const auto& function : root->functions) {
        if (function) foldStatements(function->body);
    }
}
