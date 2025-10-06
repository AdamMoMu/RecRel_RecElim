#include "transformation_divconq.h"

// Helper funciton for finding number of iterations for parametes with a division operator in recursive calls
SgFunctionDeclaration* createIterFindFunction_dc(SgGlobal *globalScope) {
    // Step 1: Create the parameter list
    SgFunctionParameterList *paramList = SageBuilder::buildFunctionParameterList();
    paramList->append_arg(SageBuilder::buildInitializedName(
        "n", SageBuilder::buildIntType()));
    paramList->append_arg(SageBuilder::buildInitializedName(
        "k", SageBuilder::buildIntType()));

    // Step 2: Create the function declaration
    SgFunctionDeclaration *iterFuncDecl = SageBuilder::buildDefiningFunctionDeclaration(
        "div_iter_find",
        SageBuilder::buildIntType(),
        paramList,
        globalScope);

    // Step 3: Create the function body
    SgBasicBlock *funcBody = SageBuilder::buildBasicBlock();

    funcBody->set_parent(iterFuncDecl->get_definition());

    SgVariableDeclaration *tempVarDecl = SageBuilder::buildVariableDeclaration(
        "iter", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)),
        funcBody);
    funcBody->append_statement(tempVarDecl);


    SgExpression *loopCondition = SageBuilder::buildGreaterThanOp(
        SageBuilder::buildVarRefExp("n", funcBody),
        SageBuilder::buildSubtractOp(SageBuilder::buildVarRefExp("k", funcBody),SageBuilder::buildIntVal(1)));

    

    SgExpression *divAssignLHS = SageBuilder::buildVarRefExp("n", funcBody);
    SgExpression *divAssignRHS = SageBuilder::buildDivideOp(SageBuilder::buildVarRefExp("n", funcBody), SageBuilder::buildVarRefExp("k", funcBody));
    SgStatement *divAssign = SageBuilder::buildAssignStatement(divAssignLHS, divAssignRHS);

    SgBasicBlock *whileBody = SageBuilder::buildBasicBlock(divAssign);

    SgStatement *incrementIter = SageBuilder::buildAssignStatement(
        SageBuilder::buildVarRefExp("iter", funcBody), 
        SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("iter", funcBody),SageBuilder::buildIntVal(1)));
    
    whileBody->append_statement(incrementIter);

    SgWhileStmt *whileLoop = SageBuilder::buildWhileStmt(loopCondition, whileBody);


    // Step 4: Append the for-loop to the function body
    funcBody->append_statement(whileLoop);

    SgStatement *returnStmt = SageBuilder::buildReturnStmt(SageBuilder::buildVarRefExp("iter", funcBody));
    funcBody->append_statement(returnStmt);

    // Step 5: Set the function body to the defining declaration
    iterFuncDecl->get_definition()->set_body(funcBody);

    // Step 6: Prepend the function declaration to the global scope
    SageInterface::prependStatement(iterFuncDecl, globalScope);
    
    return iterFuncDecl;
}


// A helper function to find a power
// Creates a function pow(n,k) that finds n**k more efficiently
SgFunctionDeclaration* createPowFunction_dc(SgGlobal *globalScope) {
    // Step 1: Create the parameter list
    SgFunctionParameterList *paramList = SageBuilder::buildFunctionParameterList();
    paramList->append_arg(SageBuilder::buildInitializedName(
        "n", SageBuilder::buildIntType()));
    paramList->append_arg(SageBuilder::buildInitializedName(
        "k", SageBuilder::buildIntType()));

    // Step 2: Create the function declaration
    SgFunctionDeclaration *powFuncDecl = SageBuilder::buildDefiningFunctionDeclaration(
        "pow",
        SageBuilder::buildIntType(),
        paramList,
        globalScope);

    // Step 3: Create the function body
    SgBasicBlock *funcBody = SageBuilder::buildBasicBlock();

    funcBody->set_parent(powFuncDecl->get_definition());

    SgVariableDeclaration *tempVarDecl = SageBuilder::buildVariableDeclaration(
        "prod", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(1)),
        funcBody);
    funcBody->append_statement(tempVarDecl);

    // for (int i = 1; i < n; ++i) { arr[i - 1] = arr[i]; }
    SgVariableDeclaration *loopVarDecl = SageBuilder::buildVariableDeclaration(
        "i", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)),
        funcBody);

    SgExpression *loopCondition = SageBuilder::buildLessThanOp(
        SageBuilder::buildVarRefExp("i", funcBody),
        SageBuilder::buildVarRefExp("k", funcBody));
    SgExpression *incrementExpr = SageBuilder::buildPlusPlusOp(
        SageBuilder::buildVarRefExp("i", funcBody), SgUnaryOp::prefix);

    SgExpression *powerAssignLHS = SageBuilder::buildVarRefExp("prod", funcBody);
    SgExpression *powerAssignRHS = SageBuilder::buildMultiplyOp(SageBuilder::buildVarRefExp("prod", funcBody), SageBuilder::buildVarRefExp("n", funcBody));
    SgStatement *powerAssign = SageBuilder::buildAssignStatement(powerAssignLHS, powerAssignRHS);

    SgBasicBlock *forLoopBody = SageBuilder::buildBasicBlock(powerAssign);
    SgForStatement *forLoop = SageBuilder::buildForStatement(
        loopVarDecl, SageBuilder::buildExprStatement(loopCondition), incrementExpr, forLoopBody);

    // Step 4: Append the for-loop to the function body
    funcBody->append_statement(forLoop);

    SgStatement *returnStmt = SageBuilder::buildReturnStmt(SageBuilder::buildVarRefExp("prod", funcBody));
    funcBody->append_statement(returnStmt);


    // Step 5: Set the function body to the defining declaration
    powFuncDecl->get_definition()->set_body(funcBody);

    // Step 6: Prepend the function declaration to the global scope
    SageInterface::prependStatement(powFuncDecl, globalScope);

    return powFuncDecl;
}



// Helper function: Call Python script to order the expression
std::string orderExpressionWithSymPy(const std::string &expr) {
    std::string command = "python3 src/simplifier.py \"" + expr + "\"";

    // Open a pipe to run the command
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Unable to open pipe for Python script\n";
        return "";
    }

    // Read the output from the Python script
    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    // Remove trailing newlines and whitespace
    result.erase(result.find_last_not_of(" \n\r\t") + 1);

    return result;
}


// Recursively check every expression and replace a specific variable with a new one
SgExpression* replaceSubExpressions(SgExpression *expr,
                                    const std::string &lhs,
                                    const std::string &param,
                                    SgBasicBlock *scope) {

    if (auto *binOp = isSgBinaryOp(expr)) {
        // Check left child
        SgExpression *lhsChild = binOp->get_lhs_operand();
        std::string lhsTrimmed = lhs;
        lhsTrimmed.erase(remove_if(lhsTrimmed.begin(), lhsTrimmed.end(), isspace), lhsTrimmed.end());

        if (lhsChild) {
            std::string orderedLHS = orderExpressionWithSymPy(lhsChild->unparseToString());
            orderedLHS.erase(remove_if(orderedLHS.begin(), orderedLHS.end(), isspace), orderedLHS.end());
            if (orderedLHS == lhsTrimmed) {
                // Return the replacement expression
                SgExpression *modifiedChild = SageBuilder::buildVarRefExp(param, scope);
                SgBinaryOp* returnOp = SageInterface::deepCopy(binOp);
                returnOp->set_lhs_operand(modifiedChild);
                return returnOp; // Return updated parent
            } 
            else {
                // Recursively check and replace in the left child
                SgExpression *modifiedChild = replaceSubExpressions(lhsChild, lhs, param, scope);
                
                if (modifiedChild) {
                    SgBinaryOp* returnOp = SageInterface::deepCopy(binOp);
                    returnOp->set_lhs_operand(modifiedChild);
                    return returnOp; // Return updated parent
                }
            }
        }

        // Check right child
        SgExpression *rhsChild = binOp->get_rhs_operand();
        if (rhsChild) {
            std::string orderedRHS = orderExpressionWithSymPy(rhsChild->unparseToString());
            orderedRHS.erase(remove_if(orderedRHS.begin(), orderedRHS.end(), isspace), orderedRHS.end());
            if (orderedRHS == lhsTrimmed) {
                SgExpression *modifiedChild = SageBuilder::buildVarRefExp(param, scope);
                SgBinaryOp* returnOp = SageInterface::deepCopy(binOp);
                returnOp->set_rhs_operand(modifiedChild);
                return returnOp; // Return updated parent
            } else {
                // Recursively check and replace in the right child
                SgExpression *modifiedChild = replaceSubExpressions(rhsChild, lhs, param, scope);
                if (modifiedChild) {
                    SgBinaryOp* returnOp = SageInterface::deepCopy(binOp);
                    returnOp->set_rhs_operand(modifiedChild);
                    return returnOp; // Return updated parent
                }
            }
        }
    }

    return nullptr; // No replacement found
}


// Make sure all the variable are correctly updated
void transformMathExpressions(SgBasicBlock *block,
                              const std::string &param,
                              const std::string &param_o,
                              const std::string &lhs,
                              SgBasicBlock *scope, int baseVal) {
    struct MathExpressionTransformer : public AstSimpleProcessing {
        std::string param, param_o, lhs;
        SgBasicBlock *scope;
        int baseVal;

        MathExpressionTransformer(const std::string &p, const std::string &po, const std::string &l, SgBasicBlock *s, int bv)
            : param(p), param_o(po), lhs(l), scope(s), baseVal(bv) {}
        // TODO add 0 base case
        void visit(SgNode *node) override {
            auto *binOp = isSgBinaryOp(node);
            if (binOp && !(isSgBinaryOp(node->get_parent())) && (isSgAddOp(node) || isSgSubtractOp(node) || isSgDivideOp(node) || isSgMultiplyOp(node))) {
                // Step 1: Check if the entire expression matches
                std::string orderedExpr = orderExpressionWithSymPy(binOp->unparseToString());
                std::string lhsTrimmed = lhs, orderedExprTrimmed = orderedExpr;
                orderedExprTrimmed.erase(remove_if(orderedExprTrimmed.begin(), orderedExprTrimmed.end(), isspace), orderedExprTrimmed.end());
                lhsTrimmed.erase(remove_if(lhsTrimmed.begin(), lhsTrimmed.end(), isspace), lhsTrimmed.end());

                if (orderedExprTrimmed == lhsTrimmed) {
                    // Replace the entire expression
                    SgExpression *minExpr = SageBuilder::buildSubtractOp(SageBuilder::buildVarRefExp(param, scope),
                                                         SageBuilder::buildIntVal(1 - baseVal));

                    SageInterface::replaceExpression(binOp, minExpr, true);
                } else {
                    // Step 2: Check subexpressions
                    SgExpression* modifiedExpr = replaceSubExpressions(binOp, lhs, param, scope);

                    if (modifiedExpr) {
                        // If a child was modified
                        SgExpression *minExpr = SageBuilder::buildSubtractOp(SageInterface::deepCopy(modifiedExpr),
                            SageBuilder::buildIntVal(baseVal));

                        SageInterface::replaceExpression(isSgExpression(binOp), isSgExpression(minExpr), true);
                    }
                }
            }
        }
    };

    // Apply the transformation
    MathExpressionTransformer transformer(param, param_o, lhs, scope, baseVal);
    transformer.traverse(block, preorder);
}



// Ensure global scope has necessary includes
void ensureGlobalIncludes_dc(SgSourceFile *sourceFile) {
    ROSE_ASSERT(sourceFile != nullptr);

    // Insert headers at the top of the file
    SageInterface::insertHeader(sourceFile, "math.h", PreprocessingInfo::before, false);
    SageInterface::insertHeader(sourceFile, "cmath", PreprocessingInfo::before, false);
    SageInterface::insertHeader(sourceFile, "algorithm", PreprocessingInfo::before, false);
}


// For error checking
void printFunctionSymbolTable_dc(SgFunctionDefinition *funcDef) {
    if (!funcDef) {
        std::cerr << "Error: Null function definition provided.\n";
        return;
    }

    // Retrieve the function body
    SgBasicBlock *body = isSgBasicBlock(funcDef->get_body());
    if (!body) {
        std::cerr << "Error: Function body is not a valid SgBasicBlock.\n";
        return;
    }

    // Retrieve the symbol table of the function's body
    SgSymbolTable *symbolTable = body->get_symbol_table();
    if (!symbolTable) {
        std::cerr << "Error: Symbol table not found in the function body.\n";
        return;
    }

    symbolTable->print();

    std::cout << "\n";

    for (auto sym : symbolTable->get_symbols())
    {
        std::cout << sym->unparseToString() << '\n';
    }
}


// Copy the Symbol Table for ROSE
void copySymbolTable_dc(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock) {
    if (!sourceBlock || !targetBlock) {
        std::cerr << "Error: Null block provided for symbol table copy.\n";
        return;
    }

    // Get the symbol table from the source block
    SgSymbolTable *sourceTable = sourceBlock->get_symbol_table();
    if (!sourceTable) {
        std::cerr << "Error: Source block does not have a symbol table.\n";
        return;
    }

    // Ensure the target block has a valid symbol table
    SgSymbolTable *targetTable = targetBlock->get_symbol_table();
    if (!targetTable) {
        std::cerr << "Error: Target block does not have a symbol table.\n";
        return;
    }

    // Iterate through the symbols in the source symbol table
    const std::set<SgNode *> &symbolSet = sourceTable->get_symbols();
    for (SgNode *node : symbolSet) {
        if (auto *symbol = isSgSymbol(node)) {
            const std::string &name = symbol->get_name().getString();

            // Insert the symbol into the target symbol table
            targetTable->insert(name, symbol);
        }
    }

    std::cout << "Symbol table successfully copied from source to target block.\n";
}


// Creates a varaible
void createVariableFromMap_dc(const std::string &param,
                                   const std::unordered_map<std::string, std::string> &lhsToSizeParam,
                                   SgBasicBlock *scope) {
    // Step 1: Search the map to see if 'param' is a value
    std::string equation;
    for (const auto &[key, value] : lhsToSizeParam) {
        if (value == param) {
            equation = key;  // Found the key (equation)
            break;
        }
    }

    if (equation.empty()) {
        return;
    }

    // Step 2: Parse the equation into an SgExpression
    auto [negativeVar, positiveVar] = analyzeLHSWithSymPy(equation);
    SgExpression *rhsExpr = SageBuilder::buildSubtractOp(SageBuilder::buildVarRefExp(positiveVar, scope), SageBuilder::buildVarRefExp(negativeVar, scope)); // Simplified parsing for now

    // Build the variable declaration 'int param = <rhsExpr>;'
    SgName varName(param);
    SgName varName2(param + "_o");
    SgType *intType = SageBuilder::buildIntType();
    SgType *intType2 = SageBuilder::buildIntType();

    // Create an assignment initializer for the variable
    SgAssignInitializer *initializer = SageBuilder::buildAssignInitializer(rhsExpr, intType);
    SgAssignInitializer *initializer2 = SageBuilder::buildAssignInitializer(rhsExpr, intType);

    // Build the variable declaration
    SgVariableDeclaration *varDecl =
        SageBuilder::buildVariableDeclaration(varName, intType, initializer, scope);
    SgVariableDeclaration *varDecl2 =
        SageBuilder::buildVariableDeclaration(varName2, intType2, initializer2, scope);
            
    scope->append_statement(varDecl);
    scope->append_statement(varDecl2);
}



// The main function responsible for transforming and creating the new function body
void createNewFunctionBody_dc(SgFunctionDefinition *funcDef,
                                    int stackSize,
                                    const std::unordered_map<std::string, int> &basisMap,
                                    const std::vector<int> &newMapping,
                                    const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> &paramAnalysisMap,
                                    const std::unordered_map<SgFunctionCallExp*, int> &callToValueMap,
                                    const std::vector<SgExpression *> &baseCaseConditions,
                                    std::unordered_map<std::string, std::string> lhsToSizeParam, 
                                    const std::unordered_map<std::string, int> lhsToBaseValue) {
    
    // Step 1: Intialize new body  
    SgBasicBlock *funcBody = funcDef->get_body();
    SgBasicBlock *newBody = SageBuilder::buildBasicBlock();

    newBody->set_parent(funcDef);
    funcDef->set_body(newBody);


    // Step 2: Generate num_iter variables and statements for 'sub' and 'div'
    // These variable hold the total number of iteration associated with each variable
    // The variable are divided based on updates with division and subtraction
    std::vector<std::string> subVarNames, divVarNames;

    bool div_exist = false;
    for (const auto &[param, analysis] : paramAnalysisMap) {
        // Variable that are recusively called with subtraction
        if (analysis.first == "sub") {
            std::string varName = "num_iter_" + param;
            auto *stmt = SageBuilder::buildVariableDeclaration(
                varName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                    SageBuilder::buildDivideOp(SageBuilder::buildVarRefExp(param, newBody), SageBuilder::buildIntVal(basisMap.at(param)))
                ), newBody);
            newBody->append_statement(stmt);
            subVarNames.push_back(varName);
        } 
        // Variable that are recusively called with division
        else if (analysis.first == "div") {
            div_exist = true;
            createVariableFromMap_dc(param, lhsToSizeParam, newBody);
            std::string varName = "num_iter_" + param;
            auto *stmt = SageBuilder::buildVariableDeclaration(
                varName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                        SageBuilder::buildFunctionCallExp("div_iter_find", SageBuilder::buildIntType(),
                            SageBuilder::buildExprListExp(SageBuilder::buildVarRefExp(param, newBody),
                                    SageBuilder::buildIntVal(basisMap.at(param))), 
                            newBody
                    )
                ), newBody);
            newBody->append_statement(stmt);
            divVarNames.push_back(varName);
        }
    }


    // Step 2.5 : If any parameters used division add some extra checks
    if (div_exist) {
        SgGlobal *globalScope = SageInterface::getGlobalScope(funcDef);
        createPowFunction_dc(globalScope);
        createIterFindFunction_dc(globalScope);
    }

    // Add all neccessary library includes
    SgSourceFile *sourceFile = SageInterface::getEnclosingNode<SgSourceFile>(funcDef);
    ensureGlobalIncludes_dc(sourceFile);

    // Step 3: Define num_iter_sub and num_iter_div
    // This adds statements to calculate the number of iteration for each set
    defineNumIterSubAndDiv_dc(newBody, subVarNames, divVarNames);

    // Add minimum calculation logic for num_iter_sub, num_iter_div
    defineNumIterLogic_dc(newBody, basisMap, subVarNames, divVarNames, funcDef, stackSize, baseCaseConditions, funcBody);

    // Step 4: Replace Recursive Calls
    deleteRecursiveCalls_dc(funcBody, callToValueMap);

    // Step 5: Replace Return Statements 
    // TODO extract modified vars
    deleteReturnStatements_dc(funcBody);
    
    copySymbolTable_dc(funcBody, newBody);

    // Step 6: Add for-loop for num_iter updates and insert modified function body
    // Add difference to modified vars
    createForLoops_dc(funcDef, funcBody, basisMap, subVarNames, divVarNames, lhsToSizeParam, lhsToBaseValue);

}




// Step 3: Define num_iter_sub and num_iter_div
void defineNumIterSubAndDiv_dc(SgBasicBlock *newBody,
                            const std::vector<std::string> &subVarNames,
                            const std::vector<std::string> &divVarNames) {

    // Step 3.1: Define num_iter_sub as the minimum of sub variables
    if (!subVarNames.empty()) {
        // Build a list of variables for std::min
        SgExprListExp *minSubList = SageBuilder::buildExprListExp();
        int count = 0;
        for (const auto &varName : subVarNames) {
            count++;
            minSubList->append_expression(SageBuilder::buildVarRefExp(varName, newBody));
        }

        if (count > 1) {
            // Build the function call: std::min
            SgFunctionCallExp *minSubCall = SageBuilder::buildFunctionCallExp(
                "std::min", SageBuilder::buildIntType(), minSubList, newBody);

            // Declare and initialize num_iter_sub
            SgVariableDeclaration *numIterSubDecl = SageBuilder::buildVariableDeclaration(
                "num_iter_sub", SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(minSubCall), newBody);

            // Append the declaration to the new body
            newBody->append_statement(numIterSubDecl);
        }
        else {

            // Declare and initialize num_iter_sub
            SgVariableDeclaration *numIterSubDecl = SageBuilder::buildVariableDeclaration(
                "num_iter_sub", SageBuilder::buildIntType(), nullptr, newBody);

            // Append the declaration to the new body
            newBody->append_statement(numIterSubDecl);
                
            SgAssignOp *assignNum = SageBuilder::buildAssignOp(
                SageBuilder::buildVarRefExp("num_iter_sub", newBody),
                SageBuilder::buildVarRefExp(subVarNames[0], newBody));

            newBody->append_statement(SageBuilder::buildExprStatement(assignNum));
        }
    }

    // Step 3.2: Define num_iter_div as the minimum of div variables
    if (!divVarNames.empty()) {
        SgExprListExp *minDivList = SageBuilder::buildExprListExp();

        int count = 0;
        for (const auto &varName : divVarNames) {
            count++;
            minDivList->append_expression(SageBuilder::buildVarRefExp(varName, newBody));
        }

        if (count > 1) {
            SgFunctionCallExp *minDivCall = SageBuilder::buildFunctionCallExp(
                "std::min", SageBuilder::buildIntType(), minDivList, newBody);

            SgVariableDeclaration *numIterDivDecl = SageBuilder::buildVariableDeclaration(
                "num_iter_div", SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(minDivCall), newBody);

            newBody->append_statement(numIterDivDecl);
        }

        else {
            // Declare and initialize num_iter_sub
            SgVariableDeclaration *numIterDivDecl = SageBuilder::buildVariableDeclaration(
                "num_iter_div", SageBuilder::buildIntType(), nullptr, newBody);

            // Append the declaration to the new body
            newBody->append_statement(numIterDivDecl);
                
            SgAssignOp *assignNum = SageBuilder::buildAssignOp(
                SageBuilder::buildVarRefExp("num_iter_div", newBody),
                SageBuilder::buildVarRefExp(divVarNames[0], newBody));

            newBody->append_statement(SageBuilder::buildExprStatement(assignNum));
        }
    }
}




// Define num_iter logic based on conditions
void defineNumIterLogic_dc(SgBasicBlock *newBody,
                        const std::unordered_map<std::string, int> &basisMap,
                        const std::vector<std::string> &subVarNames,
                        const std::vector<std::string> &divVarNames,
                        SgFunctionDefinition *funcDef,
                        int stackSize,
                        const std::vector<SgExpression *> &baseCaseConditions,
                        SgBasicBlock *oldBody) {


    SgVariableDeclaration *numIterDecl = SageBuilder::buildVariableDeclaration(
        "num_iter", SageBuilder::buildIntType(), nullptr, newBody);
    newBody->append_statement(numIterDecl);

    SgBasicBlock *modifiedBaseCase;
    std::string prefix = "num_iter_";

    // Only sub exists
    if (!subVarNames.empty() && divVarNames.empty()) {
        SgAssignOp *assignNumIter = SageBuilder::buildAssignOp(
            SageBuilder::buildVarRefExp("num_iter", newBody),
            SageBuilder::buildVarRefExp("num_iter_sub", newBody));
        newBody->append_statement(SageBuilder::buildExprStatement(assignNumIter));

        // Update parameters: param = param - num_iter * basisMap[param]
        for (const auto &param : subVarNames) {
            std::string param_name = param.substr(prefix.length());
            SgExpression *updateExpr = SageBuilder::buildSubtractOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildMultiplyOp(
                    SageBuilder::buildVarRefExp("num_iter", newBody),
                    SageBuilder::buildIntVal(basisMap.at(param_name))));
            newBody->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));
        }
    }

    // Only div exists
    else if (subVarNames.empty() && !divVarNames.empty()) {
        SgGlobal *globalScope = SageInterface::getGlobalScope(funcDef);

        SgAssignOp *assignNumIter = SageBuilder::buildAssignOp(
            SageBuilder::buildVarRefExp("num_iter", newBody),
            SageBuilder::buildVarRefExp("num_iter_div", newBody));
        newBody->append_statement(SageBuilder::buildExprStatement(assignNumIter));

        // Update parameters: param = param / pow(basisMap[param], num_iter)
        for (const auto &param : divVarNames) {
            std::string param_name = param.substr(prefix.length());

            SgExpression *updateExpr = SageBuilder::buildDivideOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildFunctionCallExp(
                    "pow", SageBuilder::buildDoubleType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("num_iter", newBody), SageBuilder::buildIntVal(1))
                    ), newBody));

            SgFunctionCallExp *maxFuncCall = SageBuilder::buildFunctionCallExp(
                "std::max", SageBuilder::buildIntType(),
                SageBuilder::buildExprListExp(updateExpr, SageBuilder::buildIntVal(1)), newBody);

            newBody->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), maxFuncCall));
        }

    
    }

    // Both sub and div exist
    else if (!subVarNames.empty() && !divVarNames.empty()) {
        SgGlobal *globalScope = SageInterface::getGlobalScope(funcDef);
        
        // Compare num_iter_sub and num_iter_div
        SgIfStmt *compareStmt = SageBuilder::buildIfStmt(
            SageBuilder::buildLessThanOp(
                SageBuilder::buildVarRefExp("num_iter_sub", newBody),
                SageBuilder::buildVarRefExp("num_iter_div", newBody)),
            SageBuilder::buildBasicBlock(), // If true: num_iter = num_iter_sub
            SageBuilder::buildBasicBlock()  // Else: num_iter = num_iter_div
        );

        // If block: num_iter = num_iter_sub, update sub parameters
        SgBasicBlock *ifBlock = isSgBasicBlock(compareStmt->get_true_body());
        ifBlock->append_statement(SageBuilder::buildAssignStatement(
            SageBuilder::buildVarRefExp("num_iter", newBody),
            SageBuilder::buildVarRefExp("num_iter_sub", newBody)));

        for (const auto &param : subVarNames) {
            std::string param_name = param.substr(prefix.length());
            SgExpression *updateExpr = SageBuilder::buildSubtractOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildMultiplyOp(
                    SageBuilder::buildVarRefExp("num_iter", newBody),
                    SageBuilder::buildIntVal(basisMap.at(param_name))));
            ifBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));
        }

        for (const auto &param : divVarNames) {
            std::string param_name = param.substr(prefix.length());
            SgExpression *updateExpr = SageBuilder::buildDivideOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildFunctionCallExp(
                    "pow", SageBuilder::buildDoubleType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("num_iter", newBody), SageBuilder::buildIntVal(1))
                    ), newBody));

            
            SgFunctionCallExp *maxFuncCall = SageBuilder::buildFunctionCallExp(
                "std::max", SageBuilder::buildIntType(),
                SageBuilder::buildExprListExp(updateExpr, SageBuilder::buildIntVal(1)), newBody);

            ifBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), maxFuncCall));
        }

        // Else block: num_iter = num_iter_div, update div parameters
        SgBasicBlock *elseBlock = isSgBasicBlock(compareStmt->get_false_body());
        elseBlock->append_statement(SageBuilder::buildAssignStatement(
            SageBuilder::buildVarRefExp("num_iter", newBody),
            SageBuilder::buildVarRefExp("num_iter_div", newBody)));


        for (const auto &param : subVarNames) {
            std::string param_name = param.substr(prefix.length());
            SgExpression *updateExpr = SageBuilder::buildSubtractOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildMultiplyOp(
                    SageBuilder::buildVarRefExp("num_iter", newBody),
                    SageBuilder::buildIntVal(basisMap.at(param_name))));
            elseBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));

        }

        for (const auto &param : divVarNames) {
            std::string param_name = param.substr(prefix.length());
            
            SgExpression *updateExpr = SageBuilder::buildDivideOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildFunctionCallExp(
                    "pow", SageBuilder::buildDoubleType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("num_iter", newBody), SageBuilder::buildIntVal(1))
                    ), newBody));

            SgFunctionCallExp *maxFuncCall = SageBuilder::buildFunctionCallExp(
                "std::max", SageBuilder::buildIntType(),
                SageBuilder::buildExprListExp(updateExpr, SageBuilder::buildIntVal(1)), newBody);

            elseBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), maxFuncCall));
        }
        

        // Append the if-else statement
        newBody->append_statement(compareStmt);
    }
}



void deleteReturnStatements_dc(SgBasicBlock *funcBody) {
    struct ReturnReplacer : public AstSimpleProcessing {

        ReturnReplacer() {}

        void visit(SgNode *node) override {
            if (auto *returnStmt = isSgReturnStmt(node)) {
                // Step 4: Add continue statement
                SgStatement *continueStmt = SageBuilder::buildContinueStmt();

                // Step 5: Replace the return statement with the sequence
                SageInterface::replaceStatement(returnStmt, continueStmt);
            }
        }
    };

    // Traverse the function body and replace return statements
    ReturnReplacer replacer;
    replacer.traverse(funcBody, preorder);
}

SgExpression *stripCasts(SgExpression *expr) {
    if (!expr) return nullptr;

    while (auto *castExpr = isSgCastExp(expr)) {
        expr = castExpr->get_operand(); // Strip the cast
    }

    // Recursively process binary expressions
    if (auto *binaryOp = isSgBinaryOp(expr)) {
        binaryOp->set_lhs_operand(stripCasts(binaryOp->get_lhs_operand()));
        binaryOp->set_rhs_operand(stripCasts(binaryOp->get_rhs_operand()));
    }

    // Recursively process unary expressions
    if (auto *unaryOp = isSgUnaryOp(expr)) {
        unaryOp->set_operand(stripCasts(unaryOp->get_operand()));
    }

    return expr;
}

void deleteRecursiveCalls_dc(SgBasicBlock *funcBody,
                             const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap) {
    // Vector to store statements to be deleted
    std::vector<SgStatement *> statementsToDelete;

    // Step 1: Traverse the AST and collect statements to be deleted
    struct CallCollector : public AstSimpleProcessing {
        const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap;
        std::vector<SgStatement *> &statementsToDelete;

        CallCollector(const std::unordered_map<SgFunctionCallExp *, int> &map,
                      std::vector<SgStatement *> &toDelete)
            : callToValueMap(map), statementsToDelete(toDelete) {}

        void visit(SgNode *node) override {
            if (auto* call = isSgFunctionCallExp(node)) {
                auto it = callToValueMap.find(call);
                if (it==callToValueMap.end()) return;
                
                auto* stmt = SageInterface::getEnclosingStatement(call);

                // --- collect the enclosing statement for deletion ---
                if (stmt) {
                    if (std::find(statementsToDelete.begin(),
                                  statementsToDelete.end(), stmt)
                        == statementsToDelete.end())
                        statementsToDelete.push_back(stmt);
                }

                auto* argList = call->get_args();
                if (!argList) return;
                for (auto* argExpr : argList->get_expressions()) {
                    // strip parentheses
                    argExpr = stripCasts(argExpr);

                    if (auto addOp = isSgAddOp(argExpr)) {
                        SgExpression* L = addOp->get_lhs_operand();
                        SgExpression* R = addOp->get_rhs_operand();
                        if (auto* var = isSgVarRefExp(L))
                            if (auto* val = isSgValueExp(R)) {
                                int offset = static_cast<SgIntVal*>(val)->get_value();
                                
                                std::string name = var->get_symbol()->get_name().getString();

                                if (stmt) {
                                    if (auto* block = isSgBasicBlock(stmt->get_parent())) {
                                        for (auto* child : block->get_statements()) {
                                            if (child == stmt) break;
                                            // look for expression statements containing an assignment


                                            if (auto* exprStmt = isSgExprStatement(child)) {
                                                if (auto* assign = isSgAssignOp(exprStmt->get_expression())) {
                                                    // lhs must match var name
                                                    if (auto* lhsRef = isSgVarRefExp(assign->get_lhs_operand())) {
                                                        std::string lhsName = lhsRef->get_symbol()->get_name().getString();
                                                        lhsName = std::regex_replace(lhsName, std::regex("\\d+$"), "");
                                                        if (lhsName == name) {
                                                            // build new rhs: old_rhs - offset
                                                            SgExpression* oldRHS = assign->get_rhs_operand();
                                                            auto* offsetVal = SageBuilder::buildIntVal(offset);
                                                            auto* newRHS = SageBuilder::buildSubtractOp(oldRHS, offsetVal);
                                                            assign->set_rhs_operand(newRHS);
                                                            break;
                                                        }
                                                    }
                                                }
                                            }

                                            else if (auto* varDecl = isSgVariableDeclaration(child)) {
                                                for (auto* initName : varDecl->get_variables()) {
                                                    std::string declName = initName->get_name().getString();
                                                    if (declName == name) {
                                                        if (auto* init = initName->get_initializer()) {
                                                            // get initializer expression if it's an assign initializer
                                                            if (auto* assignInit = isSgAssignInitializer(init)) {
                                                                SgExpression* oldInit = assignInit->get_operand();
                                                                auto* offsetVal = SageBuilder::buildIntVal(offset);
                                                                auto* newInitExpr = SageBuilder::buildSubtractOp(oldInit, offsetVal);
                                                                auto* newInit = SageBuilder::buildAssignInitializer(newInitExpr);
                                                                initName->set_initializer(newInit);
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                    }
                }

            }
        }
    };

    // Collect statements containing recursive calls
    CallCollector collector(callToValueMap, statementsToDelete);
    collector.traverse(funcBody, preorder);

    // Step 2: Safely remove the marked statements
    for (auto *stmt : statementsToDelete) {
        SageInterface::removeStatement(stmt, true);
    }
}


void createForLoops_dc(SgFunctionDefinition *funcDef,
                              SgBasicBlock *modifiedFuncBody,
                              const std::unordered_map<std::string, int> &basisMap,
                              const std::vector<std::string> &subVarNames,
                              const std::vector<std::string> &divVarNames,
                              std::unordered_map<std::string, std::string> lhsToSizeParam,
                              std::unordered_map<std::string, int> lhsToBaseValue) {
    

    SgBasicBlock *newBody = funcDef->get_body();

    std::string lhs = lhsToSizeParam.begin()->first;
    std::string param = lhsToSizeParam[lhs];

    auto [negativeVar, positiveVar] = analyzeLHSWithSymPy(lhs);

    SgVariableDeclaration *startInit = SageBuilder::buildVariableDeclaration(
        "init_start", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildVarRefExp(negativeVar, newBody)),
        newBody);
    newBody->append_statement(startInit);

    // Step 2: Initialize loop variable: int curr_iter = 0
    SgVariableDeclaration *loopVarDecl = SageBuilder::buildVariableDeclaration(
        "curr_iter", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)),
        newBody);
    newBody->append_statement(loopVarDecl);

    // Step 3: Build the loop condition: curr_iter < num_iter
    SgExpression *loopConditionExpr = SageBuilder::buildLessOrEqualOp(
        SageBuilder::buildVarRefExp("curr_iter", newBody), 
        SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("num_iter", newBody), SageBuilder::buildIntVal(1 - lhsToBaseValue[lhs]))
        );
    SgExprStatement *loopCondition = SageBuilder::buildExprStatement(loopConditionExpr);


    // Step 4: Increment: curr_iter++
    SgPlusPlusOp *increment = SageBuilder::buildPlusPlusOp(
        SageBuilder::buildVarRefExp("curr_iter", newBody), SgUnaryOp::prefix);


    // Step 5: Create the loop body
    SgBasicBlock *loopBody = SageBuilder::buildBasicBlock();
  


    SgVariableDeclaration *innerVarDecl = SageBuilder::buildVariableDeclaration(
        "inner_value", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)),
        newBody);
    loopBody->append_statement(innerVarDecl);


    SgExpression *limitExpr = SageBuilder::buildVarRefExp(param + "_o", newBody);

    // Step 6: Build the loop condition: curr_iter < num_iter
    SgExpression *innerConditionExpr = SageBuilder::buildLessThanOp(
        SageBuilder::buildVarRefExp("inner_value", newBody),
        limitExpr);
    SgExprStatement *innerCondition = SageBuilder::buildExprStatement(innerConditionExpr);


    // Step 7: Increment
    SgAssignOp *inner_increment = SageBuilder::buildAssignOp(
            SageBuilder::buildVarRefExp("inner_value", newBody),
            SageBuilder::buildAddOp(
                SageBuilder::buildVarRefExp("inner_value", newBody), 
                SageBuilder::buildVarRefExp(param, newBody)));


    // Step 8: Create the loop body
    SgBasicBlock *innerBody = SageBuilder::buildBasicBlock();

    SgStatement *assignInnerValue = SageBuilder::buildAssignStatement(
        SageBuilder::buildVarRefExp(negativeVar, newBody),SageBuilder::buildAddOp(SageBuilder::buildVarRefExp("inner_value", newBody), SageBuilder::buildVarRefExp("init_start", newBody)));


    SgExpression *paramExpr = SageBuilder::buildVarRefExp(param, newBody);
    SgExpression *paramOExpr = SageBuilder::buildVarRefExp(param + "_o", newBody);


    SgExpression *innerValuePlusParam = SageBuilder::buildAddOp(
        SageBuilder::buildVarRefExp(negativeVar, newBody), SageBuilder::buildSubtractOp(paramExpr, SageBuilder::buildIntVal(1 - lhsToBaseValue[lhs])));

    
    SgExpression *paramOMinusOne = paramOExpr;


    SgFunctionCallExp *minFuncCall = SageBuilder::buildFunctionCallExp(
        "std::min", SageBuilder::buildIntType(),
        SageBuilder::buildExprListExp(innerValuePlusParam, paramOMinusOne), newBody);


    SgStatement *assignPositiveVar = SageBuilder::buildAssignStatement(
        SageBuilder::buildVarRefExp(positiveVar, newBody), minFuncCall);


    // Step 9: Append the statements to the basic block
    innerBody->append_statement(assignInnerValue);
    innerBody->append_statement(assignPositiveVar);



    // Step 10: Append the modified function body
    for (auto *stmt : modifiedFuncBody->get_statements()) {
        SgStatement *copiedStmt = SageInterface::deepCopy(stmt);
        innerBody->append_statement(copiedStmt);
    }

    for (auto [lhs, sub]: lhsToSizeParam) {
        transformMathExpressions(innerBody, sub, sub + "_o", lhs, newBody, lhsToBaseValue[lhs]);
    }


    
    SgForStatement *innerLoop = SageBuilder::buildForStatement(nullptr, innerCondition, inner_increment, innerBody);
    loopBody->append_statement(innerLoop);


    std::string prefix = "num_iter_";

    // Step 11: Add parameter updates for sub parameters
    for (const auto &param : subVarNames) {
        if (param.find(prefix) == 0) {
            std::string param_name = param.substr(prefix.length());

            // Lookup variable symbol or create it if not found
            SgVariableSymbol *varSymbol = SageInterface::lookupVariableSymbolInParentScopes(param_name, newBody);
            if (!varSymbol) {
                // If not found, declare the variable

                SgVariableDeclaration *varDecl = SageBuilder::buildVariableDeclaration(
                    param_name, SageBuilder::buildIntType(), nullptr, newBody);
                newBody->append_statement(varDecl);
                varSymbol = SageInterface::lookupVariableSymbolInParentScopes(param_name, newBody);
            }

            // Build the update: param += basisMap[param]
            SgExpression *updateExpr = SageBuilder::buildAddOp(
                SageBuilder::buildVarRefExp(varSymbol),
                SageBuilder::buildIntVal(basisMap.at(param_name)));

            SgExprStatement *updateStmt = SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(varSymbol), updateExpr);
            loopBody->append_statement(updateStmt);
        }
    }

    // Step 12: Add parameter updates for div parameters
    for (const auto &param : divVarNames) {
        if (param.find(prefix) == 0) {
            std::string param_name = param.substr(prefix.length());

            // Lookup variable symbol or create it if not found
            SgVariableSymbol *varSymbol = SageInterface::lookupVariableSymbolInParentScopes(param_name, newBody);
            if (!varSymbol) {
                // If not found, declare the variable
                SgVariableDeclaration *varDecl = SageBuilder::buildVariableDeclaration(
                    param_name, SageBuilder::buildIntType(), nullptr, newBody);
                newBody->append_statement(varDecl);
                varSymbol = SageInterface::lookupVariableSymbolInParentScopes(param_name, newBody);
            }

            // Build the update: param *= basisMap[param]
            SgExpression *updateExpr = SageBuilder::buildMultiplyOp(
                SageBuilder::buildVarRefExp(varSymbol),
                SageBuilder::buildIntVal(basisMap.at(param_name)));

            SgExprStatement *updateStmt = SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(varSymbol), updateExpr);
            loopBody->append_statement(updateStmt);
        }
    }


    // Step 13: Build the for-loop
    SgForStatement *forLoop = SageBuilder::buildForStatement(nullptr, loopCondition, increment, loopBody);


    // Append the for-loop to the new body
    newBody->append_statement(forLoop);
}


std::pair<std::string, std::string> analyzeLHSWithSymPy(const std::string &lhs) {
    std::string negativeVar, positiveVar;

    // Build the command to call the Python script
    std::ostringstream command;
    command << "python3 src/find_expr.py \"" << lhs << "\"";

    // Open a pipe to read the script output
    FILE *pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to run the Python script.");
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::istringstream result(buffer);
        result >> negativeVar >> positiveVar;  // Parse output into variables
    }
    pclose(pipe);

    return {negativeVar, positiveVar};
}


void prependStatementsFromBlock_dc(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock) {
    if (!sourceBlock || !targetBlock) {
        std::cerr << "Error: Null SgBasicBlock provided.\n";
        return;
    }

    // Retrieve all statements from the source block
    const SgStatementPtrList &sourceStatements = sourceBlock->get_statements();

    int count = 0;
    // Iterate over the source statements in reverse order to maintain their original order
    for (auto it = sourceStatements.rbegin(); it != sourceStatements.rend(); ++it) {
        SgStatement *stmt = *it;
        if (stmt && count > 1) {
            // Create a deep copy of the statement
            SgStatement *stmtCopy = SageInterface::deepCopy(stmt);

            // Prepend the statement copy to the target block
            SageInterface::prependStatement(stmtCopy, targetBlock);
        }
        count++;
    }
}




