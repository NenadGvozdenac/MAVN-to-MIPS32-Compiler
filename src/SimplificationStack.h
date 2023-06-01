#pragma once

#include "IR.h"
#include <stack>
#include "InterferenceGraph.h"

typedef std::stack<Variable*> SimplificationStack;

SimplificationStack doSimplification(InterferenceGraph ig, int degree);