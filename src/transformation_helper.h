#ifndef TRANSFORMATION_HELPER_H
#define TRANSFORMATION_HELPER_H

#include <rose.h>
#include <string>
#include <unordered_map>
#include <vector>


void printFunctionSymbolTable(SgFunctionDefinition *funcDef);

void copySymbolTable(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock);

void prependStatementsFromBlock(SgBasicBlock *sourceBlock, SgBasicBlock *targetBlock);

void createNewFunctionBody(SgFunctionDefinition *funcDef,
                                    int stackSize,
                                    const std::unordered_map<std::string, int> &basisMap,
                                    const std::vector<int> &newMapping,
                                    const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> &paramAnalysisMap,
                                    const std::unordered_map<SgFunctionCallExp*, int> &callToValueMap,
                                    const std::vector<SgExpression *> &baseCaseConditions);

SgFunctionDeclaration* createArrayShiftFunction(SgGlobal *globalScope);
SgFunctionDeclaration* createPowFunction(SgGlobal *globalScope);
SgFunctionDeclaration* createIterFindFunction(SgGlobal *globalScope);


void ensureGlobalIncludes(SgSourceFile *sourceFile);

void defineNumIterSubAndDiv(SgBasicBlock *newBody,
                            const std::vector<std::string> &subVarNames,
                            const std::vector<std::string> &divVarNames,
                            std::vector<SgStatement*> &subStatements,
                            std::vector<SgStatement*> &divStatements);

void defineNumIterLogic(SgBasicBlock *newBody,
                        const std::unordered_map<std::string, int> &basisMap,
                        const std::vector<std::string> &subVarNames,
                        const std::vector<std::string> &divVarNames,
                        SgFunctionDefinition *funcDef,
                        int stackSize,
                        const std::vector<SgExpression *> &baseCaseConditions,
                        SgBasicBlock *oldBody);

void replaceReturnStatements(SgBasicBlock *funcBody, int stackSize, SgBasicBlock *newBody);

void replaceRecursiveCalls(SgBasicBlock *funcBody,
    int stackSize,
    const std::unordered_map<SgFunctionCallExp *, int> &callToValueMap, 
    SgBasicBlock *newBody);

void createForLoop(SgFunctionDefinition *funcDef,
                              SgBasicBlock *modifiedFuncBody,
                              const std::unordered_map<std::string, int> &basisMap,
                              const std::vector<std::string> &subVarNames,
                              const std::vector<std::string> &divVarNames);

void replaceParamsWithTempVars(SgExpression *expr, const std::unordered_map<std::string, std::string> &tempVars);

SgBasicBlock* extractAndModifyBaseCase(SgBasicBlock *funcBody,
                                      int stackSize,
                                      const std::vector<SgExpression *> &baseCaseConditions,
                                      const std::unordered_map<std::string, std::string> &tempVars, 
                                      SgBasicBlock *newBody);

#endif 
