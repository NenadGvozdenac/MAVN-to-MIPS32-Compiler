// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#pragma once

#include "IR.h"
#include <stack>
#include "InterferenceGraph.h"

// SimplificationStack defined as a stack of Variables*
typedef std::stack<Variable*> SimplificationStack;

// Function that does simplification on registers, potentionally spills
// @param InterferenceGraph ig
// @param int registerNumber
// @return SimplificationStack
SimplificationStack doSimplification(InterferenceGraph ig, int registerNumber);

bool checkSpill(Variables variables, int size);