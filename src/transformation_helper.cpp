#include "transformation_helper.h"
#include <cmath>


// The main function responsible for transforming and creating the new function body
void createNewFunctionBody(SgFunctionDefinition *funcDef,
                                    int stackSize,
                                    const std::unordered_map<std::string, int> &basisMap,
                                    const std::vector<int> &newMapping,
                                    const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> &paramAnalysisMap,
                                    const std::unordered_map<SgFunctionCallExp*, int> &callToValueMap,
                                    const std::vector<SgExpression *> &baseCaseConditions) {
    
    // Step 1: Intialize new body                                    
    SgBasicBlock *funcBody = funcDef->get_body();
    SgBasicBlock *newBody = SageBuilder::buildBasicBlock();

    newBody->set_parent(funcDef);
    funcDef->set_body(newBody);

    // Step 2: Array Initialization to hold intermidiate values
    SgAggregateInitializer *arrayInitializer = SageBuilder::buildAggregateInitializer(
        SageBuilder::buildExprListExp(SageBuilder::buildIntVal(0)), SageBuilder::buildArrayType(SageBuilder::buildIntType(), SageBuilder::buildIntVal(stackSize))
    );

    SgVariableDeclaration *recArrayDecl = SageBuilder::buildVariableDeclaration(
        "recursion_array",
        SageBuilder::buildArrayType(SageBuilder::buildIntType(), SageBuilder::buildIntVal(stackSize)),
        SageBuilder::buildAssignInitializer(arrayInitializer),
        newBody); 

    newBody->append_statement(recArrayDecl);


    // Step 2.5: Insert array_shift helper if needed to shift the values in the array
    if (stackSize > 1) {
        SgGlobal *globalScope = SageInterface::getGlobalScope(funcDef);
        createArrayShiftFunction(globalScope);
    }


    // Step 3: Generate num_iter variables and statements for 'sub' and 'div'
    // These variable hold the total number of iteration associated with each variable
    // The variable are divided based on updates with division and subtraction
    std::vector<SgStatement*> subStatements, divStatements;
    std::vector<std::string> subVarNames, divVarNames;

    bool div_exist = false;
    for (const auto &[param, analysis] : paramAnalysisMap) {
        // Variable that are recusively called with subtraction
        if (analysis.first == "sub") {
            // Create all the varaibles for each parameter
            std::string varName = "num_iter_" + param;
            auto *stmt = SageBuilder::buildVariableDeclaration(
                varName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                    SageBuilder::buildDivideOp(SageBuilder::buildVarRefExp(param, newBody), SageBuilder::buildIntVal(basisMap.at(param)))
                ), newBody);
            subStatements.push_back(stmt);
            newBody->append_statement(stmt);
            subVarNames.push_back(varName);
        } 
        // Variable that are recusively called with division
        else if (analysis.first == "div") {
            // Create all the varaibles for each parameter
            div_exist = true;
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
            divStatements.push_back(stmt);
            newBody->append_statement(stmt);
            divVarNames.push_back(varName);
        }
    }

    // Step 3.5 : If any parameters used division add some extra checks
    if (div_exist) {
        SgGlobal *globalScope = SageInterface::getGlobalScope(funcDef);
        createPowFunction(globalScope);
        createIterFindFunction(globalScope);
    }

    // Add all neccessary library includes
    SgSourceFile *sourceFile = SageInterface::getEnclosingNode<SgSourceFile>(funcDef);
    ensureGlobalIncludes(sourceFile);

    // Step 4: Define num_iter_sub and num_iter_div
    // This adds statements to calculate the number of iteration for each set
    defineNumIterSubAndDiv(newBody, subVarNames, divVarNames, subStatements, divStatements);

    // Step 4.5: Define the number of iteration to be the minimum between the subraction and division sets
    defineNumIterLogic(newBody, basisMap, subVarNames, divVarNames, funcDef, stackSize, baseCaseConditions, funcBody);

    // Step 5: Replace Recursive Calls with array access
    replaceRecursiveCalls(funcBody, stackSize, callToValueMap, newBody);

    // Step 6: Replace Return Statements with stores in to the array and array shifts
    replaceReturnStatements(funcBody, stackSize, newBody);

    // Step 6.5: Make sure ROSE detects all the variables
    copySymbolTable(funcBody, newBody);

    // Step 7: Add for loop and insert modified function body
    createForLoop(funcDef, funcBody, basisMap, subVarNames, divVarNames);

    // Step 7: Final Return Statement
    SgStatement *returnStmt = SageBuilder::buildReturnStmt(
        SageBuilder::buildPntrArrRefExp(SageBuilder::buildVarRefExp("recursion_array", newBody), SageBuilder::buildIntVal(stackSize - 1)));
    
    newBody->append_statement(returnStmt);  
}




// Ensure global scope has necessary includes
void ensureGlobalIncludes(SgSourceFile *sourceFile) {
    ROSE_ASSERT(sourceFile != nullptr);

    // Insert headers at the top of the file
    SageInterface::insertHeader(sourceFile, "math.h", PreprocessingInfo::before, false);
}


// Creates the Array Shift Function
SgFunctionDeclaration* createArrayShiftFunction(SgGlobal *globalScope) {
    // Step 1: Create the parameter list
    SgFunctionParameterList *paramList = SageBuilder::buildFunctionParameterList();

    paramList->append_arg(SageBuilder::buildInitializedName(
        "arr", SageBuilder::buildPointerType(SageBuilder::buildIntType())));
    paramList->append_arg(SageBuilder::buildInitializedName(
        "n", SageBuilder::buildIntType()));
    

    // Step 2: Create the function declaration
    SgFunctionDeclaration *arrayShiftFuncDecl = SageBuilder::buildDefiningFunctionDeclaration(
        "array_shift",
        SageBuilder::buildVoidType(),
        paramList,
        globalScope);

    // Step 3: Create the function body
    SgBasicBlock *funcBody = SageBuilder::buildBasicBlock();

    funcBody->set_parent(arrayShiftFuncDecl->get_definition());

    // for (int i = 1; i < n; ++i) { arr[i - 1] = arr[i]; }
    SgVariableDeclaration *loopVarDecl = SageBuilder::buildVariableDeclaration(
        "i", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(1)),
        funcBody);


    SgExpression *loopCondition = SageBuilder::buildLessThanOp(
        SageBuilder::buildVarRefExp("i", funcBody),
        SageBuilder::buildVarRefExp("n", funcBody));
    SgExpression *incrementExpr = SageBuilder::buildPlusPlusOp(
        SageBuilder::buildVarRefExp("i", funcBody), SgUnaryOp::prefix);

    SgExpression *arrayAccessLHS = SageBuilder::buildPntrArrRefExp(
        SageBuilder::buildVarRefExp("arr", funcBody),
        SageBuilder::buildSubtractOp(SageBuilder::buildVarRefExp("i", funcBody), SageBuilder::buildIntVal(1)));
    SgExpression *arrayAccessRHS = SageBuilder::buildPntrArrRefExp(
        SageBuilder::buildVarRefExp("arr", funcBody),
        SageBuilder::buildVarRefExp("i", funcBody));
    SgStatement *assignStmt = SageBuilder::buildAssignStatement(arrayAccessLHS, arrayAccessRHS);


    SgBasicBlock *forLoopBody = SageBuilder::buildBasicBlock(assignStmt);
    SgForStatement *forLoop = SageBuilder::buildForStatement(
        loopVarDecl, SageBuilder::buildExprStatement(loopCondition), incrementExpr, forLoopBody);


    // Step 4: Append the for-loop to the function body
    funcBody->append_statement(forLoop);

    // Step 5: Set the function body to the defining declaration
    arrayShiftFuncDecl->get_definition()->set_body(funcBody);

    // Step 6: Prepend the function declaration to the global scope
    SageInterface::prependStatement(arrayShiftFuncDecl, globalScope);
    
    return arrayShiftFuncDecl;
}


// A helper function to find a power
// Creates a function pow(n,k) that finds n**k more efficiently
SgFunctionDeclaration* createPowFunction(SgGlobal *globalScope) {
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


// Find the number of iterations for a division varaible
SgFunctionDeclaration* createIterFindFunction(SgGlobal *globalScope) {
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
        SageBuilder::buildIntVal(1));

    

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

// For error checking
void printFunctionSymbolTable(SgFunctionDefinition *funcDef) {
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



void copySymbolTable(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock) {
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



// Define num_iter_sub and num_iter_div
void defineNumIterSubAndDiv(SgBasicBlock *newBody,
                            const std::vector<std::string> &subVarNames,
                            const std::vector<std::string> &divVarNames,
                            std::vector<SgStatement*> &subStatements,
                            std::vector<SgStatement*> &divStatements) {

    // Step 4.1: Define num_iter_sub as the minimum of sub variables
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

    // Step 4.2: Define num_iter_div as the minimum of div variables
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
void defineNumIterLogic(SgBasicBlock *newBody,
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


    std::unordered_map<std::string, std::string> tempVars;
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
                    "pow", SageBuilder::buildIntType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildVarRefExp("num_iter", newBody)), newBody));
            newBody->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));

            // Create temp variable: temp_param = param / basisMap[param]
            std::string tempVarName = "temp_" + param_name;
            SgVariableDeclaration *tempVarDecl = SageBuilder::buildVariableDeclaration(
                tempVarName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                    SageBuilder::buildDivideOp(
                        SageBuilder::buildVarRefExp(param_name, newBody),
                        SageBuilder::buildIntVal(basisMap.at(param_name)))),
                newBody);
            newBody->append_statement(tempVarDecl);

            tempVars[param_name] = tempVarName;
        }

        modifiedBaseCase = extractAndModifyBaseCase(oldBody, stackSize, baseCaseConditions, tempVars, newBody);

        // Ensure base case is not null
        if (!modifiedBaseCase) {
            throw std::runtime_error("No base case found in the function.");
        }
        
        for (auto *stmt : modifiedBaseCase->get_statements()) {
            SgStatement *copiedStmt = SageInterface::deepCopy(stmt);
            newBody->append_statement(copiedStmt);
        }
    }

    // Both sub and div exist
    else if (!subVarNames.empty() && !divVarNames.empty()) {
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
                    "pow", SageBuilder::buildIntType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildVarRefExp("num_iter", newBody)), newBody));
            ifBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));
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

            // Create temp variable for div parameter
            std::string tempVarName = "temp_" + param_name;
            SgVariableDeclaration *tempVarDecl = SageBuilder::buildVariableDeclaration(
                tempVarName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                    SageBuilder::buildSubtractOp(
                        SageBuilder::buildVarRefExp(param_name, newBody),
                        SageBuilder::buildIntVal(basisMap.at(param_name)))),
                newBody);
            elseBlock->append_statement(tempVarDecl);

            tempVars[param_name] = tempVarName;
        }

        for (const auto &param : divVarNames) {
            std::string param_name = param.substr(prefix.length());
            SgExpression *updateExpr = SageBuilder::buildDivideOp(
                SageBuilder::buildVarRefExp(param_name, newBody),
                SageBuilder::buildFunctionCallExp(
                    "pow", SageBuilder::buildDoubleType(),
                    SageBuilder::buildExprListExp(
                        SageBuilder::buildIntVal(basisMap.at(param_name)),
                        SageBuilder::buildVarRefExp("num_iter", newBody)), newBody));
            elseBlock->append_statement(SageBuilder::buildAssignStatement(
                SageBuilder::buildVarRefExp(param_name, newBody), updateExpr));

            // Create temp variable for div parameter
            std::string tempVarName = "temp_" + param_name;
            SgVariableDeclaration *tempVarDecl = SageBuilder::buildVariableDeclaration(
                tempVarName, SageBuilder::buildIntType(),
                SageBuilder::buildAssignInitializer(
                    SageBuilder::buildDivideOp(
                        SageBuilder::buildVarRefExp(param_name, newBody),
                        SageBuilder::buildIntVal(basisMap.at(param_name)))),
                newBody);
            elseBlock->append_statement(tempVarDecl);

            tempVars[param_name] = tempVarName;
        }
        
        modifiedBaseCase = extractAndModifyBaseCase(oldBody, stackSize, baseCaseConditions, tempVars, newBody);

        // Ensure base case is not null
        if (!modifiedBaseCase) {
            throw std::runtime_error("No base case found in the function.");
        }
        
        for (auto *stmt : modifiedBaseCase->get_statements()) {
            SgStatement *copiedStmt = SageInterface::deepCopy(stmt);
            elseBlock->append_statement(copiedStmt);
        }

        // Append the if-else statement
        newBody->append_statement(compareStmt);
    }
}



void replaceReturnStatements(SgBasicBlock *funcBody, int stackSize, SgBasicBlock *newBody) {
    struct ReturnReplacer : public AstSimpleProcessing {
        int stackSize;
        SgBasicBlock *newBody;

        ReturnReplacer(int size, SgBasicBlock *bdy) : stackSize(size), newBody(bdy) {}

        void visit(SgNode *node) override {
            if (auto *returnStmt = isSgReturnStmt(node)) {
                SgExpression *returnExpr = returnStmt->get_expression();

                if (stackSize > 1) {
                    // Step 1: Create a new variable temp_return
                    SgVariableDeclaration *tempReturnDecl = SageBuilder::buildVariableDeclaration(
                        "temp_return", SageBuilder::buildIntType(),
                        SageBuilder::buildAssignInitializer(SageInterface::deepCopy(returnExpr)), newBody);
                        
                    // Step 2: Call array_shift(recursion_array, stackSize)
                    SgExprListExp *args = SageBuilder::buildExprListExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildIntVal(stackSize));

                    SgFunctionCallExp *arrayShiftCall = SageBuilder::buildFunctionCallExp(
                        "array_shift", SageBuilder::buildVoidType(), args, newBody);
                    SgExprStatement *shiftCallStmt = SageBuilder::buildExprStatement(arrayShiftCall);

                    // Step 3: Assign recursion_array[stack_size - 1] = temp_return
                    SgExpression *arrayAccess = SageBuilder::buildPntrArrRefExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildSubtractOp(
                            SageBuilder::buildIntVal(stackSize),
                            SageBuilder::buildIntVal(1)));

                    SgExprStatement *assignmentStmt = SageBuilder::buildAssignStatement(
                        arrayAccess, SageBuilder::buildVarRefExp("temp_return", newBody));

                    // Step 4: Add continue statement
                    SgStatement *continueStmt = SageBuilder::buildBreakStmt();

                    // Step 5: Replace the return statement with the sequence
                    SageInterface::replaceStatement(returnStmt, tempReturnDecl);
                    SageInterface::insertStatementAfter(tempReturnDecl, shiftCallStmt);
                    SageInterface::insertStatementAfter(shiftCallStmt, assignmentStmt);
                    SageInterface::insertStatementAfter(assignmentStmt, continueStmt);
                } else {
                    // Direct assignment: recursion_array[0] = return_expr
                    SgExpression *arrayAccess = SageBuilder::buildPntrArrRefExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildIntVal(0));
                    SgExprStatement *assignmentStmt = SageBuilder::buildAssignStatement(arrayAccess, returnExpr);

                    // Add continue statement
                    SgStatement *continueStmt = SageBuilder::buildBreakStmt();

                    SageInterface::replaceStatement(returnStmt, assignmentStmt);
                    SageInterface::insertStatementAfter(assignmentStmt, continueStmt);
                }
            }
        }
    };

    // Traverse the function body and replace return statements
    ReturnReplacer replacer(stackSize, newBody);
    replacer.traverse(funcBody, preorder);
}


void replaceRecursiveCalls(SgBasicBlock *funcBody,
                           int stackSize,
                           const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap, 
                           SgBasicBlock *newBody) {
    struct CallReplacer : public AstSimpleProcessing {
        int stackSize;
        const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap;
        SgBasicBlock *newBody;

        CallReplacer(int size, const std::unordered_map<SgFunctionCallExp *, int> &map, SgBasicBlock *bdy)
            : stackSize(size), callToValueMap(map), newBody(bdy) {}

        void visit(SgNode *node) override {
            if (auto *expr = isSgFunctionCallExp(node)) {
                // Check if the function call is a recursive call
                auto it = callToValueMap.find(expr);
                if (it != callToValueMap.end()) {
                    int callValue = it->second;

                    // Replace with recursion_array[stack_size - callValue]
                    SgExpression *arrayAccess = SageBuilder::buildPntrArrRefExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildSubtractOp(
                            SageBuilder::buildIntVal(stackSize),
                            SageBuilder::buildIntVal(callValue)));

                    SageInterface::replaceExpression(expr, arrayAccess, true);
                }
            }
        }
    };

    // Traverse the function body and replace recursive calls
    CallReplacer replacer(stackSize, callToValueMap, newBody);
    replacer.traverse(funcBody, preorder);
}





void createForLoop(SgFunctionDefinition *funcDef,
                              SgBasicBlock *modifiedFuncBody,
                              const std::unordered_map<std::string, int> &basisMap,
                              const std::vector<std::string> &subVarNames,
                              const std::vector<std::string> &divVarNames) {

    SgBasicBlock *newBody = funcDef->get_body();

    // Step 2: Initialize loop variable: int curr_iter = 1
    SgVariableDeclaration *loopVarDecl = SageBuilder::buildVariableDeclaration(
        "curr_iter", SageBuilder::buildIntType(),
        SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)),
        newBody);
    newBody->append_statement(loopVarDecl);

    // Step 3: Build the loop condition: curr_iter < num_iter
    SgExpression *loopConditionExpr = SageBuilder::buildLessOrEqualOp(
        SageBuilder::buildVarRefExp("curr_iter", newBody),
        SageBuilder::buildVarRefExp("num_iter", newBody));
    SgExprStatement *loopCondition = SageBuilder::buildExprStatement(loopConditionExpr);

    // Step 4: Increment: curr_iter++
    SgPlusPlusOp *increment = SageBuilder::buildPlusPlusOp(
        SageBuilder::buildVarRefExp("curr_iter", newBody), SgUnaryOp::prefix);

    // Step 5: Create the loop body
    SgBasicBlock *loopBody = SageBuilder::buildBasicBlock();

    // Step 6: Append the modified function body
    for (auto *stmt : modifiedFuncBody->get_statements()) {
        SgStatement *copiedStmt = SageInterface::deepCopy(stmt);
        copiedStmt->set_parent(loopBody);
        loopBody->append_statement(copiedStmt);
    }

    SgBasicBlock *whileBody = SageInterface::deepCopy(loopBody);
    SgWhileStmt *whileLoop = SageBuilder::buildWhileStmt(SageBuilder::buildBoolValExp(true), whileBody);

    loopBody->get_statements().clear(); // Clear existing statements
    loopBody->append_statement(whileLoop);

    std::string prefix = "num_iter_";

    // Step 7: Add parameter updates for sub parameters
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

    // Step 8: Add parameter updates for div parameters
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

    // Step 9: Build the for-loop
    SgForStatement *forLoop = SageBuilder::buildForStatement(nullptr, loopCondition, increment, loopBody);


    // Append the for-loop to the new body
    newBody->append_statement(forLoop);
}


// Adds statements from one block to another
void prependStatementsFromBlock(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock) {
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
            std::cout << targetBlock->unparseToString() << "\n\n" << std::flush;
        }
        count++;
    }
}



// Find the code block with base case
SgBasicBlock* findBaseCaseBlock(SgBasicBlock *funcBody, const std::vector<SgExpression *> &baseCaseConditions) {
    SgBasicBlock *baseBlock = nullptr;

    for (auto *stmt : funcBody->get_statements()) {
        if (auto *ifStmt = isSgIfStmt(stmt)) {
            // Compare conditions to find the base case
            SgExpression *condition = nullptr;

            if (auto *exprStmt = isSgExprStatement(ifStmt->get_conditional())) {
                condition = exprStmt->get_expression();
            } else {
                condition = isSgExpression(ifStmt->get_conditional());
            }


            // Compare the condition with base case conditions
            for (auto *baseCond : baseCaseConditions) {
                if (condition->unparseToString() == baseCond->unparseToString()) {
                    baseBlock = isSgBasicBlock(ifStmt->get_true_body());
                    if (baseBlock) return baseBlock;
                }
            }
        }
    }

    return baseBlock;
}



void replaceParamsWithTempVars(SgExpression *expr, const std::unordered_map<std::string, std::string> &tempVars) {
    struct ReplaceParams : public AstSimpleProcessing {
        const std::unordered_map<std::string, std::string> &tempVars;

        ReplaceParams(const std::unordered_map<std::string, std::string> &vars) : tempVars(vars) {}

        void visit(SgNode *node) override {
            if (auto *varRef = isSgVarRefExp(node)) {
                std::string varName = varRef->get_symbol()->get_name().getString();
                auto it = tempVars.find(varName);
                if (it != tempVars.end()) {
                    // Replace the variable with its temporary version
                    SageInterface::replaceExpression(
                        varRef, SageBuilder::buildVarRefExp(it->second), true);
                }
            }
        }
    };

    // Apply the replacement to the given expression
    ReplaceParams replacer(tempVars);
    replacer.traverse(expr, preorder);
}


// Change Base case
SgBasicBlock* extractAndModifyBaseCase(SgBasicBlock *funcBody,
                                      int stackSize,
                                      const std::vector<SgExpression *> &baseCaseConditions,
                                      const std::unordered_map<std::string, std::string> &tempVars, 
                                      SgBasicBlock *newBody) {
    // Step 1: Find the base case block
    SgBasicBlock *originalBaseBlock = findBaseCaseBlock(funcBody, baseCaseConditions);
    if (!originalBaseBlock) {
        std::cerr << "Error: Base case block not found!\n";
        return nullptr;
    }

    // Step 2: Create a deep copy of the base case block
    SgBasicBlock *copiedBaseBlock = SageInterface::deepCopy(originalBaseBlock);

    // Step 3: Modify the copied base case
    struct BaseCaseModifier : public AstSimpleProcessing {
        int stackSize;
        const std::unordered_map<std::string, std::string> &tempVars;
        SgBasicBlock *newBody;

        BaseCaseModifier(int size, const std::unordered_map<std::string, std::string> &vars, SgBasicBlock *bdy)
            : stackSize(size), tempVars(vars), newBody(bdy) {}

        void visit(SgNode *node) override {
            if (auto *returnStmt = isSgReturnStmt(node)) {
                SgExpression *returnExpr = returnStmt->get_expression();

                // Replace parameters with temporary variables
                replaceParamsWithTempVars(returnExpr, tempVars);

                if (stackSize > 1) {
                    // Insert array_shift(recursion_array, stackSize)
                    SgExprListExp *args = SageBuilder::buildExprListExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildIntVal(stackSize));

                    SgFunctionCallExp *arrayShiftCall = SageBuilder::buildFunctionCallExp(
                        "array_shift", SageBuilder::buildVoidType(), args, newBody);
                    SgExprStatement *shiftCallStmt = SageBuilder::buildExprStatement(arrayShiftCall);

                    // Assign recursion_array[stack_size - 1] = return_expr
                    SgExpression *arrayAccess = SageBuilder::buildPntrArrRefExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildSubtractOp(
                            SageBuilder::buildIntVal(stackSize),
                            SageBuilder::buildIntVal(1)));
                    SgExprStatement *assignmentStmt = SageBuilder::buildAssignStatement(arrayAccess, returnExpr);

                    // Replace the return statement with the new sequence
                    SageInterface::replaceStatement(returnStmt, shiftCallStmt);
                    SageInterface::insertStatementAfter(shiftCallStmt, assignmentStmt);
                } else {
                    // Direct assignment: recursion_array[0] = return_expr
                    SgExpression *arrayAccess = SageBuilder::buildPntrArrRefExp(
                        SageBuilder::buildVarRefExp("recursion_array", newBody),
                        SageBuilder::buildIntVal(0));
                    SgExprStatement *assignmentStmt = SageBuilder::buildAssignStatement(arrayAccess, returnExpr);

                    SageInterface::replaceStatement(returnStmt, assignmentStmt);
                }
            }
        }
    };

    // Apply modifications to the copied base block
    BaseCaseModifier modifier(stackSize, tempVars, newBody);
    modifier.traverse(copiedBaseBlock, preorder);

    // Return the modified copy of the base case
    return copiedBaseBlock;
}
