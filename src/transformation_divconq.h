#ifndef TRANSFORMATION_DIVCONQ_H
#define TRANSFORMATION_DIVCONQ_H

#include <rose.h>
#include <string>
#include <unordered_map>
#include <vector>



void printFunctionSymbolTable_dc(SgFunctionDefinition *funcDef);

void copySymbolTable_dc(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock);

void prependStatementsFromBlock_dc(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock);

void createVariableFromMap_dc(const std::string &param,
                                   const std::unordered_map<std::string, std::string> &lhsToSizeParam,
                                   SgBasicBlock *scope);

// Transformation helper functions
void createNewFunctionBody_dc(SgFunctionDefinition *funcDef,
                                    int stackSize,
                                    const std::unordered_map<std::string, int> &basisMap,
                                    const std::vector<int> &newMapping,
                                    const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> &paramAnalysisMap,
                                    const std::unordered_map<SgFunctionCallExp*, int> &callToValueMap,
                                    const std::vector<SgExpression *> &baseCaseConditions,
                                    std::unordered_map<std::string, std::string> lhsToSizeParam, 
                                    const std::unordered_map<std::string, int> lhsToBaseValue);


SgFunctionDeclaration* createPowFunction_dc(SgGlobal *globalScope);
SgFunctionDeclaration* createIterFindFunction_dc(SgGlobal *globalScope);

void ensureGlobalIncludes_dc(SgSourceFile *sourceFile);

void defineNumIterSubAndDiv_dc(SgBasicBlock *newBody,
                            const std::vector<std::string> &subVarNames,
                            const std::vector<std::string> &divVarNames);

void defineNumIterLogic_dc(SgBasicBlock *newBody,
                        const std::unordered_map<std::string, int> &basisMap,
                        const std::vector<std::string> &subVarNames,
                        const std::vector<std::string> &divVarNames,
                        SgFunctionDefinition *funcDef,
                        int stackSize,
                        const std::vector<SgExpression *> &baseCaseConditions,
                        SgBasicBlock *oldBody);

void deleteReturnStatements_dc(SgBasicBlock *funcBody);

void deleteRecursiveCalls_dc(SgBasicBlock *funcBody,
                           const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap);

void createForLoops_dc(SgFunctionDefinition *funcDef,
                              SgBasicBlock *modifiedFuncBody,
                              const std::unordered_map<std::string, int> &basisMap,
                              const std::vector<std::string> &subVarNames,
                              const std::vector<std::string> &divVarNames,
                              std::unordered_map<std::string, std::string> lhsToSizeParam,
                              const std::unordered_map<std::string, int> lhsToBaseValue);
                                    
std::pair<std::string, std::string> analyzeLHSWithSymPy(const std::string &lhs);




#endif // TRANSFORMATION_HELPER_H
