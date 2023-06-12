// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#include "SimplificationStack.h"

// Function that checks whether the register has the same position as the one in the register
// @param std::vector<int> vec
// @param int position
// @return boolean: if the register is in the removed register?
bool inRemoved(std::vector<int> vec, int position) 
{
	for (auto i = vec.begin(); i < vec.end(); i++) 
	{
		if ((*i) == position) 
		{
			return true;
		}
	}
	return false;
}

SimplificationStack doSimplification(InterferenceGraph interferenceGraph, int registerNumber) 
{
	int maxPosition = -1;														// Position of the stack	[-1 default]
	int numberOfNeighbours = -1;												// numberOfNeighbours		[-1 default]
	int size = interferenceGraph.getVariables().size();							// size of the matrix
	int counter;																
	std::vector<int> removedPositions;											

	SimplificationStack ss;														// SimplificationStack, we will be filling up

	while (size > 0)
	{
		maxPosition = -1;
		numberOfNeighbours = -1;
		for (int i = 0; i < interferenceGraph.getMatrix().size(); i++) 
		{
			if (inRemoved(removedPositions, i))									// If its removed, just continue
			{
				continue;
			}
			counter = 0;

			for (int j = 0; j < interferenceGraph.getMatrix().size(); j++)		
			{
				if (interferenceGraph.getMatrix()[i][j] == __INTERFERENCE__ && !inRemoved(removedPositions, j))	// If its not removed AND is in interference
					counter++;																					// Increase counter
			}

			if (counter > numberOfNeighbours && counter < registerNumber)
			{
				numberOfNeighbours = counter;
				maxPosition = i;
			}
		}

		for (auto it = interferenceGraph.getVariables().begin(); it != interferenceGraph.getVariables().end(); it++)
		{
			if ((*it)->getPosition() == maxPosition) 
			{
				std::cout << (*it)->getName() << " " << (*it)->getPosition() << std::endl;
				ss.push(std::shared_ptr<Variable>(*it));
			}
		}
		removedPositions.push_back(maxPosition);								// Push into removed positions the new maxPosition

		size--;
	}

	return ss;
}

bool checkSpill(Variables variables, int size) {
	int numberOfRegisters = 0;
	for (auto it = variables.begin(); it != variables.end(); it++) {
		if ((*it)->getVariableType() == Variable::VariableType::REG_VAR) {
			numberOfRegisters++;
		}
	}

	return numberOfRegisters != size;
}