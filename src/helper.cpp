#include "helper.h"

// Check if two expression are equivalent (specifically if they have the same variable name)
bool areEquivalentExpression(SgExpression *expr1, SgExpression *expr2) {
    if (!expr1 || !expr2) {
        return false;
    }

    // Check if both expressions are of the same type
    if (expr1->variantT() != expr2->variantT()) {
        return false;
    }

    // Compare specific types of expressions
    if (auto *varRef1 = isSgVarRefExp(expr1)) {
        auto *varRef2 = isSgVarRefExp(expr2);
        return varRef2 && (varRef1->get_symbol() == varRef2->get_symbol());
    }

    // By default, consider expressions non-equivalent
    return false;
}


// Get the variable name for every variable in an expression
std::set<std::string> getVariableNames(SgExpression *expr) {
    std::set<std::string> variableNames;

    // Traverse the subtree of the expression to find all variable references
    for (auto node : NodeQuery::querySubTree(expr, V_SgVarRefExp)) {
        if (auto *varRef = isSgVarRefExp(node)) {
            // Get the variable's name
            std::string varName = varRef->get_symbol()->get_name().getString();
            variableNames.insert(varName);
        }
    }

    return variableNames;
}


// Perform all the replacements given in the array "replacements" in a given expression
SgExpression* replaceVarsInExpression(SgExpression* expr, const std::unordered_map<std::string, SgExpression*>& replacements) {
    if (!expr) return nullptr;

    // If the expression is a variable reference, check for replacement
    if (auto* varRef = isSgVarRefExp(expr)) {
        std::string varName = varRef->get_symbol()->get_name().getString();
        if (replacements.find(varName) != replacements.end()) {
            return SageInterface::deepCopy(replacements.at(varName));
        }
        return expr; // No replacement needed
    }

    // Handle binary expressions (e.g., a + b, a * b)
    if (auto* binOp = isSgBinaryOp(expr)) {
        binOp->set_lhs_operand(replaceVarsInExpression(binOp->get_lhs_operand(), replacements));
        binOp->set_rhs_operand(replaceVarsInExpression(binOp->get_rhs_operand(), replacements));
        return binOp;
    }

    // Handle unary expressions (e.g., -a, ++a)
    if (auto* unaryOp = isSgUnaryOp(expr)) {
        unaryOp->set_operand(replaceVarsInExpression(unaryOp->get_operand(), replacements));
        return unaryOp;
    }

    // Handle function calls (e.g., foo(a, b))
    if (auto* funcCall = isSgFunctionCallExp(expr)) {
        SgExprListExp* args = funcCall->get_args();
        for (size_t i = 0; i < args->get_expressions().size(); ++i) {
            args->get_expressions()[i] = replaceVarsInExpression(args->get_expressions()[i], replacements);
        }
        return funcCall;
    }

    // Handle conditional expressions (e.g., a ? b : c)
    if (auto* condExpr = isSgConditionalExp(expr)) {
        condExpr->set_conditional_exp(replaceVarsInExpression(condExpr->get_conditional_exp(), replacements));
        condExpr->set_true_exp(replaceVarsInExpression(condExpr->get_true_exp(), replacements));
        condExpr->set_false_exp(replaceVarsInExpression(condExpr->get_false_exp(), replacements));
        return condExpr;
    }

    // Handle other expression types as needed
    return expr; // If the expression doesn't match any case, return it unchanged
}


// Get all the variables in a list of expressions
std::set<std::string> getReferencedVariables(SgExprListExp *args) {
    std::set<std::string> variables;

    for (auto *expr : args->get_expressions()) {
        // Traverse the expression subtree to find all SgVarRefExp nodes
        for (auto *node : NodeQuery::querySubTree(expr, V_SgVarRefExp)) {
            if (auto *varRef = isSgVarRefExp(node)) {
                variables.insert(varRef->get_symbol()->get_name().getString());
            }
        }
    }

    return variables;
}


// Simplify a list of expressions by calling a python function 
std::vector<std::string> simplifyArgumentsWithSymPy(const std::vector<std::string> &args) {
    std::vector<std::string> simplifiedArgs;

    // Prepare the Python command
    std::ostringstream command;
    command << "python3 src/solver.py";
    for (const auto &arg : args) {
        command << " \"" << arg << "\""; // Add each argument as a command-line parameter
    }

    // Open a pipe to the Python command
    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Could not open pipe to Python script.\n";
        return simplifiedArgs;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        simplifiedArgs.emplace_back(buffer);
    }

    // Close the pipe
    pclose(pipe);

    // Trim trailing newlines
    for (auto &arg : simplifiedArgs) {
        arg.erase(arg.find_last_not_of("\n") + 1);
    }

    return simplifiedArgs;
}


