#pragma once
#include "SimplificationStack.h"

// Function used to do the resource allocation on a simplification stack
// @param SimplificationStack& simplificationStack
// @param InterferenceGraph ig
// @return boolean: resource allocation successful?
bool doResourceAllocation(SimplificationStack& simplificationStack, InterferenceGraph ig);