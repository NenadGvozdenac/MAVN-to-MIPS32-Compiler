#ifndef __IR__
#define __IR__

#include "Types.h"
#include <string>


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : 
		m_variableType(VariableType::NO_TYPE), 
		m_variableName(""),
		m_variablePosition(-1),
		m_variableAssignment(Regs::NO_ASSIGN) {}
	
	Variable(std::string name, int pos, VariableType variableType) : 
		m_variableName(name),
		m_variablePosition(pos),
		m_variableType(variableType),
		m_variableAssignment(Regs::NO_ASSIGN) {
	}

	VariableType& Variable::getVariableType();

	std::string& Variable::getName();

	int& Variable::getPosition();

	Regs& Variable::getVariableAssignment();

	int& Variable::getValue();

	void Variable::setVariableType(VariableType type);

	void Variable::setName(std::string name);

	void Variable::setPosition(int position);

	void Variable::setVariableAssignment(Regs regs);

	void Variable::setValue(int value);

private:
	VariableType m_variableType;
	std::string m_variableName;
	int m_variablePosition;
	Regs m_variableAssignment;
	int m_value;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;

/**
 * This class represents one instruction in program code.
 */
class Label { 
private:
	std::string name;
	int position;
public:
	Label() : name("") {}
	Label(int position, std::string name) : name(name), position(position) {}

	std::string getNameOfLabel() 
	{
		return name;
	}

	int getPosition() 
	{
		return position;
	}
};

typedef std::list<Label*> Labels;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (std::string label, int pos, InstructionType type, Variables& dst, Variables& src, std::string instructionTemplate) :
		m_labelStr(label),  
		m_position(pos), 
		m_type(type), 
		m_dst(dst), 
		m_src(src),
		m_def(dst),
		m_use(src),
		m_instructionTemplate(instructionTemplate)
	{}

	Variables getSrc() 
	{
		return m_src;
	}

	Variables getDst() 
	{
		return m_dst;
	}

	static std::string outputList(std::list<Instruction*> list) 
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

	InstructionType getType() 
	{
		return this->m_type;
	}

	std::string getTypeToString() 
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
		else if (m_type == InstructionType::I_OR) 
		{
			return "or";
		}
		else if (m_type == InstructionType::I_SUB) 
		{
			return "sub";
		}
		else if (m_type == InstructionType::I_SW) 
		{
			return "sw";
		}
		else if (m_type == InstructionType::I_ADDU) {
			return "addu";
		}
		else if (m_type == InstructionType::I_OR) {
			return "or";
		}
		else if (m_type == InstructionType::I_LB) {
			return "lb";
		}
	}

	static void normalizeAssignmentsToVariables(std::list<Instruction*>& instructions, Variables variables);

public:
	std::string m_labelStr;
	Label* m_label;
	int m_position;
	InstructionType m_type;
	std::string m_instructionTemplate;

	Variables m_dst;
	Variables m_src;
	int m_number;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

/**
 * Use this function to check if variable exists in variables list.
 * @param variable pointer to variable which existance is meant to be checked
 * @param variables list of variables to check existance of variable
 * @return true if variable exists in variables list; false otherwise
 */
bool variableExists(Variable* variable, Variables variables);

void printAll(Variables variables, Instructions instructions, Labels labels);

bool labelExists(Labels labels, Label* label);

#endif
