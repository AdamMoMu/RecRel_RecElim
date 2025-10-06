#ifndef TAIL_H
#define TAIL_H

#include "rose.h"

void wrapFunc(SgFunctionDefinition *funcDef);

void transformTR(SgFunctionDefinition *funcDef);

void trTailStmt(SgFunctionCallExp *funcCall, SgFunctionDefinition *funcDef);

#endif