#include "SimplificationStack.h"

bool inRemoved(std::vector<int> vec, int position) {
	for (auto i = vec.begin(); i < vec.end(); i++) {
		if ((*i) == position) {
			return true;
		}
	}
	return false;
}

SimplificationStack doSimplification(InterferenceGraph interferenceGraph, int degree) {
	int maxPosition = -1;
	int numberOfNeighbourss = -1;
	int size = interferenceGraph.getVariables().size();
	int counter;
	std::vector<int> removedPositions;

	SimplificationStack ss;

	while (size > 0) 
	{
		maxPosition = -1;
		numberOfNeighbourss = -1;
		for (int i = 0; i < interferenceGraph.getMatrix().size(); i++) 
		{
			if (inRemoved(removedPositions, i))
			{
				continue;
			}
			counter = 0;

			for (int j = 0; j < interferenceGraph.getMatrix().size(); j++)
			{
				if (interferenceGraph.getMatrix()[i][j] == __INTERFERENCE__ && !inRemoved(removedPositions, j))
					counter++;
			}

			if (counter > numberOfNeighbourss && counter < degree)
			{
				numberOfNeighbourss = counter;
				maxPosition = i;
			}
		}

		for (auto it = interferenceGraph.getVariables().begin(); it != interferenceGraph.getVariables().end(); it++)
		{
			if ((*it)->getPosition() == maxPosition) 
			{
				std::cout << (*it)->getName() << " " << (*it)->getPosition() << std::endl;
				ss.push(*it);
			}
		}
		removedPositions.push_back(maxPosition);

		size--;
	}

	return ss;
}