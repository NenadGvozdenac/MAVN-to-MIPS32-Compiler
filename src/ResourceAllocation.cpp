// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#include "ResourceAllocation.h"

#include <list>

bool doResourceAllocation(SimplificationStack& simplificationStack, InterferenceGraph interferenceGraph)
{
	Variables colored;						// We want the colored variables
	while (!simplificationStack.empty())
	{
		std::list<Regs> regs;				// Always create a new list of registers
		regs.push_back(t0);					
		regs.push_back(t1);
		regs.push_back(t2);
		regs.push_back(t3);
		regs.push_back(t4);

		Variable* var = simplificationStack.top();	// Get the top variable, and remove it [pop it]
		simplificationStack.pop();

		if (!colored.empty())
		{
			for (Variable* a : colored)
			{
				if (interferenceGraph.getMatrix()[var->getPosition()][a->getPosition()] == __INTERFERENCE__)
				{
					std::cout << a->getVariableAssignment() << std::endl;
					regs.remove(a->getVariableAssignment());	// This means just remove the first element from the registers
																// Iterate and whether its the second in iteration, remove the second, etc.
				}
			}
		}

		if (!regs.empty())										// Should never be empty, and should push it into variables
		{
			var->setVariableAssignment(regs.front());
			colored.push_back(var);
		}
		else {
			return false;										// This means that spill was detected
		}
	}

	return true;
}