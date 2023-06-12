// Autor: Nenad Gvozdenac, Datum: 07.06.2023

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
	// Variable type used
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	// Constructors
	Variable() : 
		m_variableType(VariableType::NO_TYPE), 
		m_variableName(""),
		m_variablePosition(-1),
		m_variableAssignment(Regs::NO_ASSIGN),
		m_value(0) {}
	
	Variable(std::string name, int pos, VariableType variableType) : 
		m_variableName(name),
		m_variablePosition(pos),
		m_variableType(variableType),
		m_variableAssignment(Regs::NO_ASSIGN),
		m_value(0) {}

	// Destructor
	~Variable();

	// Function that retrieves the variable type of this object
	VariableType& Variable::getVariableType();
	
	// Function that retrieves the name of the variable
	std::string& Variable::getName();

	// Function that retrieves the position of the variable
	int& Variable::getPosition();

	// Function that retrieves the register of this variable (if it is of type REG_VAR)
	Regs& Variable::getVariableAssignment();

	// Function that retrieves the value of this variable (if it is of type MEM_REG)
	int& Variable::getValue();

	// Function that sets the variable type of this object
	void Variable::setVariableType(VariableType type);

	// Function that sets the variable name of this object
	void Variable::setName(std::string name);

	// Function that sets the variable position of this object
	void Variable::setPosition(int position);

	// Function that sets the variable assignment of this object
	void Variable::setVariableAssignment(Regs regs);

	// Function that sets the variable value of this object
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
typedef std::list<std::shared_ptr<Variable>> Variables;

/**
 * This class represents one label in program code.
 */
class Label { 
private:
	std::string name;
	int position;
public:
	Label(int position, std::string name) : name(name), position(position) {}

	// Destructor
	~Label();

	// Function that retrieves the name
	std::string getNameOfLabel();

	// Function that retrieves a position
	int getPosition();
};

typedef std::list<std::shared_ptr<Label>> Labels;

/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	// Constructors
	Instruction () : m_position(0), m_type(I_NO_TYPE), m_number(0) {}
	Instruction (std::string label, int pos, InstructionType type, Variables& dst, Variables& src, std::string instructionTemplate) :
		m_labelStr(label),  
		m_position(pos), 
		m_type(type), 
		m_dst(dst), 
		m_src(src),
		m_def(dst),
		m_use(src),
		m_instructionTemplate(instructionTemplate),
		m_number(0)
	{}

	// Destructor
	~Instruction();

	// Function that retrieves the source registers
	Variables getSrc();
	
	// Function that retrieves the destination registers
	Variables getDst();

	// Function that outputs a list to console
	static std::string outputList(std::list<std::shared_ptr<Instruction>> list);

	// Function that retrieves the type of the instruction, in string format
	std::string getTypeToString();

	// Function that maps variables into instruction's variables
	static void normalizeAssignmentsToVariables(std::list<std::shared_ptr<Instruction>>& instructions, Variables variables);

	// Data
	std::string m_labelStr;
	std::shared_ptr<Label> m_label;
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
	std::list<std::shared_ptr<Instruction>> m_succ;
	std::list<std::shared_ptr<Instruction>> m_pred;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<std::shared_ptr<Instruction>> Instructions;

/**
 * Use this function to check if variable exists in variables list.
 * @param variable pointer to variable which existance is meant to be checked
 * @param variables list of variables to check existance of variable
 * @return true if variable exists in variables list; false otherwise
 */
bool variableExists(std::shared_ptr<Variable> variable, std::list<std::shared_ptr<Variable>> variables);

/**
 * Use this function to print all variables, instructions and labels to console.
 * @param variables
 * @param variables instructions
 * @param variables labels
 */
void printAll(Variables variables, Instructions instructions, Labels labels);

/**
 * Use this function to check whether a label exists in labels
 * @param labels
 * @param label
 * @return boolean: exists
 */
bool labelExists(Labels labels, std::shared_ptr<Label> label);

#endif
