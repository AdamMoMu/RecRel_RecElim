#ifndef HELPER_H
#define HELPER_H

#include "rose.h"

bool areEquivalentExpression(SgExpression *expr1, SgExpression *expr2);

std::set<std::string> getVariableNames(SgExpression *expr);

SgExpression* replaceVarsInExpression(SgExpression* expr, const std::unordered_map<std::string, SgExpression*>& replacements);

std::set<std::string> getReferencedVariables(SgExprListExp *args);

std::vector<std::string> simplifyArgumentsWithSymPy(const std::vector<std::string> &args);


#endif