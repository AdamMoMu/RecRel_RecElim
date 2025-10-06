#include "helper.h"
#include "tail.h"
#include "transformation_divconq.h"
#include "transformation_helper.h"
#include <iostream>
#include <vector>
#include <stack>
#include <iomanip> 
#include <filesystem>


namespace fs = std::filesystem;

class RecursionToIteration : public AstSimpleProcessing {
private:
    struct RecursionInfo {
        SgFunctionCallExp *call;           // The recursive call
        std::vector<SgExpression *> conditions; // The conditions leading to the call
    };

    // Store recursive calls and their conditions
    std::vector<RecursionInfo> recursionCalls; 

    // Used to temporarily store a list of conditions for building the RecursionInfo struct
    std::vector<SgExpression *> conditionStack;

    //
    std::map<std::string, std::string> functionCallMap;

    // Map each parameter to a size
    std::unordered_map<std::string, std::string> lhsToSizeParam;

    // Map each parameter to its base value (extracted from base cases)
    std::unordered_map<std::string, int> lhsToBaseValue;

    // Store every base condition
    std::set<std::string> uniqueBaseConditions;

    // Store the modified base conditions that use new varaibles
    std::vector<std::string> modifiedBaseConditions;

    // Maps each parameter to a list of the arguments to the recursive calls that correspond to it
    // For example:
    // func(n,m) {
    //     ...
    //     return func(n-1,m) + func(n+1,m/2)
    // }
    // Then:
    // paramToArgsMap = {'n':['n+1','n+2'], 'm':['m','m/2']}
    std::unordered_map<std::string, std::vector<std::string>> paramToArgsMap; 

    // Maps each paramter to an operation (subtraction or division) and the list of operations found in the recursive calls
    std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> paramAnalysisMap;

    // The size of the array that hold the intermediate values
    int arrDim = 0;

    // Bool that is used to keep track of whether current function is a divide-and-conquer function or not
    bool divConq = false;

    // A bool that keeps track of whether the current function still satisfies all the prequistes needed to complete the transoformation
    bool simp = true;

    // A counter that is used to help name the newly introduced size varaibles
    int sizeCounter = 0;

    // Name of the function currently being operated on
    std::string funcName;

public:
    void visit(SgNode *node) override {
        if (auto *funcDef = isSgFunctionDefinition(node)) {
            // Reset for a new function
            recursionCalls.clear();
            conditionStack.clear();
            functionCallMap.clear();
            lhsToSizeParam.clear();
            lhsToBaseValue.clear();
            modifiedBaseConditions.clear();
            uniqueBaseConditions.clear();
            paramToArgsMap.clear();
            paramAnalysisMap.clear();
            sizeCounter = 0;
            arrDim = 0;
            divConq = false;
            simp = true;
            funcName = "";

            // Check to see if tail recursive or  non-tail
            RecursionType recursionType = checkRecursion(funcDef);

            // Tail Transformation
            if (recursionType == TAIL) {
                transformTR(funcDef);
                wrapFunc(funcDef);
            } else if (recursionType == NONTAIL) {
                analyzeNonTailRecursion(funcDef);
                printRecursionCalls();
                processMappings(funcDef);

                // If still within scope, continue
                if (simp) {
                    // If its divide and conquer
                    if (divConq) {
                        printOperations_DC();
                        simp = analyzeConditionsForDivOperations();
                        if (simp) {
                            processIteration();
                            auto [resultBool, stack_size, basis_map , update_map] = runIterationAnalysis(paramAnalysisMap);
                            if (resultBool) {
                                auto call_map = mapRecursionCallsToValues(update_map);
                                auto baseCaseConditions = findBaseCaseConditions(funcDef);
                                createNewFunctionBody_dc(funcDef, stack_size, basis_map, update_map, paramAnalysisMap, call_map, baseCaseConditions, lhsToSizeParam, lhsToBaseValue);
                            }
                        }
                    } else {
                        printOperations();
                        simp = analyzeConditionsForDivOperations();
                        if (simp) {
                            processIteration();
                            // If its divide and conquer
                            if (divConq) {
                                auto [resultBool, stack_size, basis_map , update_map] = runIterationAnalysis(paramAnalysisMap);
                                if (resultBool) {
                                    auto call_map = mapRecursionCallsToValues(update_map);
                                    auto baseCaseConditions = findBaseCaseConditions(funcDef);
                                    createNewFunctionBody_dc(funcDef, stack_size, basis_map, update_map, paramAnalysisMap, call_map, baseCaseConditions, lhsToSizeParam, lhsToBaseValue);
                                }
                            } else {
                                auto [resultBool, stack_size, basis_map , update_map] = runIterationAnalysis(paramAnalysisMap);
                                if (resultBool) {
                                    auto call_map = mapRecursionCallsToValues(update_map);
                                    auto baseCaseConditions = findBaseCaseConditions(funcDef);
                                    createNewFunctionBody(funcDef, stack_size, basis_map, update_map, paramAnalysisMap, call_map, baseCaseConditions);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    enum RecursionType {
        NONE,
        TAIL,
        NONTAIL
    };

    // Check for recursion and its type
    RecursionType checkRecursion(SgFunctionDefinition *funcDef) {
        bool tail = false;
        bool nontail = false;

        auto *funcDecl = funcDef->get_declaration();
        funcName = funcDecl->get_name().getString();
        
        // Traverse all function calls
        for (auto i : NodeQuery::querySubTree(funcDef, V_SgFunctionCallExp)) {
            if (auto *funcCall = isSgFunctionCallExp(i)) {
                // Recursive check
                if (funcCall->get_function()->unparseToString() == funcName) {
                    if (isTail(funcCall)) {
                        tail = true;
                    } else {
                        nontail = true;
                    }
                }
            }
        }

        // Classify the function based on the types of recursive calls
        if (nontail) {
            return NONTAIL;
        }
        if (tail) {
            return TAIL;
        }
        return NONE;
    }

    // Check to see if its tail-recursive
    bool isTail(SgFunctionCallExp *funcCall) {
        SgNode *parent = funcCall->get_parent();

        // Case 1: The function call is part of a return statement
        if (auto *retStmt = isSgReturnStmt(parent)) {
            return retStmt->get_expression() == funcCall;
        }

        // Case 2: The function call is the last statement in its block AND the block is the last one
        if (auto *stmt = isSgStatement(parent)) {
            auto *block = isSgBasicBlock(stmt->get_parent());
            if (block) {
                const auto &statements = block->get_statements();

                // Check if this statement is the last statement in its block
                if (!statements.empty() && statements.back() == stmt) {
                    // Ensure the block itself is the last executed in the function
                    return isLastExecuted(block, funcCall);
                }
            }
        }

        return false;
    }

    // Helper function to check if a block is the last executed in its parent
    bool isLastExecuted(SgNode *node, SgFunctionCallExp *funcCall) {
        SgNode *parent = node->get_parent();
        
        // If the parent is a function body, ensure the block is the last statement
        if (auto *funcDef = isSgFunctionDefinition(parent)) {
            const auto &statements = funcDef->get_body()->get_statements();
            if (auto nodeStmt = isSgStatement(node)) {
                return !statements.empty() && statements.back() == nodeStmt;
            }
            return false;
        }

        auto *parentBlock = isSgBasicBlock(parent);
        if (parentBlock) {
            const auto &statements = parentBlock->get_statements();

            // Check if this statement is the last statement in its block
            if (!statements.empty() && statements.back() == node) {
                // Ensure the block itself is the last executed in the function

                SgNode *superParent = parentBlock->get_parent();
        
                if (auto *topDef = isSgFunctionDefinition(superParent)) {
                    const auto &statements = topDef->get_body()->get_statements();
                    if (auto nodeStmt = isSgStatement(node)) {
                        return !statements.empty() && statements.back() == nodeStmt;
                    }
                    return false;
                }

                return isLastExecuted(parentBlock, funcCall);
            }
        }

        return isLastExecuted(parent, funcCall);
    }

    // Function used to populate the recursionInfo struct
    void analyzeNonTailRecursion(SgNode *node) {
        // Push conditions when entering an if-statement
        if (auto *ifStmt = isSgIfStmt(node)) {
            SgStatement *conditional = ifStmt->get_conditional();
            if (conditional) {
                if (auto *exprStmt = isSgExprStatement(conditional)) {
                    if (auto *innerExpr = exprStmt->get_expression()) {
                        conditionStack.push_back(innerExpr);
                    }
                }
            }

            // Analyze the `if` body
            analyzeNonTailRecursion(ifStmt->get_true_body());

            // Temporarily clear conditions for `else` block
            if (auto *elseBody = ifStmt->get_false_body()) {
                std::vector<SgExpression *> savedConditions = conditionStack;
                conditionStack.clear(); // Reset conditions for the `else` branch
                analyzeNonTailRecursion(elseBody);
                conditionStack = savedConditions; // Restore conditions after the `else` block
            }

            // Pop the condition for the current `if` block
            conditionStack.pop_back();
            return;
        }

        // Check for function calls
        if (auto *funcCall = isSgFunctionCallExp(node)) {
            if (isRecursiveCall(funcCall)) {
                // Store the recursive call and the current conditions
                recursionCalls.push_back({funcCall, conditionStack});
            }
        }

        // Traverse the children nodes
        if (node) {
            const auto &successors = node->get_traversalSuccessorContainer();
            if (!successors.empty()) {
                for (auto *child : successors) {
                    if (child) {
                        analyzeNonTailRecursion(child);
                    }
                }
            }
        }
    }

    
    // Function used to check if a function is recursive
    bool isRecursiveCall(SgFunctionCallExp *funcCall) {
        if (!funcCall) return false;

        // Ensure the function being called is not null
        auto *function = funcCall->get_function();
        if (!function) return false;

        if (function->unparseToString() == funcName) {
            return true;
        }

        return false;
    }


    // Gets all the statement that are executed before a specific recursive call
    std::vector<SgStatement *> getStatementsToCall(SgFunctionDefinition *funcDef, const RecursionInfo &recInfo) {
        std::vector<SgStatement *> relevantStatements;
        SgBasicBlock *funcBody = funcDef->get_body();

        if (!funcBody) return relevantStatements; // No body to analyze

        // Condition stack for matching
        std::vector<SgExpression *> conditionStack = recInfo.conditions;

        // Work queue to iterate through statements
        std::vector<SgStatement *> stmtQueue = funcBody->get_statements();

        // Process statements in order
        while (!stmtQueue.empty()) {
            // Process the first statement
            SgStatement *stmt = stmtQueue.front();
            stmtQueue.erase(stmtQueue.begin());

            // Handle if-statements
            if (auto *ifStmt = isSgIfStmt(stmt)) {
                SgStatement *conditional = ifStmt->get_conditional();
                SgExpression *condExpr = nullptr;

                // Safely extract the condition expression
                if (auto *exprStmt = isSgExprStatement(conditional)) {
                    condExpr = exprStmt->get_expression();
                } else {
                    condExpr = isSgExpression(conditional);
                }

                // Handle matching condition
                if (!conditionStack.empty() && condExpr) {
                    auto *currentCondition = conditionStack.front();

                    if (condExpr->unparseToString() == currentCondition->unparseToString()) {
                        conditionStack.erase(conditionStack.begin());

                        // Analyze the true body
                        if (auto *trueBlock = isSgBasicBlock(ifStmt->get_true_body())) {
                            const auto &ifStatements = trueBlock->get_statements();
                            stmtQueue.insert(stmtQueue.begin(), ifStatements.begin(), ifStatements.end());
                        } else if (auto *trueStmt = ifStmt->get_true_body()) {
                            stmtQueue.insert(stmtQueue.begin(), trueStmt);
                        }
                        continue; // Skip adding the if-statement itself
                    }
                }

                // Handle else block
                SgStatement *elseBody = ifStmt->get_false_body();
                if (elseBody) {
                    // If the elseBody is a block, process its statements
                    if (auto *elseBlock = isSgBasicBlock(elseBody)) {
                        const auto &elseStatements = elseBlock->get_statements();
                        stmtQueue.insert(stmtQueue.begin(), elseStatements.begin(), elseStatements.end());
                    } else {
                        // If the elseBody is a single statement, add it directly
                        stmtQueue.insert(stmtQueue.begin(), elseBody);
                    }
                }
                continue; // Skip further processing for this if-statement
            }

            // Check if this is the recursive call
            if (isRecursiveCallInStatement(stmt, recInfo.call)) {
                relevantStatements.push_back(stmt);
                break; // Stop traversal once the recursive call is found
            }

            // Add the statement to the relevant list
            relevantStatements.push_back(stmt);
        }

        return relevantStatements;
    }

    // Removes Casting/Parenthesis from an expression
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




    // Replace a function call with a size parameter if neccessary and classify the function as divide and conqeur
    // If non-size functions are found then the function cant be optimized
    // This process can be improved by function inlining and doesnt necessarily need to be done using ROSE
    SgExpression *replaceFunctionCalls(SgExpression *expr) {
        if (!expr) return nullptr;

        // Handle function calls
        if (auto *funcCall = isSgFunctionCallExp(expr)) {
            std::string funcName;

            // Extract the function being called
            SgExpression *functionExpr = funcCall->get_function();

            if (auto *dotExp = isSgDotExp(functionExpr)) {
                // Handle member function calls like arr.size()
                SgExpression *object = dotExp->get_lhs_operand();
                SgExpression *member = dotExp->get_rhs_operand();

                if (object && member) {
                    std::string objectName = object->unparseToString();
                    std::string memberName = member->unparseToString();
                    funcName = memberName;
                }
            } else if (auto *arrowExp = isSgArrowExp(functionExpr)) {
                // Handle pointer-based member function calls like arr->size()
                SgExpression *object = arrowExp->get_lhs_operand();
                SgExpression *member = arrowExp->get_rhs_operand();

                if (object && member) {
                    std::string objectName = object->unparseToString();
                    std::string memberName = member->unparseToString();
                    funcName = memberName;
                }
            } else if (auto *functionRef = isSgFunctionRefExp(functionExpr)) {
                // Normal function calls
                funcName = functionRef->get_symbol()->get_name().getString();
            }

            // Check if the function matches "size()" or "sizeof()"
            if (funcName == "size" || funcName == "sizeof") {
                divConq = true;
                std::string argsStr = funcCall->unparseToString();

                // Check if we already have a replacement for this function call
                if (functionCallMap.find(argsStr) == functionCallMap.end()) {
                    std::string newVarName = "size_" + std::to_string(sizeCounter++);
                    functionCallMap[argsStr] = newVarName;
                }

                std::string replacementVar = functionCallMap[argsStr];
                return SageBuilder::buildVarRefExp(replacementVar);
            }
            else {
                std::cout << "Not Simplifiable!\n\n";
                simp = false;

            }
        }

        // Recursively replace function calls in binary expressions
        if (auto *binaryOp = isSgBinaryOp(expr)) {
            binaryOp->set_lhs_operand(replaceFunctionCalls(binaryOp->get_lhs_operand()));
            binaryOp->set_rhs_operand(replaceFunctionCalls(binaryOp->get_rhs_operand()));
        }

        // Recursively replace function calls in unary expressions
        if (auto *unaryOp = isSgUnaryOp(expr)) {
            unaryOp->set_operand(replaceFunctionCalls(unaryOp->get_operand()));
        }

        return expr;
    }


    // Check if a specific statement contains a specific recursive call
    bool isRecursiveCallInStatement(SgStatement *stmt, SgFunctionCallExp *call) {
        auto *exprStmt = isSgExprStatement(stmt);
        return exprStmt && exprStmt->get_expression() == call;
    }


    // Takes a recursive call and checks its arguments
    // This checks for any size functions (not robust and needs improvement)
    // It can classify a function as divide and conquer
    // It also performs a propagation pass
    SgExprListExp *analyzeAndSimplifyRecursion(SgFunctionDefinition *funcDef, const RecursionInfo &recInfo) {
        // Make a deep copy of the recursive call's arguments for analysis
        SgExprListExp *currentArgs = SageInterface::deepCopy(recInfo.call->get_args());
        if (!currentArgs) return nullptr; // Safety check

        // Step 1: Replace function calls in the arguments FIRST
        for (auto &arg : currentArgs->get_expressions()) {
            if (arg) {
                // Strip away any cast expressions to simplify the argument
                arg = stripCasts(arg);

                // Replace any function calls inside the arguments
                arg = replaceFunctionCalls(arg);
            }
        }

        std::set<std::string> variablesOfInterest = getReferencedVariables(currentArgs);

        // Step 2: Get all relevant statements leading to this recursive call
        std::vector<SgStatement *> statements = getStatementsToCall(funcDef, recInfo);

        // Step 3: Perform backward analysis on the statements
        for (auto it = statements.rbegin(); it != statements.rend(); ++it) {
            SgStatement *stmt = *it;
            if (!stmt) continue;

            // Check for variable declarations with initialization
            if (auto *varDecl = isSgVariableDeclaration(stmt)) {
                for (auto *initName : varDecl->get_variables()) {
                    if (!initName) continue;

                    std::string varName = initName->get_name().getString();
                    if (variablesOfInterest.find(varName) != variablesOfInterest.end()) {
                        if (auto *initExpr = isSgAssignInitializer(initName->get_initializer())) {
                            SgExpression *rhsValue = initExpr->get_operand();
                            rhsValue = stripCasts(rhsValue);
                            rhsValue = replaceFunctionCalls(rhsValue);

                            // Substitution
                            std::unordered_map<std::string, SgExpression *> substitutions;
                            substitutions[varName] = SageInterface::deepCopy(rhsValue);

                            for (auto &arg : currentArgs->get_expressions()) {
                                if (arg) {
                                    arg = replaceVarsInExpression(arg, substitutions);
                                    arg = stripCasts(arg); // Clean up again
                                }
                            }

                            variablesOfInterest = getReferencedVariables(currentArgs);
                        }
                    }
                }
            }

            // Handle assignments
            if (auto *exprStmt = isSgExprStatement(stmt)) {
                auto *assign = isSgAssignOp(exprStmt->get_expression());
                if (!assign) continue;

                auto *lhs = isSgVarRefExp(assign->get_lhs_operand());
                if (!lhs) continue;

                std::string lhsVarName = lhs->get_symbol()->get_name().getString();

                if (variablesOfInterest.find(lhsVarName) != variablesOfInterest.end()) {
                    SgExpression *rhsValue = assign->get_rhs_operand();
                    rhsValue = stripCasts(rhsValue);
                    rhsValue = replaceFunctionCalls(rhsValue);

                    // Substitution
                    std::unordered_map<std::string, SgExpression *> substitutions;
                    substitutions[lhsVarName] = SageInterface::deepCopy(rhsValue);

                    for (auto &arg : currentArgs->get_expressions()) {
                        if (arg) {
                            arg = replaceVarsInExpression(arg, substitutions);
                            arg = stripCasts(arg); // Clean up again
                        }
                    }

                    variablesOfInterest = getReferencedVariables(currentArgs);
                }
            }
        }

        return currentArgs; // Return the cleaned and simplified arguments
    }


    // Simplify a specific recursive call and extract its info
    std::vector<std::string> simplifyCall(SgFunctionDefinition *funcDef, const RecursionInfo recInfo) {
        std::cout << "Analyzing Recursive Call: " << recInfo.call->unparseToString() << "\n";

        // Step 1: Perform backward analysis to get the simplified arguments as SgExprListExp
        SgExprListExp *simplifiedExprList = analyzeAndSimplifyRecursion(funcDef, recInfo);
        if (!simplifiedExprList) {
            std::cerr << "Error: Failed to simplify recursive arguments.\n";
        }

        // Step 2: Convert the simplified arguments to strings
        std::vector<std::string> originalArgs;
        for (auto *arg : simplifiedExprList->get_expressions()) {
            originalArgs.push_back(arg->unparseToString());
        }

        // Step 3: Simplify the arguments using SymPy
        std::vector<std::string> simplifiedArgs = simplifyArgumentsWithSymPy(originalArgs);

        std::cout << "Simplified Arguments: (";
        for (size_t i = 0; i < simplifiedArgs.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << simplifiedArgs[i];
        }
        std::cout << ")\n";

        return simplifiedArgs;
    }



    // Check if a specific statement contains any recursive call
    bool containsRecursiveCall(SgStatement *stmt, const std::string &funcName) {
        if (!stmt) return false;

        // Query the subtree for function call expressions
        for (auto *node : NodeQuery::querySubTree(stmt, V_SgFunctionCallExp)) {
            if (auto *funcCall = isSgFunctionCallExp(node)) {
                if (funcCall->get_function()->unparseToString() == funcName) {
                    return true; // Recursive call found
                }
            }
        }
        return false;
    }

    // Simplify Negation
    SgExpression* distributeNegation(SgExpression* expr) {
        if (!expr) return nullptr;

        // Handle binary operators
        if (auto* binOp = isSgBinaryOp(expr)) {
            switch (binOp->variantT()) {
                // Relational operators
                case V_SgLessThanOp: // !(a < b) → a >= b
                    return SageBuilder::buildGreaterOrEqualOp(
                        SageInterface::deepCopy(binOp->get_lhs_operand()),
                        SageInterface::deepCopy(binOp->get_rhs_operand()));
                case V_SgGreaterThanOp: // !(a > b) → a <= b
                    return SageBuilder::buildLessOrEqualOp(
                        SageInterface::deepCopy(binOp->get_lhs_operand()),
                        SageInterface::deepCopy(binOp->get_rhs_operand()));
                case V_SgLessOrEqualOp: // !(a <= b) → a > b
                    return SageBuilder::buildGreaterThanOp(
                        SageInterface::deepCopy(binOp->get_lhs_operand()),
                        SageInterface::deepCopy(binOp->get_rhs_operand()));
                case V_SgGreaterOrEqualOp: // !(a >= b) → a < b
                    return SageBuilder::buildLessThanOp(
                        SageInterface::deepCopy(binOp->get_lhs_operand()),
                        SageInterface::deepCopy(binOp->get_rhs_operand()));

                // Logical operators
                case V_SgAndOp: { // !(a && b) → (!a || !b), recursively distribute negation
                    auto* lhsNeg = distributeNegation(binOp->get_lhs_operand());
                    auto* rhsNeg = distributeNegation(binOp->get_rhs_operand());
                    return SageBuilder::buildOrOp(lhsNeg, rhsNeg);
                }
                case V_SgOrOp: { // !(a || b) → (!a && !b), recursively distribute negation
                    auto* lhsNeg = distributeNegation(binOp->get_lhs_operand());
                    auto* rhsNeg = distributeNegation(binOp->get_rhs_operand());
                    return SageBuilder::buildAndOp(lhsNeg, rhsNeg);
                }

                default:
                    break;
            }
        }

        // Handle existing negations: Avoid double negation (!!a → a)
        if (auto* notOp = isSgNotOp(expr)) {
            return SageInterface::deepCopy(notOp->get_operand());
        }

        // If it's not a binary operator or existing negation, wrap it with a negation
        return SageBuilder::buildNotOp(SageInterface::deepCopy(expr));
    }

    // Collect the base case conditions
    std::vector<SgExpression *> findBaseCaseConditions(SgFunctionDefinition *funcDef) {
        std::vector<SgExpression *> baseCaseConditions; // Store base case conditions
        std::vector<SgExpression *> conditionStack;     // Track conditions leading to the base case
        bool hasReturnStmt = false;

        auto *funcBody = funcDef->get_body();
        if (!funcBody) return baseCaseConditions;

        std::string funcName = funcDef->get_declaration()->get_name().getString();

        // Helper lambda to traverse statements recursively
        std::function<void(SgStatement *)> traverseStmt = [&](SgStatement *stmt) {
            if (!stmt) return;

            // Handle if-statements
            if (auto *ifStmt = isSgIfStmt(stmt)) {
                // Safely extract the condition
                SgExpression *condition = nullptr;
                if (auto *exprStmt = isSgExprStatement(ifStmt->get_conditional())) {
                    condition = exprStmt->get_expression();
                } else {
                    condition = isSgExpression(ifStmt->get_conditional());
                }

                if (condition) {
                    // Add condition to stack and analyze the true body
                    conditionStack.push_back(condition);
                    traverseStmt(ifStmt->get_true_body());
                    conditionStack.pop_back();

                    // Analyze the false body (else block)
                    if (ifStmt->get_false_body()) {
                        conditionStack.push_back(nullptr); // Indicate "else" (no condition)
                        traverseStmt(ifStmt->get_false_body());
                        conditionStack.pop_back();
                    }
                }
                return;
            }

            // Handle return statements
            if (auto *retStmt = isSgReturnStmt(stmt)) {
                // Check if the return statement contains a recursive call
                if (!containsRecursiveCall(retStmt, funcName)) {
                    hasReturnStmt = true;
                    std::cout << "Found base case return: " << retStmt->unparseToString() << "\n";

                    // Collect the current conditions
                    for (auto *cond : conditionStack) {
                        if (cond) baseCaseConditions.push_back(cond);
                    }
                }
                return;
            }

            // Recursively analyze blocks of statements
            if (auto *block = isSgBasicBlock(stmt)) {
                for (auto *nestedStmt : block->get_statements()) {
                    traverseStmt(nestedStmt);
                }
            }
        };

        // Start traversing from the function body
        traverseStmt(funcBody);

        // Handle void functions with no return statements
        if (!hasReturnStmt) {
            std::cout << "No return statement found. Checking for base case in if-blocks...\n";

            for (auto *stmt : funcBody->get_statements()) {
                if (auto *ifStmt = isSgIfStmt(stmt)) {
                    SgExpression *firstCondition = nullptr;

                    if (auto *exprStmt = isSgExprStatement(ifStmt->get_conditional())) {
                        firstCondition = exprStmt->get_expression();
                    } else {
                        firstCondition = isSgExpression(ifStmt->get_conditional());
                    }

                    if (firstCondition) {
                        std::cout << "Negating the first condition: " << firstCondition->unparseToString() << "\n";
                        auto *negatedCondition = distributeNegation(firstCondition);
                        baseCaseConditions.push_back(negatedCondition);
                        break;
                    }
                }
            }
        }

        return baseCaseConditions;
    }

    // Splits the conditions and places them into a vector as seperate entries
    void splitConditions(SgExpression *expr, std::vector<SgExpression *> &conditions) {
        if (!expr) return;

        if (auto *andOp = isSgAndOp(expr)) {
            // Split at logical AND (&&)
            splitConditions(andOp->get_lhs_operand(), conditions);
            splitConditions(andOp->get_rhs_operand(), conditions);
        } else if (auto *orOp = isSgOrOp(expr)) {
            // Split at logical OR (||)
            splitConditions(orOp->get_lhs_operand(), conditions);
            splitConditions(orOp->get_rhs_operand(), conditions);
        } else {
            // Base case: add the current expression to the list
            conditions.push_back(expr);
        }
    }

    // Mathematically simplify an expression
    std::string simplifyConditionWithSymPy(const std::string &condition) {
        // Execute the Python script and capture output
        std::string command = "python3 src/condition_solver.py \"" + condition + "\"";
        std::string result;
        char buffer[128];
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run Python script.\n";
            return condition; // Return original condition on failure
        }

        // Read the output of the Python script
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);

        return result; // Return the simplified condition
    }


    // Simplify conditions in an expression
    std::vector<std::string> simplifyAndAlignConditions(SgExpression *expr) {
        std::vector<SgExpression *> subConditions;

        // Step 1: Split the expression into subconditions
        splitConditions(expr, subConditions);

        // Step 2: Simplify each subcondition using SymPy
        std::vector<std::string> simplifiedConditions;
        for (auto *cond : subConditions) {
            if (!cond) continue;

            std::string conditionStr = cond->unparseToString();
            std::cout << "Original Condition: " << conditionStr << "\n";

            std::string simplifiedStr = simplifyConditionWithSymPy(conditionStr);
            std::cout << "Simplified Condition: " << simplifiedStr << "\n";

            simplifiedConditions.push_back(simplifiedStr);
        }

        // Step 3: Print the simplified conditions (recombine logic if needed)
        std::cout << "Final Simplified Conditions:\n";
        for (const auto &simplified : simplifiedConditions) {
            std::cout << "  " << simplified << "\n";
        }

        return simplifiedConditions;
    }


    // Go through all base case conditions and simplify them
    std::vector<std::string> simplifyConditions(std::vector<SgExpression *> &baseCaseConditions) {
        std::vector<std::string> baseConditions = {};
        for (auto *cond : baseCaseConditions) {
            if (!cond) continue;

            auto conds = simplifyAndAlignConditions(cond);

            baseConditions.insert( baseConditions.end(), conds.begin(), conds.end() );
        }
        return baseConditions;
    }


    // Gets the variables in a string
    std::set<std::string> extractVariables(const std::string &condition) {
        std::set<std::string> variables;
        std::regex wordRegex(R"(\b[a-zA-Z_]\w*\b)"); // Matches words like "left", "right"
        auto wordsBegin = std::sregex_iterator(condition.begin(), condition.end(), wordRegex);
        auto wordsEnd = std::sregex_iterator();

        for (auto it = wordsBegin; it != wordsEnd; ++it) {
            variables.insert(it->str());
        }

        return variables;
    }


    // Add parenthesis to maintain correct computation order
    static std::string wrap(const std::string &s) {
        return (s.find_first_of("+-*/") != std::string::npos)
            ? "(" + s + ")"
            : s;
    }


    // Extract the lhs of a condition
    std::string extractLHS(const std::string &condition) {
        size_t pos = condition.find('>'); // Works for '>=', '>'
        if (pos == std::string::npos) pos = condition.find('<'); // Works for '<=', '<'
        if (pos == std::string::npos) pos = condition.find('='); // Works for '=='
        if (pos != std::string::npos) {
            return condition.substr(0, pos); // Return LHS before the operator
        }
        return condition; // Return as is if no relational operator
    }


    // Get the name of each of the original parameters
    std::vector<std::string> extractOriginalParameters(SgFunctionDefinition *funcDef) {
        std::vector<std::string> originalParams;

        if (!funcDef) return originalParams;

        // Get the function declaration
        auto *funcDecl = funcDef->get_declaration();
        if (!funcDecl) return originalParams;

        // Get the parameter list
        auto &paramList = funcDecl->get_parameterList()->get_args();

        // Iterate through each parameter and collect its name
        for (auto *param : paramList) {
            if (param) {
                originalParams.push_back(param->get_name().getString());
            }
        }

        return originalParams;
    }


    // Simplify an expression with sympy
    std::string simplifyWithSymPy(const std::string &expression) {
        std::string command = "python3 src/simplifier.py \"" + expression + "\"";
        std::string result;
        char buffer[128];
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run Python script.\n";
            return expression; // Return original expression on failure
        }

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);

        // Remove any trailing newline
        result.erase(result.find_last_not_of("\n") + 1);
        return result;
    }


    // Give an expression a size variable
    std::string getSizeParameter(const std::string &lhs) {
        // Check if the LHS already has a size parameter
        if (lhsToSizeParam.find(lhs) == lhsToSizeParam.end()) {
            // Generate a new size parameter and store it
            std::string sizeParam = "size_" + std::to_string(sizeCounter++);
            lhsToSizeParam[lhs] = sizeParam;
        }
        return lhsToSizeParam[lhs];
    }


    // Get the rhs of a condition
    int getRHS(const std::string cond) {
        try {
            int num = std::stoi(cond);
            return num;
        }
        catch (const std::out_of_range& e) {
            return 0;
        }
        catch (const std::exception& e) {
            return getRHS(cond.substr(1));
        }
    }


    // Simplify an epxression and perform replacements
    std::string simplifyAndReplaceWithSymPy(const std::string &expression,
                                            const std::unordered_map<std::string, std::string> &replacements) {
        // Convert replacements to a properly formatted JSON string
        std::ostringstream replacementStream;
        replacementStream << "{";
        for (auto it = replacements.begin(); it != replacements.end(); ++it) {
            if (it != replacements.begin()) replacementStream << ",";
            replacementStream << std::quoted(it->first) << ":" << std::quoted(it->second);
        }
        replacementStream << "}";

        std::string replacementsJson = replacementStream.str();


        // Build the Python command using a string stream
        std::ostringstream commandStream;
        commandStream << "python3 src/size_finder.py "
                      << std::quoted(expression) << " " << std::quoted(replacementsJson);

        std::string command = commandStream.str(); // Convert the stream to a string
        std::string result;
        char buffer[128];
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run Python script.\n";
            return expression; // Return original expression on failure
        }

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);

        // Remove any trailing newline
        result.erase(result.find_last_not_of("\n") + 1);
        return result;
    }


    // Builds the modifiedArgs and paramToArgsMap that contain information about the parameters
    std::pair<std::vector<std::string>, std::unordered_map<std::string, std::string>> substituteAndAppendArguments(
        const std::vector<std::string> &originalParams,
        const std::vector<std::string> &recursiveArgs,
        const std::vector<std::string> &baseConditions) {

        std::vector<std::string> modifiedArgs; // The final list of arguments
        std::unordered_map<std::string, std::string> paramToArgMap; // Final parameter-to-argument mapping
        std::set<std::string> modifiedVars; // Track which variables have been modified


        // Step 2: Map original parameters to recursive arguments
        for (size_t i = 0; i < originalParams.size(); ++i) {
            if (i < recursiveArgs.size()) {
                paramToArgMap[originalParams[i]] = recursiveArgs[i];
            }
        }

        // Step 3: Process base conditions for LHS substitutions
        for (const auto &baseCondition : baseConditions) {
            auto variables = extractVariables(baseCondition);

            if (variables.size() >= 2) {
                divConq = true;
                std::cout << "Processing base condition: " << baseCondition << "\n";

                // Extract the LHS of the base condition
                std::string lhs = extractLHS(baseCondition);

                // Assign or retrieve the size parameter for this LHS
                std::string sizeParam = getSizeParameter(lhs);

                // Replace the LHS with sizeParam
                std::string modifiedCondition = baseCondition;
                size_t pos = modifiedCondition.find(lhs);

                int cVal = getRHS(modifiedCondition.substr(pos + lhs.length()));
                lhsToBaseValue[lhs] = cVal;


                if (pos != std::string::npos) {
                    modifiedCondition.replace(pos, lhs.length(), sizeParam);
                }

                // Save the modified base condition
                if (uniqueBaseConditions.insert(modifiedCondition).second) {
                    modifiedBaseConditions.push_back(modifiedCondition);
                }

                // Substitute LHS into the arguments
                std::string newArgument = lhs;
                struct Match { size_t start, length; std::string replacement; };
                std::vector<Match> matches;

                for (auto &param : originalParams) {
                    auto it = paramToArgMap.find(param);
                    if (it == paramToArgMap.end()) continue;

                    // \bparam\b to match whole words only (so "left" doesn't catch "leftover")
                    std::regex wordRx("\\b" + param + "\\b");
                    for (std::sregex_iterator i(newArgument.begin(), newArgument.end(), wordRx), end; i != end; ++i) {
                        size_t pos = static_cast<size_t>(i->position());
                        matches.push_back({
                            pos,
                            param.size(),
                            wrap(it->second)
                        });
                        modifiedVars.insert(param);
                    }
                }

                // Sort by start position
                std::sort(matches.begin(), matches.end(),[](auto &a, auto &b){ return a.start < b.start; });

                std::vector<Match> filtered;
                size_t lastEnd = 0;
                for (auto &m : matches) {
                    if (m.start >= lastEnd) {
                        filtered.push_back(m);
                        lastEnd = m.start + m.length;
                    }
                }

                // Sub in
                std::string output;
                output.reserve(newArgument.size());  // avoid repeated realloc
                size_t cur = 0;
                for (auto &m : filtered) {
                    // copy intervening literal text
                    output.append(newArgument, cur, m.start - cur);
                    // splice in the new (wrapped) argument
                    output += m.replacement;
                    cur = m.start + m.length;
                }
                // copy the final tail
                output.append(newArgument, cur, std::string::npos);
    
                // Simplify and replace the LHS with the sizeParam
                newArgument = simplifyWithSymPy(output);

                std::string finalArgument = simplifyAndReplaceWithSymPy(newArgument, {{lhs, sizeParam}});

                // Add the final argument and update mappings
                modifiedArgs.push_back(finalArgument);
                paramToArgMap[sizeParam] = finalArgument;

                // Mark the original LHS variables as modified
                modifiedVars.insert(lhs);
            } else {
                // If the condition is not modified, keep it as is
                if (uniqueBaseConditions.insert(baseCondition).second) {
                    modifiedBaseConditions.push_back(baseCondition);
                }
            }
        }

        // Step 4: Add unmodified arguments
        for (const auto &param : originalParams) {
            if (modifiedVars.find(param) == modifiedVars.end() && paramToArgMap.find(param) != paramToArgMap.end()) {
                std::string value = wrap(paramToArgMap[param]);
                value = simplifyWithSymPy(value);
                modifiedArgs.push_back(value);
            }
        }

        // Clean up paramToArgMap: remove old modified parameters
        for (const auto &param : modifiedVars) {
            paramToArgMap.erase(param);
        }

        return {modifiedArgs, paramToArgMap}; // Return modified arguments and updated mappings
    }


    // Weird way (should be improved) of finding the dimension of array given the ROSE representation of a parameter
    int inferBaseDimension(const std::string &paramType) {
        int dimension = 0;
        size_t pos = 0;

        // Count consecutive '*' after "int" in the string
        pos = paramType.find("int");
        if (pos != std::string::npos) {
            pos += 3; // Move past "int"
            while (pos < paramType.size() && (paramType[pos] == ' ' || paramType[pos] == '*')) {
                if (paramType[pos] == '*') {
                    ++dimension;
                }
                ++pos;
            }
        }
        return dimension; // Return the count of stars
    }


    // Weird way (should be improved) of finding the dimension of vector given the ROSE representation of a parameter
    int extractVectorDimension(const std::string &typeString) {
        int dimension = 0;

        for (char ch : typeString) {
            if (ch == '<') {
                ++dimension; // Increment for each '<'
            } else if (ch == '>') {
                break; // Stop counting once the first '>' is encountered
            }
        }

        // Subtract 1 because the first < corresponds to the base vector declaration
        return (dimension > 1) ? dimension - 1 : 0; 
    }


    // Helper function to check indexing operations in an argument
    int checkIndexingDimension(const std::string &argument) {
        std::regex indexPattern(R"((\w+)(\[[^\]]+\])+)"); // Matches param[n], param[n][m], etc.
        std::smatch match;

        int maxDimension = 0;
        if (std::regex_search(argument, match, indexPattern)) {
            std::string indices = match.str(2);
            int dimension = std::count(indices.begin(), indices.end(), '['); // Count the number of indices
            maxDimension = std::max(maxDimension, dimension);
        }
        return maxDimension;
    }


    // Extract the original definitions of the parameters
    std::vector<std::pair<std::string, std::string>> extractOriginalDefinitions(SgFunctionDefinition *funcDef) {
        std::vector<std::pair<std::string, std::string>> parameters; // Pair of <type, name>

        // Access the parameter list of the function
        auto *paramList = funcDef->get_declaration()->get_parameterList();
        if (!paramList) return parameters;

        // Iterate over the parameters
        for (auto *param : paramList->get_args()) {
            if (auto *initName = isSgInitializedName(param)) {
                std::string type = initName->get_type()->unparseToString(); // Extract type
                std::string name = initName->get_name().getString();        // Extract name
                parameters.emplace_back(type, name);
            }
        }

        return parameters;
    }




    // Finds the dimensionality of all the parameters
    void filterArrayParameters(SgFunctionDefinition *funcDef) {
        std::unordered_map<std::string, int> paramDimensions; // Map to store parameter dimensions

        auto parameters = extractOriginalDefinitions(funcDef); // Extract parameter types and names

        for (const auto &[type, name] : parameters) {
            int dimension = 0;

            // Check if the type is a vector
            if (type.find("std::vector") != std::string::npos) {
                dimension = extractVectorDimension(type);
            } else if (type.find("int *") != std::string::npos) {
                dimension = inferBaseDimension(type); // Simple int * has dimension 1
            }

            // Check arguments for indexing operations to refine dimension
            if (paramToArgsMap.find(name) != paramToArgsMap.end()) {
                for (const auto &arg : paramToArgsMap[name]) {
                    int indexingDimension = checkIndexingDimension(arg);
                    dimension = std::max(dimension, indexingDimension);
                }
            }

            if (dimension > 0) {
                arrDim = dimension;
                paramDimensions[name] = dimension; // Store the detected dimension
                paramToArgsMap.erase(name);       // Remove from paramToArgsMap
            }
        }

        // Print the detected dimensions
        std::cout << "Detected Array/Vector/Matrix Parameters and Dimensions:\n";
        for (const auto &[param, dimension] : paramDimensions) {
            std::cout << "  " << param << " -> Dimension: " << dimension << "\n\n";
        }
    }


    // Builds all the mapping and collects the necessary information before major modifications start
    void processMappings(SgFunctionDefinition *funcDef) {
        std::vector<std::string> originalParams = extractOriginalParameters(funcDef);
        std::vector<std::string> recursiveCall;
        auto conds = findBaseCaseConditions(funcDef);
        std::vector<std::string> baseConditions = simplifyConditions(conds);

        int count = 1;
        for (const auto &recInfo : recursionCalls) {
            recursiveCall = simplifyCall(funcDef, recInfo);
            auto [modifiedArgs, paramMapping] = substituteAndAppendArguments(originalParams, recursiveCall, baseConditions);
            std::cout << "Modified Arguments for Recursive Call " << count << ":\n";
            for (const auto &arg : modifiedArgs) {
                std::cout << "  " << arg << "\n";
            }


            std::cout << "\nParameter to Argument Mapping:\n";
            for (const auto &[param, arg] : paramMapping) {
                std::cout << "  " << param << " -> " << arg << "\n";

                // Update the global mapping (paramToArgsMap)
                if (paramToArgsMap.find(param) == paramToArgsMap.end()) {
                    paramToArgsMap[param] = {}; // Initialize the list if it doesn't exist
                }

                paramToArgsMap[param].push_back(arg);
            }

            count++;
        }

        // Filter out array/vector/matrix parameters and detect dimensions
        filterArrayParameters(funcDef);

        std::cout << "\nModified Base Conditions:\n";
        for (const auto &cond : modifiedBaseConditions) {
            std::cout << "  " << cond << "\n";
        }

        // Output the global parameter-to-arguments mapping
        std::cout << "\nGlobal Parameter to Arguments Mapping:\n";
        for (const auto &[param, args] : paramToArgsMap) {
            std::cout << "  " << param << " -> [";
            for (size_t i = 0; i < args.size(); ++i) {
                std::cout << args[i];
                if (i < args.size() - 1) std::cout << ", ";
            }
            std::cout << "]\n";
        }
    }


    // For Divide and conquer functions. 
    bool analyzeParamToArgsMap_DC(const std::unordered_map<std::string, std::vector<std::string>> &paramToArgsMap) {
        paramAnalysisMap.clear(); // Reset the analysis map

        for (const auto &[param, args] : paramToArgsMap) {
            std::string dominantOp;
            std::vector<int> allOperands;

            for (const auto &arg : args) {
                // Call the Python script to analyze the operation
                std::string command = "python3 src/operations_dc.py \"" + arg + "\"";
                std::string result;
                char buffer[128];

                FILE *pipe = popen(command.c_str(), "r");
                if (!pipe) {
                    std::cerr << "Failed to run Python script.\n";
                    return false;
                }

                while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                    result += buffer;
                }
                pclose(pipe);

                // Strip newline characters from the result
                result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

                // Parse the result: format "operation:operand1,operand2,..."
                size_t sep = result.find(":");
                if (sep == std::string::npos) {
                    std::cerr << "Invalid result from Python script.\n";
                    return false;
                }

                std::string operation = result.substr(0, sep);
                std::string operandsStr = result.substr(sep + 1);

                // Convert operands to integers
                std::stringstream ss(operandsStr);
                std::string item;
                std::vector<int> operands;

                while (std::getline(ss, item, ',')) {
                    try {
                        operands.push_back(std::stoi(item)); // Convert to integer
                    } catch (const std::exception &e) {
                        std::cerr << "Failed to convert operand to integer: " << item << "\n";
                        return false;
                    }
                }

                // Determine the dominant operation
                if (dominantOp.empty()) {
                    dominantOp = operation; // Set the first operation as dominant
                } else if (dominantOp != operation) {
                    std::cerr << "Inconsistent operation detected: " << operation << " != " << dominantOp << "\n";
                    return false; // Inconsistent operations detected
                }

                allOperands.insert(allOperands.end(), operands.begin(), operands.end());
            }

            // Save the result in the global map
            paramAnalysisMap[param] = {dominantOp, allOperands};
        }

        return true;
    }


    // For NOT Divide and conquer functions. 
    bool analyzeParamToArgsMap(const std::unordered_map<std::string, std::vector<std::string>> &paramToArgsMap) {
        for (const auto &[param, args] : paramToArgsMap) {
            std::string dominantOp;
            std::vector<int> allOperands;

            for (const auto &arg : args) {
                // Call the Python script to analyze the operation
                std::string command = "python3 src/operations.py \"" + arg + "\"";
                std::string result;
                char buffer[128];

                FILE *pipe = popen(command.c_str(), "r");
                if (!pipe) {
                    std::cerr << "Failed to run Python script.\n";
                    return false;
                }

                while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                    result += buffer;
                }
                pclose(pipe);

                // Strip newline characters from the result
                result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

                // Parse the result: format "operation:operand1,operand2,..."
                size_t sep = result.find(":");
                if (sep == std::string::npos) {
                    std::cerr << "Invalid result from Python script.\n";
                    return false;
                }

                std::string operation = result.substr(0, sep);
                if (operation == "none") {
                    return false;
                }

                std::string operandsStr = result.substr(sep + 1);

                // Convert operands to integers
                std::stringstream ss(operandsStr);
                std::string item;
                std::vector<int> operands;

                while (std::getline(ss, item, ',')) {
                    try {
                        operands.push_back(std::stoi(item)); // Convert to integer
                    } catch (const std::exception &e) {
                        std::cerr << "Failed to convert operand to integer: " << item << "\n";
                        return false;
                    }
                }

                // Determine the dominant operation
                if (dominantOp.empty()) {
                    dominantOp = operation; // Set the first operation as dominant
                } else if (dominantOp != operation) {
                    std::cerr << "Inconsistent operation detected: " << operation << " != " << dominantOp << "\n";
                    return false; // Inconsistent operations detected
                }

                allOperands.insert(allOperands.end(), operands.begin(), operands.end());
            }

            if (dominantOp == "div" && std::adjacent_find( allOperands.begin(), allOperands.end(), std::not_equal_to<>() ) == allOperands.end() && arrDim > 0) {
                lhsToSizeParam[param] = param;
                divConq = true;
            }

            // Save the result in the global map
            paramAnalysisMap[param] = {dominantOp, allOperands};
        }

        return true;
    }


    // Print some info for the user
    void printOperations() {
        if (analyzeParamToArgsMap(paramToArgsMap)) {
            std::cout << "Parameter Analysis Results:\n";
            for (const auto &[param, result] : paramAnalysisMap) {
                std::cout << "  " << param << " -> (" << result.first << ", [";
                for (size_t i = 0; i < result.second.size(); ++i) {
                    std::cout << result.second[i];
                    if (i < result.second.size() - 1) std::cout << ", ";
                }
                std::cout << "])\n";
            }
        } else {
            std::cout << "Inconsistent operations detected. Analysis failed.\n";
        }
    }


    // Print some info for the user
    void printOperations_DC() {
        if (analyzeParamToArgsMap_DC(paramToArgsMap)) {
            std::cout << "Parameter Analysis Results:\n";
            for (const auto &[param, result] : paramAnalysisMap) {
                std::cout << "  " << param << " -> (" << result.first << ", [";
                for (size_t i = 0; i < result.second.size(); ++i) {
                    std::cout << result.second[i];
                    if (i < result.second.size() - 1) std::cout << ", ";
                }
                std::cout << "])\n";
            }
        } else {
            std::cout << "Inconsistent operations detected. Analysis failed.\n";
        }
    }


    // Makes sure that a function with division updates in a recursive call is suitable for changing
    bool analyzeConditionsForDivOperations() {
        
        // Step 1: Check if any parameter has a dominant "div" operation
        bool hasDivOperation = false;

        for (const auto &[param, analysisResult] : paramAnalysisMap) {
            if (analysisResult.first == "div") {
                hasDivOperation = true;
                break;
            }
        }

        if (!hasDivOperation) {
            // No "div" operations, return true
            return true;
        }

        // Step 2: Analyze the conditions for each recursive call
        for (const auto &recInfo : recursionCalls) {
            for (const auto *condition : recInfo.conditions) {
                if (!condition) continue; // Skip null conditions

                // Cast away constness to query the AST
                auto *nonConstCondition = const_cast<SgExpression*>(condition);

                // Perform a traversal of the condition's AST
                std::vector<SgNode *> nodes = NodeQuery::querySubTree(nonConstCondition, V_SgNode);

                for (const auto *node : nodes) {
                    // Check for array access (SgPntrArrRefExp)
                    if (isSgPntrArrRefExp(node)) {
                        std::cout << "Detected array access in condition.\n";
                        return false;
                    }
                    // Check for subtraction (SgSubtractOp)
                    if (isSgSubtractOp(node)) {
                        std::cout << "Detected subtraction in condition.\n";
                        return false;
                    }
                    // Check for modulo (SgModOp)
                    if (isSgModOp(node)) {
                        std::cout << "Detected modulo operation in condition.\n";
                        return false;
                    }
                }
            }
        }

        // If no invalid conditions are found, return true
        return true;
    }


    // Analyses the parameters to find the stack sizes for each
    std::tuple<bool, int, std::unordered_map<std::string, int>, std::vector<int>>
    runIterationAnalysis(const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> &paramAnalysisMap) {
        // Prepare input for Python
        std::ostringstream inputStream;
        for (const auto &[param, result] : paramAnalysisMap) {
            inputStream << param << ":" << result.first;
            for (const auto &operand : result.second) {
                inputStream << "," << operand;
            }
            inputStream << " ";
        }
        std::string inputStr = inputStream.str();
        inputStr.pop_back(); // Remove trailing space

        // Execute Python script
        std::string command = "python3 src/iteration_analysis.py \"" + inputStr + "\"";
        std::string resultStr;
        char buffer[256];

        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("Failed to execute Python script.");
        }

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            resultStr += buffer;
        }
        pclose(pipe);

        // Remove trailing newline and whitespace
        resultStr.erase(resultStr.find_last_not_of(" \n\r\t") + 1);

        std::cout << "Raw Python Output: [" << resultStr << "]\n";  // Debug print

        // Parse the output manually
        std::istringstream resultStream(resultStr);
        bool resultBool = false;
        int stackSize = 0;
        std::unordered_map<std::string, int> basisMap;
        std::vector<int> newMapping;

        std::string token;
        std::vector<std::string> tokens;

        while (resultStream >> token) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) {
            throw std::runtime_error("Invalid output format from Python script.");
        }

        resultBool = (tokens[0] == "True");
        stackSize = std::stoi(tokens[1]);

        size_t i = 2;
        for (; i < tokens.size(); ++i) {
            auto pos = tokens[i].find(":");
            if (pos == std::string::npos) break;

            std::string key = tokens[i].substr(0, pos);
            std::string value = tokens[i].substr(pos + 1);
            basisMap[key] = std::stoi(value);
        }

        for (; i < tokens.size(); ++i) {
            newMapping.push_back(std::stoi(tokens[i]));
        }

        return {resultBool, stackSize, basisMap, newMapping};
    }


    // Function to map recursion calls to values in new_mapping
    std::unordered_map<SgFunctionCallExp *, int> mapRecursionCallsToValues(const std::vector<int> &new_mapping) {

        std::unordered_map<SgFunctionCallExp *, int> callToValueMap;

        // Ensure the sizes of recursionCalls and new_mapping match
        if (recursionCalls.size() != new_mapping.size()) {
            throw std::runtime_error("Mismatch between recursionCalls and new_mapping sizes.");
        }

        // Map each recursive call to its corresponding value
        for (size_t i = 0; i < recursionCalls.size(); ++i) {
            callToValueMap[recursionCalls[i].call] = new_mapping[i];
        }

        return callToValueMap;
    }





    // Calls all the required functions for extracting information related to the iterative function to be built
    void processIteration() {
        auto [resultBool, stack_size, basis_map , update_map] = runIterationAnalysis(paramAnalysisMap);
        simp = resultBool;

        if (simp) {
            std::cout << "Iteration Analysis Results:\n";
        
            // Print the boolean result
            std::cout << "  Result (bool): " << (resultBool ? "True" : "False") << "\n";

            // Print the first maximum value
            std::cout << "  Stack Size: " << stack_size << "\n";

            // Print the basis map
            std::cout << "  Basis Map:\n";
            for (const auto &[param, basis] : basis_map) {
                std::cout << "    " << param << " -> " << basis << "\n";
            }

            std::cout << "New Mapping: [";
            for (size_t i = 0; i < update_map.size(); ++i) {
                std::cout << update_map[i];
                if (i < update_map.size() - 1) std::cout << ", ";
            }
            std::cout << "]\n\n";

            auto call_map = mapRecursionCallsToValues(update_map);
            for (const auto &[call, value] : call_map) {
                std::cout << "Recursive Call at address: " << call 
                        << " -> Value: " << value << "\n";
            }
        }
        else {
            std::cout << "Not Simplifiable\n";
        }
        
    }


    // Print some info for the user
    void printRecursionCalls() {
        std::cout << "Non-Tail Recursive Calls and Conditions:\n";
        for (const auto &recInfo : recursionCalls) {
            std::cout << "Recursive Call: " << recInfo.call->unparseToString() << "\n";
            std::cout << "Conditions Leading to This Call:\n";
            for (auto *cond : recInfo.conditions) {
                std::cout << "  - " << cond->unparseToString() << "\n";
            }
            std::cout << "Number of Conditions: " << recInfo.conditions.size() << "\n";
            std::cout << "--------------------------\n";
        }
    }
};


// The next following structs and functions are helpers that are used for debugging
// They all check prerequistes that ROSE needs to finish transforming an AST into code
// struct ParentChecker : public AstSimpleProcessing {
//     void visit(SgNode *node) override {
//         if (!node) return; // Safety check

//         if (isSgProject(node)) return;

//         // Check if the node has a parent
//         if (!node->get_parent()) {
//             std::cerr << "Error: Node of type " << node->class_name() << " has no parent!" << std::endl;

//             // Try printing the node's content (if possible)
//             std::string nodeStr;
//             try {
//                 nodeStr = node->unparseToString();
//             } catch (...) {
//                 nodeStr = "[unparseToString() failed]";
//             }

//             std::cerr << "  Node content: " << nodeStr << std::endl;
//         }
//     }
// };

// struct CheckInitializedNames : public AstSimpleProcessing {
//     void visit(SgNode *node) override {
//         if (auto *initName = isSgInitializedName(node)) {
//             if (!initName->get_parent()) {
//                 std::cerr << "Error: SgInitializedName '" << initName->get_name().getString()
//                           << "' has no parent!" << std::endl;
//             }
//         }
//     }
// };

// struct CheckVariableDeclarations : public AstSimpleProcessing {
//     void visit(SgNode *node) override {
//         if (auto *varDecl = isSgVariableDeclaration(node)) {
//             if (!varDecl->get_parent()) {
//                 std::cerr << "Error: SgVariableDeclaration has no parent: "
//                           << varDecl->unparseToString() << std::endl;
//             }
//         }
//     }
// };


// struct PrintAllInitializedNames : public AstSimpleProcessing {
//     void visit(SgNode *node) override {
//         if (auto *initName = isSgInitializedName(node)) {
//             std::cout << "Variable: " << initName->get_name().getString()
//                       << " in " << node->get_parent()->class_name()
//                       << std::endl;
//         }
//     }
// };


// Main Function
int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    // Initialize the ROSE framework
    SgProject *project = frontend(argc - 1, argv);

    // Apply the transformation
    RecursionToIteration transformer;
    transformer.traverseInputFiles(project, preorder);

    // ParentChecker checker;
    // checker.traverse(project, preorder);

    // CheckInitializedNames checker2;
    // checker2.traverse(project, preorder);

    // CheckVariableDeclarations checker3;
    // checker3.traverse(project, preorder);

    // PrintAllInitializedNames checker4;
    // checker4.traverse(project, preorder);

    // AstDOTGeneration dotGen;
    // dotGen.generate(project, "ast_debug");

    std::string output_filename = argv[argc - 1];
    for (SgFile* file : project->get_fileList()) {
        file->set_unparse_output_filename(output_filename);
    }

    // Write out the transformed files
    backend(project);

    return 0;
}
