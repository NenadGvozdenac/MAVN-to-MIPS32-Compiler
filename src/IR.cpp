// Autor: Nenad Gvozdenac, Datum: 07.06.2023

#include "IR.h"

std::string Label::getNameOfLabel() {
	return this->name;
}

int Label::getPosition() {
	return this->position;
}

std::string Instruction::outputList(std::list<Instruction*> list)
{
	std::string str("");

	for (Instruction* el : list)
	{
		Variables vb = el->getSrc();
		for (Variable* v : vb)
		{
			str += std::to_string(v->getPosition()) + " ";
		}
	}

	return str;
}

std::string Instruction::getTypeToString()
{
	switch (m_type) {
		case I_ADD:		return "add";
		case I_ADDI:	return "addi";
		case I_B:		return "b";
		case I_BLTZ:	return "bltz";
		case I_LA:		return "la";
		case I_LI:		return "li";
		case I_LW:		return "lw";
		case I_NOP:		return "nop";
		case I_SUB:		return "sub";
		case I_SW:		return "sw";
		case I_ADDU:	return "addu";
		case I_OR:		return "or";
		case I_LB:		return "lb";
	}
}

bool labelExists(Labels labels, Label* label) {
	for (auto& el : labels) {
		if (el->getNameOfLabel() == label->getNameOfLabel()) {
			return true;
		}
	}

	return false;
}

Variable::VariableType& Variable::getVariableType() 
{
	return this->m_variableType;
}

void Variable::setVariableType(VariableType type) 
{
	this->m_variableType = type;
}

std::string& Variable::getName()
{
	return this->m_variableName;
}

void Variable::setName(std::string name) 
{
	this->m_variableName = name;
}

int& Variable::getPosition()
{
	return this->m_variablePosition;
}

void Variable::setPosition(int position)
{
	this->m_variablePosition = position;
}

Regs& Variable::getVariableAssignment() 
{
	return this->m_variableAssignment;
}

void Variable::setVariableAssignment(Regs regs) 
{
	this->m_variableAssignment = regs;
}

int& Variable::getValue() 
{
	return this->m_value;
}

void Variable::setValue(int value) 
{
	this->m_value = value;
}

void Instruction::normalizeAssignmentsToVariables(std::list<Instruction*>& instructions, Variables variables) 
{
	for (auto it = instructions.begin(); it != instructions.end(); it++) 
	{
		for (auto& el : (*it)->m_dst) 
		{
			std::string variableName = el->getName();

			for (auto& el2 : variables) 
			{
				if (el2->getName() == variableName) 
				{
					el->setVariableAssignment(el2->getVariableAssignment());
					break;
				}
			}
		}

		for (auto& el : (*it)->m_src) 
		{
			std::string variableName = el->getName();

			for (auto& el2 : variables) 
			{
				if (el2->getName() == variableName) 
				{
					el->setVariableAssignment(el2->getVariableAssignment());
					break;
				}
			}
		}
	}
}

void printAll(Variables variables, Instructions instructions, Labels labels)
{
	for (auto it = variables.begin(); it != variables.end(); ++it) 
	{
		std::cout << "VARIABLE: \t" << (*it)->getName() << " -> " << (*it)->getVariableAssignment() << std::endl;
	}

	std::cout << "\n";

	for (auto it = instructions.begin(); it != instructions.end(); it++) 
	{
		std::cout << "POSITION: " << (*it)->m_position;
		std::cout << "\tINSTRUCTION: \t";
		std::cout << (*it)->getTypeToString() << " ";

		for (Variable* el : (*it)->getDst()) 
		{
			std::cout << el->getName() << " ";
		}

		for (Variable* el : (*it)->getSrc()) 
		{
			std::cout << el->getName() << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n";

	for (auto it = labels.begin(); it != labels.end(); it++) 
	{
		std::cout << "LABEL: \t\t";
		std::cout << (*it)->getNameOfLabel() << std::endl;
	}

	std::cout << std::endl;
}