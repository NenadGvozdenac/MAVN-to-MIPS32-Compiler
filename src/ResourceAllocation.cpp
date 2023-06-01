#include "ResourceAllocation.h"

#include <list>

using std::list;

bool doResourceAllocation(SimplificationStack& simplificationStack, InterferenceGraph interferenceGraph)
{
	Variables colored;
	while (!simplificationStack.empty())
	{
		list<Regs> regs;
		regs.push_back(t0);
		regs.push_back(t1);
		regs.push_back(t2);
		regs.push_back(t3);
		regs.push_back(t4);

		Variable* var = simplificationStack.top();
		simplificationStack.pop();

		if (!colored.empty())
		{
			for (Variable* a : colored)
			{
				if (interferenceGraph.getMatrix()[var->getPosition()][a->getPosition()] == __INTERFERENCE__)
				{
					std::cout << a->getVariableAssignment() << std::endl;
					regs.remove(a->getVariableAssignment());
				}
			}

		}

		if (!regs.empty())
		{
			var->setVariableAssignment(regs.front());
			colored.push_back(var);
		}
		else {
			return false;
		}
	}

	return true;
}