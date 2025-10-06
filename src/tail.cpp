#include "helper.h"
#include "tail.h"

// Wrap function in a while loop thats always true
void wrapFunc(SgFunctionDefinition *funcDef) {
    // Get the current function body
    SgBasicBlock *funcBody = funcDef->get_body();

    // Create a deep copy of the current body (to reuse inside the while loop)
    SgBasicBlock *loopBody = SageInterface::deepCopy(funcBody);

    // Create a new `while(1)` loop with the copied body
    SgWhileStmt *whileLoop = SageBuilder::buildWhileStmt(
        SageBuilder::buildBoolValExp(true), loopBody);

    // Replace the contents of the function body with the `while(1)` loop
    funcBody->get_statements().clear(); // Clear existing statements
    funcBody->append_statement(whileLoop);
}


// Transforms a tail recursive function
void transformTR(SgFunctionDefinition *funcDef) {
    auto *funcDecl = funcDef->get_declaration();
    std::string funcName = funcDecl->get_name().getString();
    
    // Traverse all function calls
    for (auto i : NodeQuery::querySubTree(funcDef, V_SgFunctionCallExp)) {
        if (auto *funcCall = isSgFunctionCallExp(i)) {
            // Recursive check
            if (funcCall->get_function()->unparseToString() == funcName) {
                trTailStmt(funcCall, funcDef);
            }
        }
    }
}



// Transform the recusive tail call
void trTailStmt(SgFunctionCallExp *funcCall, SgFunctionDefinition *funcDef) {
    auto *funcDecl = funcDef->get_declaration();
    SgInitializedNamePtrList &paramList = funcDecl->get_parameterList()->get_args();

    // Map parameters to their new values
    std::vector<SgStatement *> updateStatements;
    std::vector<SgStatement *> saveStatements;
    std::unordered_map<std::string, SgExpression *> paramUpdates;
    std::set<std::string> changedVars;

    // Compare arguments with parameters
    SgExprListExp *args = funcCall->get_args();
    for (size_t i = 0; i < args->get_expressions().size(); ++i) {
        auto *arg = args->get_expressions()[i];
        auto *param = isSgInitializedName(paramList[i]);
        
        // Check if the parameter's value is updated
        auto *paramRef = SageBuilder::buildVarRefExp(param);
        if (!areEquivalentExpression(arg, paramRef)) {
            // Add an assignment statement to update the parameter
            paramUpdates[param->get_name().getString()] = arg;
            changedVars.insert(param->get_name().getString());
        }
    }
    
    // Handle swaps and assignments
    std::unordered_map<std::string, SgExpression *> tempVars;
    for (const auto &[paramName, updateExpr] : paramUpdates) {
        auto *param = SageBuilder::buildVarRefExp(paramName, funcDef);

        std::set<std::string> argVars = getVariableNames(updateExpr);

        std::set<std::string> otherVars = changedVars;
        otherVars.erase(paramName);

        std::set<std::string> commonVars;
        std::set_intersection(argVars.begin(), argVars.end(),
                        otherVars.begin(), otherVars.end(),
                        std::inserter(commonVars, commonVars.begin()));

        // Check if it's a swap
        if (!commonVars.empty()) {
            // If there are overlaps, we need temporary variables for intermediate assignments
            for (const auto &varName : commonVars) {
                if (tempVars.find(varName) == tempVars.end()) {
                    // Create a new temporary variable for the overlapping parameter
                    std::string tempVarName = "temp_" + varName;
                    auto *tempVarDecl = SageBuilder::buildVariableDeclaration(
                        tempVarName, param->get_type(), nullptr, funcDef->get_body());

                    // Add the temporary variable to the block and map
                    saveStatements.push_back(tempVarDecl);
                    tempVars[varName] = SageBuilder::buildVarRefExp(tempVarDecl);
                }
            }

            // Perform the swap using temporary variables
            for (const auto &varName : commonVars) {
                auto *tempVarRef = tempVars[varName];
                auto *assignToTemp = SageBuilder::buildAssignStatement(tempVarRef, SageBuilder::buildVarRefExp(varName, funcDef));
                saveStatements.push_back(assignToTemp);
            }

            // Replace variables in updateExpr with their temporary counterparts
            SgExpression *updatedExpr = SageInterface::deepCopy(updateExpr);


            // Replace variables in the expression
            updatedExpr = replaceVarsInExpression(updatedExpr, tempVars);

            // Assign the updated expression to the parameter
            auto *assignToParam = SageBuilder::buildAssignStatement(param, updatedExpr);
            updateStatements.push_back(assignToParam);

        } else {
            // If there are no overlaps, perform a direct assignment
            auto *assignStmt = SageBuilder::buildAssignStatement(param, SageInterface::deepCopy(updateExpr));
            updateStatements.push_back(assignStmt);
        }
    }

    
    // Find the parent statement of the function call
    SgNode *parent = funcCall->get_parent();
    auto *parentStmt = isSgStatement(parent);
    
    // Replace the return statement if the call is in one
    if (auto *retStmt = isSgReturnStmt(parentStmt)) {
        for (auto *stmt : saveStatements) {
            SageInterface::insertStatementBefore(retStmt, stmt);
        }

        for (auto *stmt : updateStatements) {
            SageInterface::insertStatementBefore(retStmt, stmt);
        }

        SageInterface::replaceStatement(retStmt, SageBuilder::buildContinueStmt());
    } else {
        for (auto *stmt : saveStatements) {
            SageInterface::insertStatementBefore(parentStmt, stmt);
        }
        
        // Otherwise, insert the block before the parent statement
        for (auto *stmt : updateStatements) {
            SageInterface::insertStatementBefore(parentStmt, stmt);
        }

        SageInterface::removeStatement(parentStmt);
    }
}