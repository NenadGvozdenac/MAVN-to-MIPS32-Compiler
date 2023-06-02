#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"
#include "IR.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Variables& vars, Instructions& ins, Labels& labs) :
	lexicalAnalysis(lex), 
	errorFound(false), 
	tokenIterator(lexicalAnalysis.getTokenList().begin()),
	variables(vars),
	instructions(ins),
	labels(labs) {
}


bool SyntaxAnalysis::doSyntaxAnalysis() 
{
	currentToken = getNextToken();
	SyntaxAnalysis::Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token) 
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t) 
{
	if (errorFound == false) {
		if (currentToken.getType() == t) {
			cout << currentToken.getValue() << endl;
			if(currentToken.getType() != T_END_OF_FILE)
				currentToken = getNextToken();
		} else {
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken() 
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

// Q -> S ; L
void SyntaxAnalysis::Q() 
{
	if (this->errorFound != true) {
		S();
		switch (this->currentToken.getType()) {
			case T_SEMI_COL:
				eat(T_SEMI_COL);
				L();
				break;
		}
	}
}

/**
 * S -> _mem mid num
 * S -> _reg rid
 * S -> _func id
 * S -> id: E
 * S -> E
 */
void SyntaxAnalysis::S() 
{
	if (errorFound == false) {
		string variableName;
		string variableValue;
		Label* label;
		Variable* v;
		
		switch (this->currentToken.getType()) 
		{
			case T_MEM:
				eat(T_MEM);
				
				variableName = this->currentToken.getValue();
				eat(T_M_ID);
				
				variableValue = this->currentToken.getValue();
				eat(T_NUM);

				v = new Variable(variableName, 0, Variable::MEM_VAR);

				if (variableExists(v, variables)) {
					errorFound = true;
					break;
				}

				v->setValue(stoi(variableValue));
				variables.push_back(v);
				break;
			case T_REG:
				eat(T_REG);

				variableName = this->currentToken.getValue();

				v = new Variable(variableName, regPosition++, Variable::REG_VAR);

				if (variableExists(v, variables)) {
					errorFound = true;
					break;
				}

				variables.push_back(v);
				eat(T_R_ID);
				break;

			case T_FUNC:
				eat(T_FUNC);
				label = new Label(instructionPosition, this->currentToken.getValue());

				if (labelExists(labels, label)) {
					errorFound = true;
					break;
				}

				labels.push_back(label);
				eat(T_ID);
				break;

			case T_ID:
				label = new Label(instructionPosition, this->currentToken.getValue());

				if (labelExists(labels, label)) {
					errorFound = true;
					break;
				}

				labels.push_back(label);
				eat(T_ID);
				eat(T_COL);
				E();
				break;

			default:
				E();
		}
	}
}

/**
 * S -> _mem mid num
 * L -> eof
 * L -> Q
 */
void SyntaxAnalysis::L() 
{
	if (errorFound == false) 
	{
		switch (this->currentToken.getType()) 
		{
			case T_END_OF_FILE:
				eat(T_END_OF_FILE);
				break;
			default:
				Q();
		}
	}
}

/**
	E → add rid, rid, rid
	E → addi rid, rid, num
	E → sub rid, rid, rid
	E → la rid, mid
	E → lw rid, num(rid)
	E → li rid, num
	E → sw rid, num(rid)
	E → b id
	E → bltz rid, id
	E → nop
 */

int FindVariable(std::string s, Variables& variables) 
{
	for (Variable* it : variables) 
	{
		if (it->getName() == s) 
		{
			return it->getPosition();
		}
	}

	return -1;
}

void SyntaxAnalysis::E()
{
	if (errorFound == false) 
	{
		string nameOfVariableOne, nameOfVariableTwo, nameOfVariableThree;
		string valueOfVariableOne, valueOfVariableTwo;
		Variables destination, source;
		Instruction* instruction;

		switch (this->currentToken.getType()) 
		{
		case T_ADD:
			eat(T_ADD);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableThree = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableThree, FindVariable(nameOfVariableThree, variables), Variable::REG_VAR));
			eat(T_R_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_ADD, destination, source, "add `d, `s, `s");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_ADDI:
			eat(T_ADDI);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			valueOfVariableOne = this->currentToken.getValue();
			eat(T_NUM);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_ADDI, destination, source, "addi `d, `s, `i");
			instruction->m_number = stoi(valueOfVariableOne);
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_SUB:
			eat(T_SUB);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableThree = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableThree, FindVariable(nameOfVariableThree, variables), Variable::REG_VAR));
			eat(T_R_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_SUB, destination, source, "sub `d, `s, `s");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_LA:
			eat(T_LA);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::MEM_VAR));
			eat(T_M_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_LA, destination, source, "la `d, `s");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_LW:
			eat(T_LW);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			valueOfVariableOne = this->currentToken.getValue();
			eat(T_NUM);

			eat(T_L_PARENT);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_R_PARENT);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_LW, destination, source, "lw `d, `i(`s)");
			instructions.push_back(instruction);
			instruction->m_number = stoi(valueOfVariableOne);
			destination.clear();
			source.clear();
			break;

		case T_LI:
			eat(T_LI);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			valueOfVariableOne = this->currentToken.getValue();
			eat(T_NUM);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_LI, destination, source, "li `d, `i");
			instructions.push_back(instruction);
			instruction->m_number = stoi(valueOfVariableOne);
			destination.clear();
			source.clear();
			break;

		case T_SW:
			eat(T_SW);

			nameOfVariableOne = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			valueOfVariableOne = this->currentToken.getValue();
			eat(T_NUM);

			eat(T_L_PARENT);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_R_PARENT);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_SW, destination, source, "sw `s, `i(`s)");
			instructions.push_back(instruction);
			instruction->m_number = stoi(valueOfVariableOne);
			destination.clear();
			source.clear();
			break;

		case T_B:
			eat(T_B);

			nameOfVariableOne = this->currentToken.getValue();
			eat(T_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_B, destination, source, "b `n");

			for (auto el = labels.begin(); el != labels.end(); el++) {
				if (nameOfVariableTwo == (*el)->getNameOfLabel()) {
					instruction->m_label = (*el);
				}
			}

			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_BLTZ:
			eat(T_BLTZ);

			nameOfVariableOne = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			eat(T_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_BLTZ, destination, source, "bltz `s, `n");

			for (auto el = labels.begin(); el != labels.end(); el++) {
				if (nameOfVariableTwo == (*el)->getNameOfLabel()) {
					instruction->m_label = (*el);
				}
			}

			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_NOP:
			eat(T_NOP);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_NOP, destination, source, "nop");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		// Added new instructions (3)
		case T_ADDU:
			eat(T_ADDU);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableThree = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableThree, FindVariable(nameOfVariableThree, variables), Variable::REG_VAR));
			eat(T_R_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_ADDU, destination, source, "addu `d, `s, `s");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_OR:
			eat(T_OR);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			nameOfVariableThree = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableThree, FindVariable(nameOfVariableThree, variables), Variable::REG_VAR));
			eat(T_R_ID);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_OR, destination, source, "or `d, `s, `s");
			instructions.push_back(instruction);
			destination.clear();
			source.clear();
			break;

		case T_LB:
			eat(T_LB);

			nameOfVariableOne = this->currentToken.getValue();
			destination.push_back(new Variable(nameOfVariableOne, FindVariable(nameOfVariableOne, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_COMMA);

			valueOfVariableOne = this->currentToken.getValue();
			eat(T_NUM);

			eat(T_L_PARENT);

			nameOfVariableTwo = this->currentToken.getValue();
			source.push_back(new Variable(nameOfVariableTwo, FindVariable(nameOfVariableTwo, variables), Variable::REG_VAR));
			eat(T_R_ID);

			eat(T_R_PARENT);

			instruction = new Instruction((*labels.rbegin())->getNameOfLabel(), instructionPosition++, I_LB, destination, source, "lb `d, `i(`s)");
			instructions.push_back(instruction);
			instruction->m_number = stoi(valueOfVariableOne);
			destination.clear();
			source.clear();
			break;

		default:
			this->errorFound = true;
			printSyntaxError(this->currentToken);
		}
	}
}

void SyntaxAnalysis::findPredAndSucc(Instructions& instructions) 
{

	for (Instruction* instruction : instructions) 
	{
		if (instruction->getType() == InstructionType::I_B) 
		{
			for (Instruction* instruction2 : instructions) 
			{
				if (instruction2->m_position == instruction->m_label->getPosition())
				{
					instruction->m_succ.push_back(instruction2);
					instruction2->m_pred.push_back(instruction);
				}
			}
		}
		else 
		{
			for (Instruction* instruction2 : instructions) 
			{
				if (instruction->m_position + 1 == instruction2->m_position) 
				{
					instruction->m_succ.push_back(instruction2);
					instruction2->m_pred.push_back(instruction);
				}
				else if (instruction->getType() == InstructionType::I_BLTZ) 
				{
					if (instruction->m_label->getPosition() == instruction2->m_position)
					{
						instruction->m_succ.push_back(instruction2);
						instruction2->m_pred.push_back(instruction);
					}
				}
			}
		}
	}
}

void SyntaxAnalysis::printInstructions(Instructions instructions)
{
	for (auto it = instructions.begin(); it != instructions.end(); it++) 
	{
		int counter = 0;

		std::cout << "Type: " << (*it)->getTypeToString() << "\tPos: " << (*it)->m_position << std::endl;
		std::cout << "------------------------------------------------------------------------" << endl;
		for (Variables::iterator it2 = (*it)->m_dst.begin(); it2 != (*it)->m_dst.end(); it2++) 
		{
			std::cout << "Dest" << ++counter << ":\tName: " << (*it2)->getName() << ",\tPos: " << (*it2)->getPosition() <<
				",\tAssignment: " << (*it2)->getVariableAssignment() << std::endl;
		}
		counter = 0;
		for (auto it2 = (*it)->m_src.begin(); it2 != (*it)->m_src.end(); it2++) 
		{
			std::cout << "Src" << ++counter << ":\tName: " << (*it2)->getName() << ",\tPos: " << (*it2)->getPosition() <<
				",\tAssignment: " << (*it2)->getVariableAssignment() << std::endl;
		}
		std::cout << "------------------------------------------------------------------------" << endl;
		std::cout << "PRED: \t";
		for (auto it2 = (*it)->m_pred.begin(); it2 != (*it)->m_pred.end(); it2++) 
		{
			std::cout << (*it2)->m_position << "\t";
		}
		std::cout << std::endl;

		std::cout << "SUCC: \t";
		for (auto it2 = (*it)->m_succ.begin(); it2 != (*it)->m_succ.end(); it2++) 
		{
			std::cout << (*it2)->m_position << "\t";
		}
		std::cout << std::endl;

		std::cout << "USE: \t";
		for (auto it2 = (*it)->m_use.begin(); it2 != (*it)->m_use.end(); it2++) 
		{
			if((*it2)->getVariableType() != Variable::MEM_VAR)
				std::cout << (*it2)->getName() << "\t";
		}
		std::cout << std::endl;

		std::cout << "DEF:\t";
		for (auto it2 = (*it)->m_def.begin(); it2 != (*it)->m_def.end(); it2++) 
		{
			if ((*it2)->getVariableType() != Variable::MEM_VAR)
				std::cout << (*it2)->getName() << "\t";
		}
		std::cout << std::endl;

		std::cout << "IN:\t";
		for (auto it2 = (*it)->m_in.begin(); it2 != (*it)->m_in.end(); it2++) 
		{
			if ((*it2)->getVariableType() != Variable::MEM_VAR)
				std::cout << (*it2)->getName() << "\t";
		}
		std::cout << std::endl;

		std::cout << "OUT:\t";
		for (auto it2 = (*it)->m_out.begin(); it2 != (*it)->m_out.end(); it2++) 
		{
			if ((*it2)->getVariableType() != Variable::MEM_VAR)
				std::cout << (*it2)->getName() << "\t";
		}
		std::cout << std::endl;
		std::cout << "------------------------------------------------------------------------" << endl;
		std::cout << "------------------------------------------------------------------------" << endl;
		std::cout << std::endl << std::endl;
	}
}


// Empty
void SyntaxAnalysis::R() {
}