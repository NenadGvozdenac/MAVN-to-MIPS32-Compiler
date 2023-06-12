// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#include "InterferenceGraph.h"

InterferenceGraph::InterferenceGraph(Variables v, Instructions& inst) : m_variables(calculateRegisterVariables(v))
{
	makeInterferenceGraph(inst);
}

InterferenceMatrix& InterferenceGraph::getMatrix() 
{
	return m_matrix;
}

Variables& InterferenceGraph::getVariables() 
{
	return *m_variables;
}

Variables* InterferenceGraph::calculateRegisterVariables(Variables& v) 
{
	Variables* vars = new Variables();
	int position = 0;

	for (auto it = v.begin(); it != v.end(); it++) 
	{
		if ((*it)->getVariableType() == Variable::REG_VAR) 
		{
			(*it)->setPosition(position++);
			vars->push_back((*it));
		}
	}

	return vars;
}

void InterferenceGraph::makeInterferenceGraph(Instructions& instructions) 
{
	this->getMatrix().resize(this->getVariables().size());

	for (auto iterator = this->getMatrix().begin(); iterator != this->getMatrix().end(); iterator++)
	{
		iterator->resize(this->getVariables().size(), __EMPTY__);
	}

	for (std::shared_ptr<Instruction> instruction : instructions)
	{
		for (std::shared_ptr<Variable> variableA : instruction->m_def)
		{
			for (std::shared_ptr<Variable> variableB : instruction->m_out)
			{
				if (variableA->getPosition() != variableB->getPosition() && 
					variableA->getVariableType() != Variable::MEM_VAR && 
					variableB->getVariableType() != Variable::MEM_VAR)
				{
					this->getMatrix()[variableA->getPosition()][variableB->getPosition()] = __INTERFERENCE__;
					this->getMatrix()[variableB->getPosition()][variableA->getPosition()] = __INTERFERENCE__;
				}
			}
		}
	}
}

void InterferenceGraph::printInterferenceMatrix() 
{

	std::cout << "Interference Matrix: " << std::endl;

	std::string t = "--------";

	for (int i = 0; i < this->getVariables().size() + 1; i++) 
	{
		std::cout << t;
	}

	std::cout << "\n";

	std::cout << "\t";
	for (auto iteratorVariable = this->getVariables().begin(); iteratorVariable != this->getVariables().end(); iteratorVariable++)
	{
		std::cout << (*iteratorVariable)->getName() << "\t";
	}

	std::cout << "\n";
	
	auto iteratorVariable = this->getVariables().begin();
	for (auto iteratorOne = this->getMatrix().begin(); iteratorOne != this->getMatrix().end(); iteratorOne++)
	{
		std::cout << (*iteratorVariable)->getName() << "\t";
		iteratorVariable++;
		for (auto iteratorTwo = (*iteratorOne).begin(); iteratorTwo != (*iteratorOne).end(); iteratorTwo++) 
		{
			std::cout << (*iteratorTwo) << "\t";
		}
		std::cout << "\n";
	}

	for (int i = 0; i < this->getVariables().size() + 1; i++)
	{
		std::cout << t;
	}

	std::cout << std::endl << std::endl;
}