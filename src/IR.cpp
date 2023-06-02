#include "IR.h"

/*
std::string& Instruction::getLabelStr()
{
	return this->m_labelStr;
}

Label*& Instruction::getLabel()
{
	return this->m_label;
}

int& Instruction::getPosition()
{
	return this->m_position;
}

InstructionType& Instruction::getInstructionType()
{
	return this->m_type;
}

std::string& Instruction::getInstructionTemplate()
{
	return this->m_instructionTemplate;
}

int& Instruction::getNumber()
{
	return this->m_number;
}

Variables& Instruction::getSrc()
{
	return m_src;
}

Variables& Instruction::getDst()
{
	return m_dst;
}

Variables& Instruction::getUse()
{
	return m_src;
}

Variables& Instruction::getDef()
{
	return m_dst;
}

Variables& Instruction::getIn()
{
	return m_src;
}

Variables& Instruction::getOut()
{
	return m_dst;
}

std::list < Instruction* >& Instruction::getSucc() 
{
	return m_succ;
}

std::list < Instruction* >& Instruction::getPred()
{
	return m_pred;
}

void Instruction::setLabelStr(std::string labelStr)
{
	this->m_labelStr = labelStr;
}

void Instruction::setLabel(Label* label)
{
	this->m_label = label;
}

void Instruction::setPosition(int position)
{
	this->m_position = position;
}

void Instruction::setType(InstructionType instructionType)
{
	this->m_type = instructionType;
}

void Instruction::setNumber(int number)
{
	this->m_number = number;
}

void Instruction::setDst(Variables variables)
{
	this->m_dst = variables;
}

void Instruction::setSrc(Variables variables)
{
	this->m_src = variables;
}

void Instruction::setUse(Variables variables)
{
	this->m_use = variables;
}

void Instruction::setDef(Variables variables)
{
	this->m_def = variables;
}

void Instruction::setIn(Variables variables)
{
	this->m_in = variables;
}

void Instruction::setOut(Variables variables)
{
	this->m_out = variables;
}

void Instruction::setSucc(std::list<Instruction*> instructions)
{
	this->m_succ = instructions;
}

void Instruction::setPred(std::list<Instruction*> instructions)
{
	this->m_pred = instructions;
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
	if (m_type == InstructionType::I_ADD)
	{
		return "add";
	}
	else if (m_type == InstructionType::I_ADDI)
	{
		return "addi";
	}
	else if (m_type == InstructionType::I_B)
	{
		return "b";
	}
	else if (m_type == InstructionType::I_BLTZ)
	{
		return "bltz";
	}
	else if (m_type == InstructionType::I_LA)
	{
		return "la";
	}
	else if (m_type == InstructionType::I_LI)
	{
		return "li";
	}
	else if (m_type == InstructionType::I_LW)
	{
		return "lw";
	}
	else if (m_type == InstructionType::I_NOP)
	{
		return "nop";
	}
	else if (m_type == InstructionType::I_NOR)
	{
		return "nor";
	}
	else if (m_type == InstructionType::I_OR)
	{
		return "or";
	}
	else if (m_type == InstructionType::I_SLT)
	{
		return "slt";
	}
	else if (m_type == InstructionType::I_SUB)
	{
		return "sub";
	}
	else if (m_type == InstructionType::I_SW)
	{
		return "sw";
	}
}

*/

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

using std::cout;

void printAll(Variables variables, Instructions instructions, Labels labels)
{
	for (auto it = variables.begin(); it != variables.end(); ++it) 
	{
		cout << "VARIABLE: \t" << (*it)->getName() << " -> " << (*it)->getVariableAssignment() << std::endl;
	}

	std::cout << "\n";

	for (auto it = instructions.begin(); it != instructions.end(); it++) 
	{
		std::cout << "POSITION: " << (*it)->m_position;
		cout << "\tINSTRUCTION: \t";
		cout << (*it)->getTypeToString() << " ";

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
		cout << "LABEL: \t\t";
		cout << (*it)->getNameOfLabel() << std::endl;
	}

	cout << std::endl;
}